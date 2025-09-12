#!/bin/bash
set -e
BUILD=$(pwd)/../build

printf "🔧 Configuring...\n"
cmake -B $BUILD -S ../ -DCMAKE_INSTALL_PREFIX=/usr

printf "🔨 Building...\n"
cmake --build $BUILD

printf "📦 Installing...\n"
sudo cmake --install $BUILD

printf "✅ Done. You can now use 'port-scanner'\n"
