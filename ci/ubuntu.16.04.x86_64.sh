set -x
echo "CI: Ubuntu 16.04 x86_64"

if [ "$1" = "prep" ]; then

	installDependencyIfNeeded () {
		if [ $(dpkg-query -W -f='${Status}' $@ 2>/dev/null | grep -c "ok installed") -ne 0 ]; then
			echo "CI: Found $@"
		else
			echo "CI: Missing dependency, installing $@..."
			sudo apt-get install $@ -y
			if [ $? -eq 0 ]; then
				echo "CI: Installed $@ successfully."
			else
				echo "CI: Failed to install $@."
				exit 1
			fi
		fi
	}

	# dependencies
	installDependencyIfNeeded build-essential
	installDependencyIfNeeded libssl-dev
	installDependencyIfNeeded libboost-all-dev
	installDependencyIfNeeded pkg-config
	installDependencyIfNeeded cmake
	# - gui specific
	installDependencyIfNeeded qtbase5-dev
	installDependencyIfNeeded qt5-default
	installDependencyIfNeeded qtdeclarative5-dev
	installDependencyIfNeeded qml-module-qtquick-controls
	installDependencyIfNeeded qml-module-qtquick-xmllistmodel
	installDependencyIfNeeded qttools5-dev-tools
	installDependencyIfNeeded qml-module-qtquick-dialogs
	installDependencyIfNeeded qml-module-qt-labs-settings
	installDependencyIfNeeded qml-module-qtgraphicaleffects
	# - temporary link workaround
	installDependencyIfNeeded libunbound-dev

	exit 0

fi

echo "CI: Building static release..."
./build.sh release-static
if [ $? -ne 0 ]; then
	echo "CI: Build failed with error code: $?"
	exit 1
fi

echo "CI: Building deployable binary..."
cd build
make deploy
if [ $? -ne 0 ]; then
	echo "CI: Build failed with error code: $?"
	exit 1
fi
cd ..

echo "CI: Creating release archive..."
RELEASE_NAME="lethean-gui-linux-64bit-$BUILD_VERSION"
cd build/release/bin/
mkdir $RELEASE_NAME
cp letheand $RELEASE_NAME/
cp lethean-wallet-gui $RELEASE_NAME/
cp -R qml $RELEASE_NAME/
cp -R libs $RELEASE_NAME/
cp -R plugins $RELEASE_NAME/
cp -R translations $RELEASE_NAME/
cp start-gui.sh $RELEASE_NAME/
cp ../../../ci/package-artifacts/CHANGELOG.txt $RELEASE_NAME/
cp ../../../ci/package-artifacts/README.txt $RELEASE_NAME/
tar -cvjf $RELEASE_NAME.tar.bz2 $RELEASE_NAME
sha256sum $RELEASE_NAME.tar.bz2 > $RELEASE_NAME.tar.bz2.sha256.txt
