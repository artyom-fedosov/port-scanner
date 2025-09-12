#!/bin/bash
set -e
BUILD=$(pwd)/../build

printf "🧹 Cleaning previous build...\n"
rm -rf ${BUILD}
mkdir ${BUILD}

printf "🔧 Configuring project with CMake...\n"
cmake -B ${BUILD} -S ../ -DCMAKE_INSTALL_PREFIX=/usr

printf "🔨 Building project...\n"
cmake --build ${BUILD}

printf "📦 Creating packages with CPack...\n"
cpack --config ${BUILD}/CPackConfig.cmake --package-directory ${BUILD}

printf "✅ Packaging complete. Generated packages:\n"
ls -lh ${BUILD}/*.deb ${BUILD}/*.rpm 2>/dev/null ||
        printf "No packages found in build directory\n"
