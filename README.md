Aurumcoin AU
============

Aurumcoin (AU) is a SHA256 proof-of-work cryptocurrency with a fixed supply of
300,000 AU. This repository contains the Aurumcoin full node software, based on
**Bitcoin Core v32.1**. It replaces the legacy Aurumcoin client 0.12.4 (based on
Bitcoin Core 0.12.0), which is no longer maintained.

> **Status: test version ("Version A").** The consensus rules are identical to
> the legacy 0.12 client, so this software can run alongside existing 0.12
> nodes. It has validated the complete existing chain (height 1,348,395) with
> full script verification. It is not yet an official release.

Coin properties
---------------

| Property | Value |
|---|---|
| Algorithm | SHA256 proof-of-work |
| Block reward | 1 AU, halving every 150,000 blocks |
| Maximum supply | 300,000 AU |
| Block time | 60 seconds |
| Difficulty retarget | every 300 blocks, every 2 blocks from height 1,086,560 |
| Coinbase maturity | 2 blocks |
| P2P port / RPC port | 11080 / 21080 |
| Address format | legacy (Base58, same prefixes as Bitcoin) |
| SegWit / Taproot | not active |

Programs and files
------------------

| | |
|---|---|
| Daemon | `aurumcoind` |
| RPC client | `aurumcoin-cli` |
| Tools | `aurumcoin-tx`, `aurumcoin-util`, `aurumcoin-wallet`, `aurumcoin` (wrapper) |
| Data directory | Linux `~/.aurumcoin`, Windows `%APPDATA%\Aurumcoin` (if it exists, as used by 0.12) or `%LOCALAPPDATA%\Aurumcoin`, macOS `~/Library/Application Support/Aurumcoin` |
| Configuration file | `aurumcoin.conf` |

Building (Linux / WSL, Ubuntu 24.04 or newer)
---------------------------------------------

```sh
sudo apt install build-essential cmake pkgconf python3 libevent-dev libboost-dev libsqlite3-dev git
cmake -B build -DENABLE_IPC=OFF -DWITH_ZMQ=OFF -DBUILD_GUI=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

The programs are placed in `build/bin/`. See [doc/build-unix.md](doc/build-unix.md)
for more build options.

Importing the existing chain
----------------------------

A complete chain can be imported from a bootstrap file:

```sh
./build/bin/aurumcoind -daemon -loadblock=/path/to/bootstrap.dat
./build/bin/aurumcoin-cli getblockchaininfo
```

Wallets
-------

The wallet creates legacy addresses only, because SegWit is not active on the
Aurumcoin network. Wallets from the 0.12 client (`wallet.dat`) can be converted
with the `migratewallet` RPC. **Always make a backup of `wallet.dat` first.**

Porting documentation
---------------------

Details about the port from 0.12 to Bitcoin Core v32.1 (in German):

- [doc/aurumcoin/AUSGANGSSITUATION_UND_ZIEL.md](doc/aurumcoin/AUSGANGSSITUATION_UND_ZIEL.md): starting point and goals
- [doc/aurumcoin/PORTING.md](doc/aurumcoin/PORTING.md): consensus rules, pitfalls, checklist and test results

The original Bitcoin Core README is in [doc/README-bitcoin-core.md](doc/README-bitcoin-core.md).

Links
-----

- Source code and issues: https://github.com/aurumcoin-au/Aurumcoin-AU
- Community homepage: https://aurumcoin-au.com

License
-------

Aurumcoin AU is released under the terms of the MIT license. See
[COPYING](COPYING) for more information. It is based on Bitcoin Core,
Copyright (c) 2009-2026 The Bitcoin Core developers.
