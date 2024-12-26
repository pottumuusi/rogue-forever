#!/bin/bash

error_exit() {
    echo "${1}"
    exit 1
}

main() {
    if [ -z "${GH_TOKEN}" ] ; then
        error_exit "GH_TOKEN is empty. Please set a value for GH_TOKEN."
    fi

    curl -L \
        -H "Accept: application/vnd.github+json" \
        -H "Authorization: Bearer ${GH_TOKEN}" \
        -H "X-GitHub-Api-Version: 2022-11-28" \
        https://api.github.com/repos/pottumuusi/rogue-forever/issues
}

main "${@}"