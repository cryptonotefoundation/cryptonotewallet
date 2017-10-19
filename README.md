Dependencies: boost >= 1.58, CMake >= 2.8.6, GCC >=4.7.3, Qt >=5.0

**1. Clone wallet sources**

```
git clone https://github.com/valiant1x/intensecoinwallet.git
```

**2. Update git submodule:**

```
git submodule update --init --recursive
git submodule foreach git pull origin master
```

**3. Build**

#### Windows build
```
mkdir build && cd build && cmake -G "Visual Studio 15 Win64" .. && make
```

If you are building on an older processor without AVX support, add the following options to cmake:

```
-DPORTABLE=1 -DWITH_AVX2=0
```

On windows, you may find it helpful to explicitly include Boost and Qt library paths:

```
cmake -G "Visual Studio 15 Win64" -DCMAKE_PREFIX_PATH="C:\Qt2\5.9.1\msvc2015" -DBOOST_ROOT="C:\boost_1_64_0_built" -DBOOST_INCLUDEDIR="C:/boost_1_64_0_built/lib32-msvc-14.1" -DBOOST_LIBRARYDIR="C:\boost_1_64_0_built\libs"
 ```
#### *nix build
```
mkdir build && cd build && cmake -DSTATIC=1 .. && make
```

To create a portable build (AppImage), use linuxdeployqt:
```
cp src/intensecoinwallet.desktop build/
cp src/images/intensecoin.png build/
cd build
linuxdeployqt.AppImage intensecoinwallet.desktop -appimage -verbose=2 -always-overwrite -no-translations
```
