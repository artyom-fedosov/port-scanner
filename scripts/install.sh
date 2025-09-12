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

sudo -v

readonly PROJECT="$(dirname "$(dirname "$(realpath "$BASH_SOURCE")")")"
cd "${PROJECT}"

readonly BUILD=build/
readonly SOURCE="${PROJECT}"

printf "🔧 Configuring...\n"
cmake -B "${BUILD}" -S "${SOURCE}" -DCMAKE_INSTALL_PREFIX=/usr

printf "🔨 Building...\n"
cmake --build "${BUILD}"

printf "📦 Installing...\n"
sudo cmake --install "${BUILD}"

printf "✅ Done. You can now use 'port-scanner'\n"
