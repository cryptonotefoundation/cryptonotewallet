================================================
Welcome to Intense Coin!
================================================

We are building a blockchain-backed decentralized VPN to make the internet a safer place.
This package contains the command line utilities needed to join the Intense Coin network.



------------------------------------------------
Getting Started - Synchronization
------------------------------------------------

Before you can participate in the network, your local copy of the blockchain data must be
synchronized to match the rest of the network.

Windows:
 - double-click the daemon: intensecoind.exe
 - blockchain data is stored in: C:\Users\[USERNAME]\AppData\Roaming\intensecoin

Linux or macOS:
 - open the terminal
 - change into the CLI archive directory: $ cd intensecoin-cli-archive-directory/
 - run the daemon: $ ./intensecoind
 - blockchain data is stored in: ~/.intensecoin


Synchronization can take several hours the first time. To check on progress, simply type
"status" and press enter. This is how you execute commands on the daemon. After it has
completed, your status message will look like this:

     Height: some-number/the-same-number (100.0%) on mainnet

It indicates that you have a copy of the same number of blocks that the network has
created. This is also called the "height" of the network.

Now you can run intense-wallet-cli to set up your wallet.



------------------------------------------------
Basic Commands: intensecoind
------------------------------------------------

help
 - shows all possible commands

status
 - returns a summary of your synchronization status, network hashrate and connections


