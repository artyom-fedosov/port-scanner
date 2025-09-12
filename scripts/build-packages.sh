#!/bin/bash
set -e
BUILD=$(pwd)/../build

printf "🧹 Cleaning previous build..."
rm -rf $BUILD
mkdir $BUILD

printf "🔧 Configuring project with CMake..."
cmake -B $BUILD -S ../ -DCMAKE_INSTALL_PREFIX=/usr

printf "🔨 Building project..."
cmake --build $BUILD

printf "📦 Creating packages with CPack..."
cpack --config $BUILD/CPackConfig.cmake --package-directory $BUILD

printf "✅ Packaging complete. Generated packages:"
ls -lh $BUILD/*.deb $BUILD/*.rpm 2>/dev/null ||
        printf "No packages found in build directory."
