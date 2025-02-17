#!/bin/bash

set -e

readonly RELEASE_DIRECTORY_LINUX="rogue-forever-linux-${RELEASE_TAG}"
readonly RELEASE_DIRECTORY_WINDOWS="rogue-forever-windows-${RELEASE_TAG}"
readonly RELEASE_ZIP_LINUX="${RELEASE_DIRECTORY_LINUX}.zip"

readonly TEMP_SKIP_BUILDS="TRUE"

cd $(dirname $0)

readonly ROGUE_FOREVER_BASE_PATH="$(pushd .. &> /dev/null; pwd ; popd &> /dev/null)"

error_exit() {
    echo "${1}"
    exit 1
}

install_prerequisites() {
    echo "Installing prerequisites"

    sudo apt update
    sudo apt -y upgrade

    which jq || sudo apt install jq
    which zip || sudo apt install zip
}

create_release_package_linux() {
    if [ "TRUE" != ${TEMP_SKIP_BUILDS} ] ; then
        make
    fi

    mkdir ${RELEASE_DIRECTORY_LINUX}

    cp scripts/install_runtime_dependencies_release.sh ${RELEASE_DIRECTORY_LINUX}

    # TODO move all remaining release contents

    zip -r "${RELEASE_ZIP_LINUX}" "${RELEASE_DIRECTORY_LINUX}"
}

create_release_package_windows() {
    error_exit "create_release_package_windows is not yet implemented"
}

create_release_packages() {
    if [ "TRUE" != ${TEMP_SKIP_BUILDS} ] ; then
        ./scripts/install_build_dependencies.sh

        make test
    fi

    create_release_package_linux

    # create_release_package_windows
}

main() {
    local -r github_api_version="2022-11-28"

    local output_curl=''
    local rogue_forever_upload_url=''

    if [ -z "${GH_TOKEN}" ] ; then
        error_exit "GH_TOKEN is empty. Please set a value for GH_TOKEN."
    fi

    pushd ${ROGUE_FOREVER_BASE_PATH}

    echo -e "ABTEST RELEASE_TAG is: ${RELEASE_TAG}"

    install_prerequisites

    create_release_packages

    # Post new release to GitHub
    output_curl="$(curl -L \
        -X POST \
        -H "Accept: application/vnd.github+json" \
        -H "Authorization: Bearer ${GH_TOKEN}" \
        -H "X-GitHub-Api-Version: ${github_api_version}" \
        https://api.github.com/repos/pottumuusi/rogue-forever/releases \
        -d "{\"tag_name\":\"${RELEASE_TAG}\"}")"

    rogue_forever_upload_url=$(echo ${output_curl} \
        | jq '.upload_url' \
        | cut -d '{' -f 1 \
        | tr -d \")

    if [ "null" == "${rogue_forever_upload_url}" ] ; then
        error_exit "Assets URL is null"
    fi

    # Post release assets to GitHub
    curl -L \
        -X POST \
        -H "Accept: application/vnd.github+json" \
        -H "Authorization: Bearer ${GH_TOKEN}" \
        -H "X-GitHub-Api-Version: ${github_api_version}" \
        -H "Content-Type: application/octet-stream" \
        "${rogue_forever_upload_url}?name=${RELEASE_ZIP_LINUX}" \
        --data-binary "@${RELEASE_ZIP_LINUX}"

    popd # ${ROGUE_FOREVER_BASE_PATH}
}

main "${@}"