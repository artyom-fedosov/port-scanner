#!/usr/bin/env bash
#
# Create packages in the build directory

source "$(dirname "$(realpath "${BASH_SOURCE[0]}")")/common.sh"

readonly REQUIRED_UTILITIES=(rm mkdir cmake cpack dirname realpath)
check_utilities "${REQUIRED_UTILITIES[@]}"

printf "🧹 Cleaning previous build...\n"
rm --recursive --force "${BUILD}"
mkdir "${BUILD}"

printf "🔧 Configuring project with CMake...\n"
cmake -B "${BUILD}" -S "${SOURCE}" -DCMAKE_INSTALL_PREFIX=/usr

printf "🔨 Building project...\n"
cmake --build "${BUILD}"

printf "📦 Creating packages with CPack...\n"
cpack --config "${BUILD}"/CPackConfig.cmake --package-directory "${BUILD}"

printf "✅ Packaging complete\n"
