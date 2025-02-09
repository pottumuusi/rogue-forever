#!/bin/bash

set -e

cd $(dirname $0)

readonly ROGUE_FOREVER_BASE_PATH="$(pushd .. &> /dev/null; pwd ; popd &> /dev/null)"

error_exit() {
    echo "${1}"
    exit 1
}

create_release_packages() {
    pushd ${ROGUE_FOREVER_BASE_PATH}

    ./scripts/install_build_dependencies.sh

    make test

    # TODO create Linux release package

    # TODO create Windows release package

    popd # ${ROGUE_FOREVER_BASE_PATH}
}

main() {
    if [ -z "${GH_TOKEN}" ] ; then
        error_exit "GH_TOKEN is empty. Please set a value for GH_TOKEN."
    fi

    echo -e "ABTEST RELEASE_TAG is: ${RELEASE_TAG}"

    create_release_packages

    echo "ABTEST exiting early. Testing release package creation."
    exit 1

    # Post new release to GitHub
    curl -L \
        -X POST \
        -H "Accept: application/vnd.github+json" \
        -H "Authorization: Bearer ${GH_TOKEN}" \
        -H "X-GitHub-Api-Version: 2022-11-28" \
        https://api.github.com/repos/pottumuusi/rogue-forever/releases \
        -d "{\"tag_name\":\"${RELEASE_TAG}\"}"

    # TODO Upload release packages
}

main "${@}"