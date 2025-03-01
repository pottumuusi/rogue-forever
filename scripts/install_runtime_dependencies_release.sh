#!/bin/bash

set -e

readonly INSTALL_ASSETS="TRUE"
readonly INSTALL_MAPS="TRUE"

cd $(dirname $0)

main() {
    if [ "TRUE" == "${INSTALL_ASSETS}" ] ; then
        echo "Installing assets"
        wget http://gitlab.justworks.today/rogue-forever/assets_basic.zip
        unzip assets_basic.zip
    fi

    if [ "TRUE" == "${INSTALL_MAPS}" ] ; then
        echo "Installing maps"
        wget http://gitlab.justworks.today/rogue-forever/maps_basic.zip
        unzip maps_basic.zip
    fi
}

main "${@}"