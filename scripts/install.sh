#!/bin/bash
set -e

check_utility() {
    local utility="${1}"
    if ! command -v "${utility}" >/dev/null 2>&1; then
        printf "❌ %s is required!\n" "${utility}" >&2
        false
    fi
}

for utility in rm mkdir cmake; do
    check_utility "${utility}"
done

BUILD="$(pwd)"/../build

printf "🔧 Configuring...\n"
cmake -B "${BUILD}" -S ../ -DCMAKE_INSTALL_PREFIX=/usr

printf "🔨 Building...\n"
cmake --build "${BUILD}"

printf "📦 Installing...\n"
sudo cmake --install "${BUILD}"

printf "✅ Done. You can now use 'port-scanner'\n"
