# Windows 11/10: Use Ubuntu for Windows
## Ubuntu for Windows
* https://www.microsoft.com/en-us/p/ubuntu/9nblggh4msv6
* Open in "Store" and install
## WSL
* https://docs.microsoft.com/en-us/windows/wsl/install
* run Powershell "as Admin" to install wsl
<pre>
wsl --install
</pre>
* Reboot PC


# Ubuntu / Linux
* 1: sudo apt update
* 2: sudo apt upgrade
* 3: sudo apt install git gcc make cmake libboost1.67-dev qt5-default build-essential
* 4: git clone https://github.com/BMoreToken/cryptonotewallet bmorecoinwallet
* 5 : cd bmorecoinwallet
* 6: git clone https://github.com/BMoreToken/bmorecoin cryptonote
* 7: mkdir build; cd build
* 8: cmake ..
* 9: make
* 10: ./bmorecoin &

# How to get BALTx ( use ubuntu from above if in windows )
## Do this once to install
* cd ~
* wget https://github.com/xmrig/xmrig/releases/download/v6.16.2/xmrig-6.16.2-linux-x64.tar.gz
* tar -xvf xmrig-6.16.2-linux-x64.tar.gz
* sudo mv xmrig-6.16.2/xmrig /bin/xmrig
* nano config.json
<pre>create config.json file using sample below ( nano/vi/vim/pico ) using your new wallet address</pre>
## Do this to start making BALTx
* xmrig -c config.json 
* View your status at https://pool.bmorecoin.com/#worker_stats
### sample config.json
<pre>
{
    "autosave": true,
    "cpu": true,
    "opencl": false,
    "cuda": false,
    ,
    "cpu": {
        "enabled": true,
        "huge-pages": true,
        "huge-pages-jit": false,
        "hw-aes": null,
        "priority": null,
        "memory-pool": false,
        "yield": true,
        "asm": true,
        "argon2-impl": null,
        "astrobwt-max-size": 550,
        "astrobwt-avx2": false,
        "argon2": [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
        "astrobwt": [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
        "cn": [
            [1, 0],
            [1, 2],
            [1, 4],
            [1, 6],
            [1, 8],
            [1, 10],
            [1, 12],
            [1, 14]
        ],
        "cn-heavy": [
            [1, 0],
            [1, 2],
            [1, 4],
            [1, 6]
        ],
        ,
        "cn-lite": [
            [1, 0],
            [1, 1],
            [1, 2],
            [1, 3],
            [1, 4],
            [1, 5],
            [1, 6],
            [1, 7],
            [1, 8],
            [1, 9],
            [1, 10],
            [1, 11],
            [1, 12],
            [1, 13],
            [1, 14],
            [1, 15]
        ],
        "cn-pico": [
            [2, 0],
            [2, 1],
            [2, 2],
            [2, 3],
            [2, 4],
            [2, 5],
            [2, 6],
            [2, 7],
            [2, 8],
            [2, 9],
            [2, 10],
            [2, 11],
            [2, 12],
            [2, 13],
            [2, 14],
            [2, 15]
        ],
        "cn/0": [0, 1],
        "cn/upx2": [
            [2, 0],
            [2, 1],
            [2, 2],
            [2, 3],
            [2, 4],
            [2, 5],
            [2, 6],
            [2, 7],
            [2, 8],
            [2, 9],
            [2, 10],
            [2, 11],
            [2, 12],
            [2, 13],
            [2, 14],
            [2, 15]
        ],
        "ghostrider": [
            [8, 0],
            [8, 2],
            [8, 4],
            [8, 6],
            [8, 8],
            [8, 10],
            [8, 12],
            [8, 14]
        ],
        "rx": [0, 2, 4, 6, 8, 10, 12, 14],
        "rx/wow": [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
        "cn-lite/0": false,
        "rx/arq": "rx/wow",
        "rx/keva": "rx/wow"
    },  
    "pools": [
        {
            "coin": null,
            "algo": "cn/0",
            "url": "pool.bmorecoin.com:3333",
            "user": "bxdhToH3hn5QUH7tWzitYpREug4Z8f3h5Ewgw5ddzozAANYCtkJmRdXB6Z9wouqkhfe5QTa1nEcvoBasBhgWb41B2Bj4DdSuj",
            "pass": "x",
            "tls": false,
            "keepalive": true,
            "nicehash": false
        }
    ]
}
</pre>


