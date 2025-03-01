#!/bin/bash

set -e

readonly RELEASE_DIRECTORY_WINDOWS="rogue-forever-windows-${RELEASE_TAG}"
readonly RELEASE_ZIP_WINDOWS="${RELEASE_DIRECTORY_WINDOWS}.zip"

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

create_release_package_windows() {
    mkdir ${RELEASE_DIRECTORY_WINDOWS}

    make windows

    cp --verbose \
        scripts/install_runtime_dependencies_release.ps1 \
        ${RELEASE_DIRECTORY_WINDOWS}

    cp --verbose \
        rogue_forever.exe \
        ${RELEASE_DIRECTORY_WINDOWS}

    zip -r "${RELEASE_ZIP_WINDOWS}" "${RELEASE_DIRECTORY_WINDOWS}"
}

create_release_packages() {
    ./scripts/install_build_dependencies/install_build_dependencies.sh full

    make test

    create_release_package_windows
}

post_new_release_to_github() {
    output_curl="$(curl -L \
        -X POST \
        -H "Accept: application/vnd.github+json" \
        -H "Authorization: Bearer ${GH_TOKEN}" \
        -H "X-GitHub-Api-Version: ${github_api_version}" \
        https://api.github.com/repos/pottumuusi/rogue-forever/releases \
        -d "{\"tag_name\":\"${RELEASE_TAG}\"}")"

    rogue_forever_upload_url="$(echo ${output_curl} \
        | jq '.upload_url' \
        | cut -d '{' -f 1 \
        | tr -d \")"
}

post_release_asset_to_github() {
    local uploaded_asset_name=''

    local release_asset="${1}"

    output_curl="$(curl -L \
        -X POST \
        -H "Accept: application/vnd.github+json" \
        -H "Authorization: Bearer ${GH_TOKEN}" \
        -H "X-GitHub-Api-Version: ${github_api_version}" \
        -H "Content-Type: application/octet-stream" \
        "${rogue_forever_upload_url}?name=${release_asset}" \
        --data-binary "@${release_asset}")"

    uploaded_asset_name=$(echo ${output_curl} \
        | jq '.name' \
        | tr -d \")

    if [ "${uploaded_asset_name}" != "${release_asset}" ] ; then
        error_exit "Release asset name ${release_asset} not detected from response."
    fi
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

    post_new_release_to_github
    if [ "null" == "${rogue_forever_upload_url}" ] ; then
        error_exit "Assets URL is null. Does the release already exist?"
    fi

    post_release_asset_to_github ${RELEASE_ZIP_WINDOWS}

    popd # ${ROGUE_FOREVER_BASE_PATH}
}

main "${@}"