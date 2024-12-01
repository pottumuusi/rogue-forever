#!/bin/bash

set -e

cd $(dirname $0)

main() {
    local -r workarea_path='/tmp/rogue_forever_runtime_dependencies'
    local -r scripts_path="$(pwd)"
    local -r rogue_forever_base_path="$(pushd .. &> /dev/null; pwd ; popd &> /dev/null)"

    mkdir ${workarea_path}
    pushd ${workarea_path}

    # Install assets
    wget http://gitlab.justworks.today/rogue-forever/assets_basic.zip
    unzip assets_basic.zip
    mv --verbose ./assets ${rogue_forever_base_path}/

    # Install maps
    wget http://gitlab.justworks.today/rogue-forever/maps_basic.zip
    unzip maps_basic.zip
    mv --verbose ./maps ${rogue_forever_base_path}/

    # Install SDL2 .dll files
    wget https://github.com/libsdl-org/SDL/releases/download/release-2.30.6/SDL2-2.30.6-win32-x64.zip
    wget https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-2.8.2-win32-x64.zip
    unzip SDL2-2.30.6-win32-x64.zip
    unzip SDL2_image-2.8.2-win32-x64.zip
    mv --verbose SDL2.dll ${rogue_forever_base_path}/
    mv --verbose SDL2_image.dll ${rogue_forever_base_path}/

    popd # ${workarea_path}

    rm --verbose -r ${workarea_path}
}

main "${@}"
