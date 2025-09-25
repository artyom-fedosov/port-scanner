cmake_minimum_required(VERSION 3.16)

project(port-scanner
        DESCRIPTION "TCP port scanner"
        HOMEPAGE_URL https://github.com/artyom-fedosov/port-scanner
        LANGUAGES CXX)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -Wshadow -Wconversion\
        -Wsign-conversion -Wold-style-cast -Wuseless-cast\
        -Wzero-as-null-pointer-constant -Wmisleading-indentation\
        -Wredundant-decls -Wattributes -Wempty-body\
        -Wmissing-field-initializers -Wmaybe-uninitialized\
        -Wsuggest-attribute=const -Wsuggest-attribute=pure\
        -Wsuggest-attribute=noreturn -Wswitch-default -Wfloat-equal\
        -Wextra-semi -Wnull-dereference -Wduplicated-branches -Wlogical-op\
        -Wparentheses -Wsequence-point -Wunused-result\
        -Wconditionally-supported -Wignored-qualifiers -Wdisabled-optimization\
        -Wunreachable-code -Wformat-overflow -Wformat-truncation\
        -Wstrict-aliasing -Wcast-align -g")
set(CMAKE_CXX_FLAGS_RELEASE -O3)
set(CMAKE_CXX_CLANG_TIDY clang-tidy CACHE STRING "Enable clang-tidy")

add_executable(port-scanner)

target_sources(port-scanner
PRIVATE
        src/main.cpp
        src/scanner.cpp
        src/printer.cpp
)

set_target_properties(port-scanner PROPERTIES
        CXX_CLANG_TIDY "clang-tidy"
)

target_include_directories(port-scanner
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

include(GNUInstallDirs)
install(TARGETS port-scanner RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES man/port-scanner.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

# ===== Packaging =====
set(CPACK_GENERATOR RPM;DEB;TGZ)

set(CPACK_PACKAGE_NAME port-scanner)
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR})
set(CPACK_PACKAGE_VERSION 1.1.0)
set(CPACK_PACKAGE_CONTACT artjomsfedosovs2@gmail.com)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Lightweight TCP port scanner for Linux, written in C++.")
set(CPACK_PACKAGE_VENDOR "Artyom Fedosov")
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})

set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Artyom Fedosov")
set(CPACK_DEBIAN_PACKAGE_SECTION net)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.27)")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)

set(CPACK_RPM_PACKAGE_LICENSE MIT)
set(CPACK_RPM_PACKAGE_GROUP Applications/Internet)
set(CPACK_RPM_PACKAGE_REQUIRES "glibc >= 2.27")
set(CPACK_RPM_PACKAGE_DESCRIPTION "Lightweight TCP port scanner for Linux, written in C++.")
set(CPACK_RPM_PACKAGE_ARCHITECTURE x86_64)

include(CPack)

# ===== Tests =====
enable_testing()

add_executable(tests
        tests/test_scanner.cpp
        src/scanner.cpp
)

target_include_directories(tests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)

add_test(NAME Valgrind COMMAND valgrind
        --tool=memcheck
        --leak-check=full
        --show-leak-kinds=all
        --track-origins=yes
        --errors-for-leak-kinds=definite
        --error-exitcode=1
        --undef-value-errors=yes
        --read-var-info=yes
        $<TARGET_FILE:port-scanner> 192.168.8.1 80 443 1 2 3 4 5
)
