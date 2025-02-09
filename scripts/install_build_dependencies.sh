#!/bin/bash

# TODO consider using only absolute paths

set -ex

readonly INSTALL_SDL="TRUE"
readonly INSTALL_SDL_IMAGE="TRUE"
readonly INSTALL_GTEST="TRUE"
readonly INSTALL_CMAKE="TRUE"
readonly INSTALL_CPP_COMPILER="TRUE"
readonly INSTALL_CJSON="TRUE"
readonly INSTALL_WGET="TRUE"
readonly INSTALL_GIT="TRUE"
readonly INSTALL_UNZIP="TRUE"
readonly INSTALL_MINGW_64="TRUE"
readonly INSTALL_SDL_MINGW="TRUE"
readonly INSTALL_SDL_IMAGE_MINGW="TRUE"

cd $(dirname $0)

readonly ROGUE_FOREVER_BASE_PATH="$(pushd .. &> /dev/null; pwd ; popd &> /dev/null)"

error_exit() {
    echo "${1}"
    exit 1
}

main() {
    if [ ! -d "deps_install_workarea" ] ; then
        mkdir deps_install_workarea
    fi

    # TODO create workarea directory under /tmp
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

    if [ "TRUE" == "${INSTALL_SDL_MINGW}" ] ; then
        install_sdl_mingw
    fi

    if [ "TRUE" == "${INSTALL_SDL_IMAGE_MINGW}" ] ; then
        install_sdl_image_mingw
    fi

    popd # deps_install_workarea

    if [ "TRUE" == "${INSTALL_CJSON}" ] ; then
        install_cjson
    fi

    pushd ./deps_install_workarea
    if [ "TRUE" == "${INSTALL_GTEST}" ] ; then
        ../install_gtest.sh
    fi
    popd # ./deps_install_workarea

    rm -rf ./deps_install_workarea
}

install_cjson() {
    local -r install_using_cmake="FALSE"
    local -r install_by_copying="TRUE"
    local -r cjson_repo="cJSON_cloned"

    pushd ${ROGUE_FOREVER_BASE_PATH}/external

    if [ ! -d "cJSON" ] ; then
        mkdir -p cJSON/src
        mkdir -p cJSON/include
    fi

    git clone https://github.com/DaveGamble/cJSON.git ${cjson_repo}

    if [ "TRUE" == "${install_using_cmake}" ] ; then
        # This execution path is untested
        pushd ${cjson_repo}
        mkdir build
        cd build
        cmake .. -DBUILD_SHARED_AND_STATIC_LIBS=On
        make
        sudo make install
        popd # ${cjson_repo}
    elif [ "TRUE" == "${install_by_copying}" ] ; then
        cp --verbose ${cjson_repo}/cJSON.c ./cJSON/src/
        cp --verbose ${cjson_repo}/cJSON.h ./cJSON/include/
    else
        rm --verbose -rf ${cjson_repo}
        error_exit "No valid installation method for cJSON selected"
    fi

    rm --verbose -rf ${cjson_repo}

    popd # ${ROGUE_FOREVER_BASE_PATH}/external
}

install_sdl_mingw() {
    local -r sdl2_mingw_zip="SDL2-devel-2.30.6-mingw.zip"
    local -r destination_path_lib="${ROGUE_FOREVER_BASE_PATH}/external/sdl_devel/SDL2-2.30.6/x86_64-w64-mingw32/"
    local -r destination_path_include="${ROGUE_FOREVER_BASE_PATH}/external/sdl_devel/combined/"

    pushd ${ROGUE_FOREVER_BASE_PATH}/scripts/deps_install_workarea/sdl_devel

    if [ ! -d "${destination_path_lib}" ] ; then
        mkdir -p ${destination_path_lib}
    fi

    if [ ! -d "${destination_path_include}" ] ; then
        mkdir -p ${destination_path_include}
    fi

    wget "https://github.com/libsdl-org/SDL/releases/download/release-2.30.6/${sdl2_mingw_zip}"
    unzip "${sdl2_mingw_zip}"
    cp --verbose -r SDL2-2.30.6/x86_64-w64-mingw32/lib/ ${destination_path_lib}
    cp --verbose -r SDL2-2.30.6/x86_64-w64-mingw32/include/ ${destination_path_include}

    popd # ${ROGUE_FOREVER_BASE_PATH}/scripts/deps_install_workarea/sdl_devel
}

install_sdl_image_mingw() {
    local -r sdl2_image_mingw_zip="SDL2_image-devel-2.8.2-mingw.zip"
    local -r destination_path_lib="${ROGUE_FOREVER_BASE_PATH}/external/sdl_devel/SDL2_image-2.8.2/x86_64-w64-mingw32/"
    local -r destination_path_include="${ROGUE_FOREVER_BASE_PATH}/external/sdl_devel/combined/"

    pushd ${ROGUE_FOREVER_BASE_PATH}/scripts/deps_install_workarea/sdl_devel

    if [ ! -d "${destination_path_lib}" ] ; then
        mkdir -p ${destination_path_lib}
    fi

    if [ ! -d "${destination_path_include}" ] ; then
        mkdir -p ${destination_path_include}
    fi

    wget "https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/${sdl2_image_mingw_zip}"
    unzip "${sdl2_image_mingw_zip}"
    cp --verbose -r SDL2_image-2.8.2/x86_64-w64-mingw32/lib/ ${destination_path_lib}
    cp --verbose -r SDL2_image-2.8.2/x86_64-w64-mingw32/include/ ${destination_path_include}

    popd # ${ROGUE_FOREVER_BASE_PATH}/scripts/deps_install_workarea/sdl_devel
}

main
