#include <iostream>
#include <cassert>
#include <memory>
#include <tuple>
#include <utility>
#include <optional>

class GuiProxy {
public:
    const char *name() const { return "GuiProxy"; }
};

class NotificationFactory {
public:
    const char *name() const { return "NotificationFactory"; }
};

class CommClient {
public:
    CommClient() {}
    virtual ~CommClient() {}
    virtual const char *name() const { return "CommClient"; }
    virtual const char *getMsg() = 0;
};

class MsgClient : public CommClient {
public:
    MsgClient() {}
    ~MsgClient() override {}
    const char *name() const override { return "MsgClient"; }
    const char *getMsg() override { return "Any message"; }
};

class Datamap {
public:
    Datamap(const std::string &path)
        : path_(path) {}

    const char *name() const { return "Datamap"; }
    const char *path() const { return path_.c_str(); }

private:
    const std::string path_;
};


template<typename... Types>
class ServiceContainer {
public:
    ServiceContainer()
        : services_(std::make_tuple(std::unique_ptr<Types>()...)) {}

    template<typename T, typename S = T, typename... Args>
    void install(Args&&... args) {
        install_single<T, S>(services_, std::forward<Args>(args)...);
    }

    template <typename T>
    T& get() const {
        auto& ptr = std::get<std::unique_ptr<T>>(services_);
        assert(ptr && "This type of interface is not installed");
        return *ptr;
    }

private:
    std::tuple<std::unique_ptr<Types>...> services_;

    template<typename T, typename S, typename Tuple, typename... Args>
    void install_single(Tuple& t, Args&&... args) {
        auto& ptr = std::get<std::unique_ptr<T>>(t);
        assert(!ptr && "An interface of this type was previously installed");
        ptr = std::make_unique<S>(std::forward<Args>(args)...);
    }
};


int main()
{
    ServiceContainer< GuiProxy, Datamap, NotificationFactory, CommClient >  container;

    container.install< GuiProxy >();
    container.install< Datamap >("/tmp/datamap.json");
    container.install< NotificationFactory >();
    container.install< CommClient, MsgClient >();

    std::cout << "gp: " << container.get< GuiProxy >().name() << std::endl;
    std::cout << "dm: " << container.get< Datamap >().name() << std::endl;
    std::cout << "nf: " << container.get< NotificationFactory >().name() << std::endl;
    std::cout << "mc: " << container.get< CommClient >().name() << std::endl;

    std::cout << "dm path: " << container.get< Datamap >().path() << std::endl;
    std::cout << "mc msg: " << container.get< CommClient >().getMsg() << std::endl;

    return 0;
}
