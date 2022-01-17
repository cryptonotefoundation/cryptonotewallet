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
* nano config.json
<pre>create config.json file using sample below ( nano/vi/vim/pico ) using your new wallet address</pre>
## Do this to start making BALTx
* ./xmrig-6.16.2/xmrig -c config.json 
* View your status at https://pool.bmorecoin.com/#worker_stats
### sample config.json
<pre>
{
    "autosave": true,
    "cpu": true,
    "opencl": false,
    "cuda": false,
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


