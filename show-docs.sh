#!/usr/bin/env bash
#
# Generate Doxygen HTML docs and open them in the default browser

set -e          # exit on error
set -u          # error on unset variables

OUTPUT_DIR="$(awk '/^OUTPUT_DIRECTORY[[:space:]]*=/ {print $3}' Doxyfile)"
OUTPUT_DIR="${OUTPUT_DIR:-.}"
INDEX_FILE_PATH="${OUTPUT_DIR}/html/index.html"

doxygen Doxyfile

if [[ ! -f "${INDEX_FILE_PATH}" ]]; then
    printf "Error: ${INDEX_FILE_PATH} not found.\n"
    exit 1
fi

if command -v xdg-open >/dev/null; then
    xdg-open "${INDEX_FILE_PATH}"
else
    printf "Please open ${INDEX_FILE_PATH} manually.\n"
fi
