#!/bin/bash
set -e

check_utility() {
    local utility="${1}"
    if ! command -v "${utility}" >/dev/null 2>&1; then
        printf "❌ %s is required!\n" "${utility}" >&2
        false
    fi
}

for utility in rm mkdir cmake cpack; do
    check_utility "${utility}"
done

BUILD="$(pwd)"/../build

printf "🧹 Cleaning previous build...\n"
rm -rf "${BUILD}"
mkdir "${BUILD}"

printf "🔧 Configuring project with CMake...\n"
cmake -B "${BUILD}" -S ../ -DCMAKE_INSTALL_PREFIX=/usr

printf "🔨 Building project...\n"
cmake --build "${BUILD}"

printf "📦 Creating packages with CPack...\n"
cpack --config "${BUILD}"/CPackConfig.cmake --package-directory "${BUILD}"

printf "✅ Packaging complete. Generated packages:\n"
ls -lh "${BUILD}"/*.deb "${BUILD}"/*.rpm 2>/dev/null ||
        printf "No packages found in build directory\n"
