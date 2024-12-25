#!/bin/bash

set -e

readonly OPERATING_SYSTEM_DEBIAN='Debian GNU/Linux'

OPERATING_SYSTEM=''

error_exit() {
    echo ${1}
    exit 1
}

detect_operating_system_linux() {
    OPERATING_SYSTEM="$(grep "^NAME=" /etc/os-release | cut -d '=' -f 2)"

    echo "Detected Linux operating system: ${OPERATING_SYSTEM}"
}

detect_operating_system() {
    if [ -f '/etc/os-release' ] ; then
        detect_operating_system_linux
        return
    fi

    error_exit "Unknown operating system"
}

install_wget_debian() {
    echo "Upgrading packages and installing using sudo"

    sudo apt update
    sudo apt upgrade
    sudo apt install wget
}

install_wget() {
    echo "Installing wget"

    echo "Comparing"
    echo "${OPERATING_SYSTEM_DEBIAN}"
    echo "${OPERATING_SYSTEM}"
    if [ "${OPERATING_SYSTEM_DEBIAN}" == "${OPERATING_SYSTEM}" ] ; then
        install_wget_debian
    fi
}

main() {
    detect_operating_system

    echo "This script will the following utilities if they are missing:"
    echo -e "\t wget"
    read -p "Continue and install rogue-forever? (y/n) : " user_input

    if [ "y" != "${user_input}" ] ; then
        echo "Exiting installer."
        exit 0
    fi

    which wget
    if [ "0" == "${?}" ] ; then # TODO negate
        install_wget
    fi
}

main "${@}"