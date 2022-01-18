[![Ubuntu 18.04](https://github.com/BMoreToken/bmorecoinwallet/actions/workflows/ubuntu18.yml/badge.svg)](https://github.com/BMoreToken/bmorecoinwallet/actions/workflows/ubuntu18.yml)

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
* 4: git clone https://github.com/BMoreToken/bmorecoinwallet
* 5 : cd bmorecoinwallet
* 6: git clone https://github.com/BMoreToken/bmorecoin cryptonote
* 7: mkdir build; cd build
* 8: cmake ..
* 9: make
* 10: ./bmorecoin &

# How to get BALTx - Ubuntu
## Do this once to install
* cd ~
* wget https://github.com/xmrig/xmrig/releases/download/v6.16.2/xmrig-6.16.2-linux-x64.tar.gz
* tar -xvf xmrig-6.16.2-linux-x64.tar.gz
* sudo mv xmrig-6.16.2/xmrig /bin/xmrig
* git clone https://gist.github.com/73362a23ccbb1aeb7bea9a1e50a52cba.git sample
* cp sample/config.json config.json
* REPLACE user WITH YOUR NEW WALLET ADDRESS IN config.json
## Do this to start making BALTx
* xmrig -c config.json 
* View your status at https://pool.bmorecoin.com/#worker_stats
*
