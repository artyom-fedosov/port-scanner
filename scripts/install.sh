#!/bin/bash
set -e
BUILD=$(pwd)/../build

echo "🔧 Configuring..."
cmake -B $BUILD -S ../ -DCMAKE_INSTALL_PREFIX=/usr

echo "🔨 Building..."
cmake --build $BUILD

echo "📦 Installing..."
sudo cmake --install $BUILD

echo "✅ Done. You can now use 'port-scanner'."
