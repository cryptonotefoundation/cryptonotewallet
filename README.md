Dependencies: boost >= 1.55, CMake >= 2.8.6, GCC >=4.7.3, Qt >=5.0

**1. Clone wallet sources**

```
git clone https://github.com/valiant1x/cryptonotewallet/intensecoinwallet.git
```

**2. Update git submodule:**

```
git submodule update --init --recursive
git submodule foreach git pull origin master
```

**3. Build**

```
mkdir build && cd build && cmake .. && make
```

On windows, you may find it helpful to explicitly include Boost and Qt library paths:

```
cmake -DCMAKE_PREFIX_PATH="C:\Qt2\5.9.1\msvc2015" -DBOOST_ROOT="C:\boost_1_64_0_built" -DBOOST_INCLUDEDIR="C:/boost_1_64_0_built/lib32-msvc-14.1" -DBOOST_LIBRARYDIR="C:\boost_1_64_0_built\libs"
 ```
