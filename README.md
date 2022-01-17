<pre>
Windows: Use Ubuntu
https://www.microsoft.com/en-us/p/ubuntu/9nblggh4msv6?activetab=pivot:overviewtab
https://docs.microsoft.com/en-us/windows/wsl/install

Powershell as Admin
wsl --install
Reboot

Linux / Ubuntu
Prerequisites
Dependencies: GCC 4.7.3 or later, CMake 2.8.6 or later, Boost 1.55 or later, and Qt 5.9 or later. You may download them from:

https://gcc.gnu.org/
https://www.cmake.org/
https://www.boost.org/
https://www.qt.io
On Ubuntu it is possible to install them using apt:
sudo apt update
sudo apt upgrade
sudo apt install git gcc make cmake libboost-all-dev qt5-default
Building
To acquire the source via git and build the release version, run the following commands:

git clone https://github.com/BMoreToken/cryptonotewallet bmorecoinwallet
cd bmorecoinwallet
git clone https://github.com/BMoreToken/bmorecoin cryptonote
mkdir build
cd build
cmake ..
make
If the build is successful the binary can me run by ./bmorecoin

Windows 10
Prerequisites
Install Visual Studio 2017 Community Edition
When installing Visual Studio, you need to install Desktop development with C++ and the VC++ v140 toolchain components. The option to install the v140 toolchain can be found by expanding the "Desktop development with C++" node on the right. You will need this for the project to build correctly.
Install CMake
Install Boost 1.67.0, ensuring you download the installer for MSVC 14.1.
Install Qt 5.11.0
Building
From the start menu, open 'x64 Native Tools Command Prompt for vs2022'
git clone https://github.com/BMoreToken/cryptonotewallet bmorecoinwallet
Edit the CMakeLists.txt file and set the path to QT cmake folder. For example: set(CMAKE_PREFIX_PATH "C:\Qt\5.15.2\msvc2019_64\lib\cmake\".
cd bmorecoinwallet
git clone https://github.com/BMoreToken/bmorecoin cryptonote
mkdir build
cd build
cmake -G "Visual Studio 17 2022 Win64" -DBOOST_LIBRARYDIR:PATH=c:/local/boost_1_67_0 .. (Or your boost installed dir.)
msbuild bmorecoin.sln /p:Configuration=Release
If the build is successful the binaries will be in the Release folder.

</pre>





**1. Clone wallet sources**

```
git clone https://github.com/cryptonotefoundation/cryptonotewallet.git
```

**2. Modify `CryptoNoteWallet.cmake`**
 
```
set(CN_PROJECT_NAME "furiouscoin")
set(CN_CURRENCY_DISPLAY_NAME "FuriousCoin")
set(CN_CURRENCY_TICKER "XFC")
```

**3. Set symbolic link to coin sources at the same level as `src`. For example:**

```
ln -s ../cryptonote cryptonote
```

Alternative way is to create git submodule:

```
git submodule add https://github.com/cryptonotefoundation/cryptonote.git cryptonote
```

Replace URL with git remote repository of your coin.

**4. Build**

```
mkdir build && cd build && cmake .. && make
```
