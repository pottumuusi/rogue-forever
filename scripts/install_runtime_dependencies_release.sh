#!/bin/bash

set -e

readonly INSTALL_ASSETS="TRUE"
readonly INSTALL_MAPS="TRUE"
readonly INSTALL_LIBSDL2="TRUE"

cd $(dirname $0)

install_libsdl2_debian() {
    echo "Installing libsdl2-2.0-0"
    sudo apt update
    sudo apt install libsdl2-2.0-0
}

install_libsdl2() {
    install_libsdl2_debian
}

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

    if [ "TRUE" == "${INSTALL_LIBSDL2}" ] ; then
        install_libsdl2
    fi
}

main "${@}"