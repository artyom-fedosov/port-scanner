#!/usr/bin/env bash
#
# Adds [[nodiscard]] attributes at the start of functions than return non-void values

ROOT="${1:-.}"
for file in $(find "${ROOT}" -type f); do
    printf "File: %s\n" "${file}"
    lineNumber=0
    while read -r line; do
        ((lineNumber++))
        
        if [[ "$line" =~ ^[[:space:]]*(void|return) ]]; then
            continue
        fi
    
        printf "%s\n" "${line}" | grep -P '^\s*[_a-zA-Z][\w:<>\*&]+\s+[_a-zA-Z][\w:]*\s*\(.*\)\s*{?'
        if [[ $? -eq 0 ]]; then
            printf "Do you want to place? (y, n): "
            read </dev/tty
            if [[ ${REPLY} == y ]]; then
                sed -E -i "${lineNumber}s/^([[:space:]]*)([_[:alpha:]][[:alnum:]_:<>\*&]+[[:space:]]+[_[:alpha:]][[:alnum:]_:]*)/\1[[nodiscard]] \2/" "${file}"
            fi
        fi
    done <"${file}"
done

