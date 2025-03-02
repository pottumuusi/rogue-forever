#!/bin/bash

set -e

readonly INSTALL_ASSETS="TRUE"
readonly INSTALL_MAPS="TRUE"

cd $(dirname $0)

readonly ROGUE_FOREVER_BASE_PATH="$(pushd .. &> /dev/null; pwd ; popd &> /dev/null)"

main() {
    pushd ${ROGUE_FOREVER_BASE_PATH}

    if [ "TRUE" == "${INSTALL_ASSETS}" ] ; then
        echo "Installing assets"
        wget http://gitlab.justworks.today/rogue-forever/assets_basic.zip
        unzip assets_basic.zip
        rm --verbose assets_basic.zip
    fi

    if [ "TRUE" == "${INSTALL_MAPS}" ] ; then
        echo "Installing maps"
        wget http://gitlab.justworks.today/rogue-forever/maps_basic.zip
        unzip maps_basic.zip
        rm --verbose maps_basic.zip
    fi

    popd # ${ROGUE_FOREVER_BASE_PATH}
}

main "${@}"