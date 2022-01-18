#!/bin/bash

if ! test -f "linuxdeploy-x86_64.AppImage"; then
    wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
fi
if ! test -f "linuxdeploy-plugin-qt-x86_64.AppImage"; then
    wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
fi
chmod u+x ./*.AppImage
cp ../build/release/bmorecoin .
mkdir -p usr/share/icons
mkdir -p usr/lib
cp ../src/images/bmorecoin.png usr/share/icons
./linuxdeploy-x86_64.AppImage --executable ./bmorecoin --desktop-file bmorecoin-desktop.desktop --appdir . --output appimage --plugin qt
