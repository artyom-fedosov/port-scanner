#!/usr/bin/env bash
#
# Common functions and variables for scripts

set -e          # exit on error
set -u          # error on unset variables
set -o pipefail # fail on pipe errors

trap 'printf "❌ Error on line %d: %s\n" "$LINENO" "$BASH_COMMAND" >&2' ERR

check_utility() {
    local utility="$1"
    if ! command -v "${utility}" >/dev/null 2>&1; then
        printf "❌ %s is required!\n" "${utility}" >&2
        exit 1
    fi
}

check_utilities() {
    local utilities=("$@")
    printf "🔍 Checking required utilities...\n"
    for utility in "${utilities[@]}"; do
        check_utility "${utility}"
    done
}

PROJECT="$(dirname "$(dirname "$(realpath "${BASH_SOURCE[0]}")")")"
readonly PROJECT
readonly BUILD="${PROJECT}/build/"
readonly SOURCE="${PROJECT}/"
