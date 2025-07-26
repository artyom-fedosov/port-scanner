#!/bin/bash
set -e
BUILD=$(pwd)/../build

echo "🧹 Cleaning previous build..."
rm -rf $BUILD
mkdir $BUILD

echo "🔧 Configuring project with CMake..."
cmake -B $BUILD -S ../ -DCMAKE_INSTALL_PREFIX=/usr

echo "🔨 Building project..."
cmake --build $BUILD

echo "📦 Creating packages with CPack..."
cpack --config $BUILD/CPackConfig.cmake --package-directory $BUILD

echo "✅ Packaging complete. Generated packages:"
ls -lh $BUILD/*.deb $BUILD/*.rpm 2>/dev/null ||
        echo "No packages found in build directory."
