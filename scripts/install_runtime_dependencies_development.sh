#!/bin/bash

set -e

readonly INSTALL_ASSETS="TRUE"
readonly INSTALL_MAPS="TRUE"
readonly INSTALL_SDL2_WINDOWS_DLL="TRUE"

cd $(dirname $0)

main() {
    local -r workarea_path='/tmp/rogue_forever_runtime_dependencies'
    local -r rogue_forever_base_path="$(pushd .. &> /dev/null; pwd ; popd &> /dev/null)"

    mkdir ${workarea_path}
    pushd ${workarea_path}

    if [ "TRUE" == "${INSTALL_ASSETS}" ] ; then
        echo "Installing assets"
        wget http://gitlab.justworks.today/rogue-forever/assets_basic.zip
        unzip assets_basic.zip
        mv --verbose ./assets ${rogue_forever_base_path}/
    fi

    if [ "TRUE" == "${INSTALL_MAPS}" ] ; then
        echo "Installing maps"
        wget http://gitlab.justworks.today/rogue-forever/maps_basic.zip
        unzip maps_basic.zip
        mv --verbose ./maps ${rogue_forever_base_path}/
    fi

    if [ "TRUE" == "${INSTALL_SDL2_WINDOWS_DLL}" ] ; then
        echo "Installing SDL2 .dll files for Windows build"
        wget https://github.com/libsdl-org/SDL/releases/download/release-2.30.6/SDL2-2.30.6-win32-x64.zip
        wget https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-2.8.2-win32-x64.zip
        unzip SDL2-2.30.6-win32-x64.zip
        unzip SDL2_image-2.8.2-win32-x64.zip
        mv --verbose SDL2.dll ${rogue_forever_base_path}/
        mv --verbose SDL2_image.dll ${rogue_forever_base_path}/
    fi

    popd # ${workarea_path}

    rm --verbose -r ${workarea_path}
}

main "${@}"
