set -x
echo "CI: macOS 10.12"

if [ "$1" = "prep" ]; then

	installDependencyIfNeeded () {
		brew ls --versions $@ 2>/dev/null
		if [ $? -eq 0 ]; then
			echo "CI: Found $@"
		else
			echo "CI: Missing dependency, installing $@..."
			brew install $@
			if [ $? -eq 0 ]; then
				echo "CI: Installed $@ successfully."
			else
				echo "CI: Failed to install $@."
				exit 1
			fi
		fi
	}

	# tools
	which brew
	if [ $? -eq 0 ]; then
		echo "CI: Brew is available."
	else
		echo "CI: Brew is missing, installing now..."
		/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
	fi

	# dependencies
	installDependencyIfNeeded cmake
	installDependencyIfNeeded boost
	installDependencyIfNeeded openssl
	installDependencyIfNeeded pkgconfig
	# - gui specific
	installDependencyIfNeeded qt5
	# TODO: brew link --force --overwrite qt5
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
RELEASE_NAME="lethean-gui-mac-64bit-$BUILD_VERSION"
cd build/release/bin/
mkdir $RELEASE_NAME
cp -Rp lethean-wallet-gui.app $RELEASE_NAME/
cp ../../../ci/package-artifacts/CHANGELOG.txt $RELEASE_NAME/
cp ../../../ci/package-artifacts/README.txt $RELEASE_NAME/
cp ../../../ci/package-artifacts/*.http $RELEASE_NAME/
tar -cvjf $RELEASE_NAME.tar.bz2 $RELEASE_NAME
shasum -a 256 $RELEASE_NAME.tar.bz2 > $RELEASE_NAME.tar.bz2.sha256.txt
