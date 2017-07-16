Dependencies: boost >= 1.55, CMake >= 2.8.6, GCC >=4.7.3, Qt >=5.0

**1. Clone wallet sources**

```
git clone https://github.com/valiant1x/cryptonotewallet/intensecoinwallet.git
```

**2. Create git submodule:**

```
git submodule add https://github.com/valiant1x/intensecoin.git cryptonote
```

**3. Build**

```
mkdir build && cd build && cmake .. && make
```
