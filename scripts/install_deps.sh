#!/bin/bash

set -ex

readonly INSTALL_SDL="TRUE"
readonly INSTALL_SDL_IMAGE="TRUE"
readonly INSTALL_GTEST="TRUE"
readonly INSTALL_CMAKE="TRUE"
readonly INSTALL_CPP_COMPILER="TRUE"
readonly INSTALL_CJSON="TRUE"
readonly INSTALL_NLOHMANNJSON="TRUE" # TODO set to false
readonly INSTALL_WGET="TRUE"
readonly INSTALL_GIT="TRUE"
readonly INSTALL_UNZIP="TRUE"

cd $(dirname $0)

readonly ROGUE_FOREVER_BASE_PATH="$(pushd .. &> /dev/null; pwd ; popd &> /dev/null)"

main() {
    if [ ! -d deps_install_workarea ] ; then
        mkdir deps_install_workarea
    fi

    # TODO create workarea directory under /tmp
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

    popd # deps_install_workarea

    pushd ../
    if [ ! -d external ] ; then
        mkdir external
    fi
    popd # ../

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

    rm -rf ./deps_install_workarea
}

error_exit() {
    echo "${1}"
    exit 1
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

main
