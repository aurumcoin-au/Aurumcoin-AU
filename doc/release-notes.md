Aurumcoin AU v32.1.0 Release Notes ("Version A")
================================================

Aurumcoin AU v32.1.0 is the first Aurumcoin client based on **Bitcoin Core v32.1**.
It replaces the legacy Aurumcoin client 0.12.4 (based on Bitcoin Core 0.12.0),
which is no longer maintained.

Source code and issue tracker: <https://github.com/aurumcoin-au/Aurumcoin-AU>
Website: <https://aurumcoin-au.com>

For the changes inherited from Bitcoin Core, see
[release-notes-bitcoin-core-32.1.md](release-notes/release-notes-bitcoin-core-32.1.md).


Status and compatibility
------------------------

This is **"Version A"**: the consensus rules are identical to Aurumcoin 0.12.
v32.1 nodes and 0.12 nodes run side by side on the same network and accept each
other's blocks and transactions. This was verified on the live network:

- the complete existing chain (height 1,348,395) validates with full script checks,
- blocks mined with v32.1 (from height 1,348,396) are accepted by 0.12 nodes and vice versa,
- transactions created by v32.1 wallets are relayed and mined by 0.12 nodes
  (when they pay at least 1000 sat/kvB, see "Fees").

A later "Version B" will separate the network from the legacy 0.12 nodes
(own message start / ports). It will be announced in advance.

Blocks mined by v32.1 have version `0x20000000`. When more than 50 of the last
100 blocks have a version above 4, Aurumcoin 0.12 nodes show
"Warning: This version is obsolete; upgrade required!" - this is intended as an
upgrade hint for operators of old nodes.


Network
-------

| | mainnet | testnet3 | testnet4 | signet | regtest |
|---|---|---|---|---|---|
| P2P port | 11080 | 11180 | 11280 | 11380 | 11480 |
| RPC port | 21080 | 21180 | 21280 | 21380 | 21480 |
| Message start | `f9beb4d9` (as 0.12) | `AUT3` | `AUT4` | Bitcoin signet XOR `AUSG` | `AURG` |

- **mainnet** keeps the legacy message start, which is identical to Bitcoin's.
  Bitcoin nodes may therefore connect; they cannot follow the Aurumcoin chain.
- The **test networks** have their own message start and ports and no seeds, so
  they never connect to Bitcoin test networks. There is currently no public
  Aurumcoin test network.
- DNS seeds: `seed.cointech.net`, `seed.aurumcoin-au.com`.
  Fixed seeds: `93.186.200.229`, `5.199.133.55`.
- New nodes are protected by `nMinimumChainWork` and `assumevalid` set to block
  1,348,395 (`00000000a1658908e661068c0e3b0cffdf54e396fdac2de78166ea8267e29378`).
- `-maxtipage` defaults to 365 days (Bitcoin Core: 24 hours). Days can pass
  between Aurumcoin blocks; with 24 hours a synced node would stay in "initial
  block download" mode.


Wallet
------

- **Only legacy addresses (starting with `1`) are supported.** SegWit and Taproot
  are not active on the Aurumcoin chain; coins sent to such outputs could be
  spent by anyone. Therefore:
  - new addresses and change are always legacy (P2PKH),
  - the GUI offers no other address type,
  - Bech32/Bech32m addresses are rejected everywhere (sending, validation, raw
    transactions), including pasted Bitcoin `bc1...` addresses.
  Sending to P2SH addresses (starting with `3`) remains possible.
- Transactions are created with version 1 (Aurumcoin 0.12 only relays version 1)
  and blocks contain no witness commitment.
- New wallets are descriptor wallets (SQLite). Legacy wallets (Berkeley DB
  `wallet.dat` from 0.12) can no longer be used directly and must be migrated.


Migrating a wallet from Aurumcoin 0.12
--------------------------------------

**Make a backup of your `wallet.dat` first** and keep it until the migration has
been verified.

1. Stop the 0.12 client.
2. Copy `wallet.dat` into a new folder in the wallet directory of the v32 data
   directory, e.g. `~/.aurumcoin/wallets/mywallet/wallet.dat`
   (Windows: `%APPDATA%\Aurumcoin\wallets\mywallet\wallet.dat`).
3. Start `aurumcoind` (or `aurumcoin-qt`) and wait until the chain is synced.
4. Run `aurumcoin-cli migratewallet mywallet`
   (GUI: File -> Migrate Wallet). If the wallet is encrypted, the passphrase is
   needed.
5. Check the balance and addresses: `aurumcoin-cli -rpcwallet=mywallet getbalances`.

The migration rescans the whole chain, which can take 10 minutes or more. A copy
of the old wallet is kept as `<name>_<time>.legacy.bak`.
Tested with a real Aurumcoin 0.12 wallet (104 keys, 1326 unspent outputs): all
keys and labels were migrated and the balance matched the UTXO set exactly.


Fees
----

- Minimum relay fee, incremental relay fee, wallet minimum fee and fallback fee:
  **100 sat/kvB** (0.000001 AU/kvB). A typical transaction costs about 25 sat.
- The fallback fee is enabled, because fee estimation rarely has enough data on
  the Aurumcoin chain.
- **Note for the transition period:** Aurumcoin 0.12 nodes only relay
  transactions that pay at least **1000 sat/kvB**. Low-fee transactions reach
  v32.1 nodes and miners only. To be relayed by 0.12 nodes too, pay at least
  1 sat/vB: per transaction with `fee_rate=1` (e.g.
  `aurumcoin-cli -named sendtoaddress address=1... amount=1 fee_rate=1`), in the
  GUI with a custom fee of 1000 sat/kvB, or for all transactions with
  `-mintxfee=0.00001 -fallbackfee=0.00001`.


Programs and files
------------------

| | Aurumcoin 0.12 | Aurumcoin v32.1 |
|---|---|---|
| Daemon / CLI / GUI | `aurumcoind`, `aurumcoin-cli`, `aurumcoin-qt` | same names |
| Tools | `aurumcoin-tx` | `aurumcoin-tx`, `aurumcoin-util`, `aurumcoin-wallet`, `aurumcoin` |
| Data directory | `~/.aurumcoin`, `%APPDATA%\Aurumcoin` | same (Windows: `%LOCALAPPDATA%\Aurumcoin` if no legacy directory exists) |
| Configuration | `aurumcoin.conf` | `aurumcoin.conf` |
| Sample init scripts | - | `contrib/init/aurumcoind.*` (see `doc/init.md`) |

The block and chainstate databases of 0.12 cannot be reused. A v32.1 node syncs
the chain from the network or imports it from a bootstrap file
(`-loadblock=bootstrap.dat`, see README).


Known issues and limitations
----------------------------

- The unit and functional test suites inherited from Bitcoin Core assume
  Bitcoin's consensus rules and SegWit; parts of them fail by design. See
  [doc/aurumcoin/TESTS.md](aurumcoin/TESTS.md).
- GitHub Actions CI is disabled (`.github/workflows-disabled.md`).
- macOS binaries are not provided yet.
- No public Aurumcoin test network exists yet.


Credits
-------

Thanks to the Bitcoin Core developers for the code base, and to everyone who
kept the Aurumcoin network running.
