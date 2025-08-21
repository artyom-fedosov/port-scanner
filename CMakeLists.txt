cmake_minimum_required(VERSION 3.16)

project(port-scanner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES src/*.cpp)
add_executable(port-scanner ${SOURCES})

include(GNUInstallDirs)
install(TARGETS port-scanner RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES man/port-scanner.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
