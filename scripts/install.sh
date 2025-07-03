#!/bin/bash
set -e

echo "🔧 Configuring..."
cmake -B build -DCMAKE_INSTALL_PREFIX=/usr

echo "🔨 Building..."
cmake --build build

echo "📦 Installing..."
sudo cmake --install build

echo "✅ Done. You can now use 'port-scanner'."
