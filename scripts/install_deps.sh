#!/bin/bash

# TODO consider using only absolute paths

set -ex

# readonly INSTALL_SDL="TRUE"
# readonly INSTALL_SDL_IMAGE="TRUE"
# readonly INSTALL_GTEST="TRUE"
# readonly INSTALL_CMAKE="TRUE"
# readonly INSTALL_CPP_COMPILER="TRUE"
# readonly INSTALL_CJSON="FALSE"
# readonly INSTALL_NLOHMANNJSON="TRUE"
# readonly INSTALL_WGET="TRUE"
# readonly INSTALL_GIT="TRUE"
# readonly INSTALL_UNZIP="TRUE"
# readonly INSTALL_MINGW_64="TRUE"
readonly INSTALL_SDL_MINGW_LIBRARY="TRUE"
readonly INSTALL_SDL_IMAGE_MINGW_LIBRARY="TRUE"

cd $(dirname $0)

readonly ROGUE_FOREVER_BASE_PATH="$(pushd .. &> /dev/null; pwd ; popd &> /dev/null)"

main() {
    if [ ! -d "deps_install_workarea" ] ; then
        mkdir deps_install_workarea
    fi

    if [ ! -d "deps_install_workarea/sdl_devel" ] ; then
        mkdir deps_install_workarea/sdl_devel
    fi

    if [ ! -d "${ROGUE_FOREVER_BASE_PATH}/external" ] ; then
        mkdir ${ROGUE_FOREVER_BASE_PATH}/external
    fi

    if [ ! -d "${ROGUE_FOREVER_BASE_PATH}/external/sdl_devel" ] ; then
        mkdir ${ROGUE_FOREVER_BASE_PATH}/external/sdl_devel
    fi

    pushd deps_install_workarea

    if [ "TRUE" == "${INSTALL_CMAKE}" ] ; then
        sudo apt update
        sudo apt install -y cmake
    fi

    if [ "TRUE" == "${INSTALL_CPP_COMPILER}" ] ; then
        sudo apt update
        sudo apt install -y g++
    fi

    if [ "TRUE" == "${INSTALL_WGET}" ] ; then
        sudo apt update
        sudo apt install -y wget
    fi

    if [ "TRUE" == "${INSTALL_GIT}" ] ; then
        sudo apt update
        sudo apt install -y git
    fi

    if [ "TRUE" == "${INSTALL_UNZIP}" ] ; then
        sudo apt update
        sudo apt install -y unzip
    fi

    if [ "TRUE" == "${INSTALL_MINGW_64}" ] ; then
        sudo apt update
        sudo apt install -y mingw-w64
    fi

    if [ "TRUE" == "$INSTALL_SDL" ] ; then
        sudo apt install -y libx11-dev libxext-dev

        wget https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.24.0.zip
        unzip release-2.24.0.zip
        pushd SDL-release-2.24.0
        ./configure
        make
        sudo make install
        popd
    fi

    if [ "TRUE" == "$INSTALL_SDL_IMAGE" ] ; then
        sudo apt install -y libx11-dev libxext-dev

        wget https://github.com/libsdl-org/SDL_image/archive/refs/tags/release-2.6.2.zip
        unzip release-2.6.2.zip
        pushd SDL_image-release-2.6.2
        ./configure
        make
        sudo make install
        popd
    fi

    if [ "TRUE" == "${INSTALL_SDL_MINGW_LIBRARY}" ] ; then
        install_sdl_mingw_library
    fi

    if [ "TRUE" == "${INSTALL_SDL_IMAGE_MINGW_LIBRARY}" ] ; then
        install_sdl_image_mingw_library
    fi

    popd # deps_install_workarea

    if [ "TRUE" == "${INSTALL_CJSON}" ] ; then
        install_cjson
    fi

    if [ "TRUE" == "${INSTALL_NLOHMANNJSON}" ] ; then
        install_nlohmannjson
    fi

    pushd ./deps_install_workarea
    if [ "TRUE" == "${INSTALL_GTEST}" ] ; then
        ../install_gtest.sh
    fi
    popd # ./deps_install_workarea

    sudo rm -rf ./deps_install_workarea # TODO do not sudo
}

# TODO install by just copying the header and C file, instead of using cmake.
install_cjson() {
    pushd ../external

    git clone https://github.com/DaveGamble/cJSON.git
    pushd cJSON

    mkdir build
    cd build
    cmake .. -DBUILD_SHARED_AND_STATIC_LIBS=On
    make
    sudo make install

    popd # cJSON

    popd # ../external
}

install_nlohmannjson() {
    pushd ../external

    if [ ! -d nlohmann ] ; then
        mkdir nlohmann
    fi

    pushd nlohmann

    wget https://raw.githubusercontent.com/nlohmann/json/v3.11.3/single_include/nlohmann/json.hpp

    popd # nlohmann
    popd # ../external
}

install_sdl_mingw_library() {
    local -r external_destination_path="${ROGUE_FOREVER_BASE_PATH}/external/sdl_devel/SDL2-2.30.6/x86_64-w64-mingw32/"

    pushd ${ROGUE_FOREVER_BASE_PATH}/scripts/deps_install_workarea/sdl_devel

    if [ ! -d "${external_destination_path}" ] ; then
        mkdir -p ${external_destination_path}
    fi

    wget https://github.com/libsdl-org/SDL/releases/download/release-2.30.6/SDL2-devel-2.30.6-mingw.zip
    unzip SDL2-devel-2.30.6-mingw.zip
    cp --verbose -r SDL2-2.30.6/x86_64-w64-mingw32/lib/ ${external_destination_path}

    popd # ${ROGUE_FOREVER_BASE_PATH}/scripts/deps_install_workarea/sdl_devel
}

install_sdl_image_mingw_library() {
    echo "install_sdl_image_mingw_library Not yet implemented"

    # wget https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-devel-2.8.2-mingw.zip
}

cd $(dirname $0)

main
