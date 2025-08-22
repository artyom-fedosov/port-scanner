#!/bin/bash
set -e

BUILD_DIR=build

echo "🧹 Cleaning previous build..."
rm -rf "$BUILD_DIR"
mkdir "$BUILD_DIR"

echo "🔧 Configuring project with CMake..."
cmake -B "$BUILD_DIR" -DCMAKE_INSTALL_PREFIX=/usr

echo "🔨 Building project..."
cmake --build "$BUILD_DIR"

echo "📦 Creating packages with CPack..."
cpack --config "$BUILD_DIR/CPackConfig.cmake" --package-directory "$BUILD_DIR"

echo "✅ Packaging complete. Generated packages:"
ls -lh "$BUILD_DIR"/*.deb "$BUILD_DIR"/*.rpm 2>/dev/null || echo "No packages found in build directory."
