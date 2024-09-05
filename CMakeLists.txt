cmake_minimum_required(VERSION 3.16)

project(ServiceContainer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ServiceContainer main.cpp
    1.txt)

include(GNUInstallDirs)
install(TARGETS ServiceContainer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
