#!/bin/bash
set -e
BUILD=$(pwd)/../build

printf "🔧 Configuring..."
cmake -B $BUILD -S ../ -DCMAKE_INSTALL_PREFIX=/usr

printf "🔨 Building..."
cmake --build $BUILD

printf "📦 Installing..."
sudo cmake --install $BUILD

printf "✅ Done. You can now use 'port-scanner'."
