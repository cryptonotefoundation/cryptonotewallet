#!/bin/bash

BUILD_TYPE=$1
source ./utils.sh
platform=$(get_platform)
# default build type
if [ -z $BUILD_TYPE ]; then
    BUILD_TYPE=release
fi

if [ "$BUILD_TYPE" == "release" ]; then
    echo "Building release"
    CONFIG="CONFIG+=release";
    BIN_PATH=release/bin
elif [ "$BUILD_TYPE" == "release-static" ]; then
    echo "Building release-static"
    if [ "$platform" != "darwin" ]; then
	    CONFIG="CONFIG+=release static";
    else
        # OS X: build static libwallet but dynamic Qt.
        echo "OS X: Building Qt project without static flag"
        CONFIG="CONFIG+=release";
    fi
    BIN_PATH=release/bin
elif [ "$BUILD_TYPE" == "release-android" ]; then
    echo "Building release for ANDROID"
    CONFIG="CONFIG+=release static WITH_SCANNER";
    ANDROID=true
    BIN_PATH=release/bin
elif [ "$BUILD_TYPE" == "debug-android" ]; then
    echo "Building debug for ANDROID : ultra INSECURE !!"
    CONFIG="CONFIG+=debug qml_debug WITH_SCANNER";
    ANDROID=true
    BIN_PATH=debug/bin
elif [ "$BUILD_TYPE" == "debug" ]; then
    echo "Building debug"
	CONFIG="CONFIG+=debug"
    BIN_PATH=debug/bin
else
    echo "Valid build types are release, release-static, release-android, debug-android and debug"
    exit 1;
fi


source ./utils.sh
pushd $(pwd)
ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
MONERO_DIR=lethean
MONEROD_EXEC=letheand

MAKE='make'
if [[ $platform == *bsd* ]]; then
    MAKE='gmake'
fi


# build libwallet
./get_libwallet_api.sh $BUILD_TYPE

# build zxcvbn
$MAKE -C src/zxcvbn-c || exit

if [ ! -d build ]; then mkdir build; fi


# Platform indepenent settings
if [ "$ANDROID" != true ] && ([ "$platform" == "linux32" ] || [ "$platform" == "linux64" ]); then
    distro=$(lsb_release -is)
    if [ "$distro" == "Ubuntu" ]; then
        CONFIG="$CONFIG libunwind_off"
    fi
fi

if [ "$platform" == "darwin" ]; then
    BIN_PATH=$BIN_PATH/lethean-wallet-gui.app/Contents/MacOS/
elif [ "$platform" == "mingw64" ] || [ "$platform" == "mingw32" ]; then
    MONEROD_EXEC=letheand.exe
fi

# force version update
get_tag
echo "var GUI_VERSION = \"$TAGNAME\"" > version.js
pushd "$MONERO_DIR"
get_tag
popd
echo "var GUI_MONERO_VERSION = \"$TAGNAME\"" >> version.js

cd build
qmake ../lethean-wallet-gui.pro "$CONFIG" || exit
$MAKE || exit

# Copy monerod to bin folder
#if [ "$platform" != "mingw32" ] && [ "$ANDROID" != true ]; then
if [ "$ANDROID" != true ]; then
cp ../$MONERO_DIR/bin/$MONEROD_EXEC $BIN_PATH
fi

# make deploy
popd
