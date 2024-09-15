#!/bin/bash

set -ex

readonly INSTALL_SDL="TRUE"
readonly INSTALL_SDL_IMAGE="TRUE"
readonly INSTALL_GTEST="TRUE"
readonly INSTALL_CMAKE="TRUE"
readonly INSTALL_CPP_COMPILER="TRUE"
readonly INSTALL_CJSON="FALSE"
readonly INSTALL_NLOHMANNJSON="TRUE"
readonly INSTALL_WGET="TRUE"
readonly INSTALL_GIT="TRUE"
readonly INSTALL_UNZIP="TRUE"
readonly INSTALL_PUGIXML="TRUE"

main() {
    if [ ! -d deps_install_workarea ] ; then
        mkdir deps_install_workarea
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
    if [ ! -d external/include ] ; then
        mkdir external/include
    fi
    if [ ! -d external/src ] ; then
        mkdir external/src
    fi
    popd # ../

    if [ "TRUE" == "${INSTALL_CJSON}" ] ; then
        install_cjson
    fi

    if [ "TRUE" == "${INSTALL_NLOHMANNJSON}" ] ; then
        install_nlohmannjson
    fi

    if [ "TRUE" == "${INSTALL_PUGIXML}" ] ; then
        install_pugixml
    fi

    pushd ./deps_install_workarea
    if [ "TRUE" == "${INSTALL_GTEST}" ] ; then
        ../install_gtest.sh
    fi
    popd # ./deps_install_workarea

    rm -rf ./deps_install_workarea
}

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

install_pugixml() {
    pushd ../external/

    if [ ! -d include/pugixml ] ; then
        mkdir include/pugixml
    fi

    if [ ! -d src/pugixml ] ; then
        mkdir src/pugixml
    fi

    wget https://github.com/zeux/pugixml/releases/download/v1.14/pugixml-1.14.zip
    unzip pugixml-1.14.zip

    cp pugixml-1.14/src/pugixml.hpp ./include/pugixml/
    cp pugixml-1.14/src/pugiconfig.hpp ./include/pugixml/
    cp pugixml-1.14/src/pugixml.cpp ./src/pugixml/

    rm -r pugixml-1.14
    rm pugixml-1.14.zip

    popd # ../external/
}

cd $(dirname $0)

main
