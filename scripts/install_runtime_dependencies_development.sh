#!/bin/bash

set -e

readonly INSTALL_ASSETS="TRUE"
readonly INSTALL_MAPS="TRUE"

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

    popd # ${workarea_path}

    rm --verbose -r ${workarea_path}
}

main "${@}"
