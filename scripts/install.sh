#!/bin/bash
#
# Install port-scanner utility and its man page

source "$(dirname "$(realpath "${BASH_SOURCE[0]}")")/common.sh"

readonly REQUIRED_UTILITIES=(rm mkdir cmake dirname realpath)
check_utilities "${REQUIRED_UTILITIES[@]}"

sudo -v

printf "🔧 Configuring...\n"
cmake -B "${BUILD}" -S "${SOURCE}" -DCMAKE_INSTALL_PREFIX=/usr

printf "🔨 Building...\n"
cmake --build "${BUILD}"

printf "📦 Installing...\n"
sudo cmake --install "${BUILD}"

printf "✅ Done\n"
