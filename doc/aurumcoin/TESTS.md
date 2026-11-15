Aurumcoin AU v32 - test status
==============================

The test suites are inherited from Bitcoin Core v32.1. Aurumcoin changes the
consensus parameters (subsidy, halving, difficulty retarget, maximum supply,
coinbase maturity), keeps SegWit/Taproot inactive and rejects Bech32 addresses.
Tests that hard-code Bitcoin values therefore fail by design. They are kept in
the source tree but disabled, so that `ctest` reflects the state of the
Aurumcoin code.

How to run
----------

```sh
cmake --build build -j$(nproc)
ctest --test-dir build -j$(nproc)          # disabled suites are reported as "Disabled"
build/bin/test_aurumcoin --run_test=<suite> # run a single (also a disabled) suite
```

Unit tests (`test_aurumcoin`)
-----------------------------

Status 2026-11-15: 153 suites, **136 pass**, 17 disabled.

The disabled suites are listed in `AURUMCOIN_DISABLED_UNIT_TESTS` in
`src/test/CMakeLists.txt`:

| Suite | Reason |
|---|---|
| `amount_tests` | expects the unit string `BTC/kvB`, Aurumcoin prints `AU/kvB` |
| `coinselector_tests` | creates Bech32 (SegWit) wallet outputs, which Aurumcoin refuses |
| `denialofservice_tests` | stale-tip / eviction logic depends on Bitcoin timing and `-maxtipage` (Aurumcoin: 365 days) |
| `descriptor_tests` | expected Bech32 strings use Bitcoin's `bc` HRP (Aurumcoin: `au`) |
| `group_outputs_tests` | requests Bech32 destinations from the wallet |
| `key_io_tests` | Bitcoin address test vectors incl. Bech32, rejected by Aurumcoin |
| `miner_tests` | hard-coded block nonces / templates for Bitcoin's subsidy and rules |
| `net_peer_connection_tests` | expects Bitcoin's default port 8333 in log messages (Aurumcoin: 11080) |
| `peerman_tests` | expects `NODE_WITNESS` among the desirable service flags |
| `pow_tests` | Bitcoin's difficulty retarget (Aurumcoin: two-phase retarget) |
| `rpc_tests` | amount parsing up to 21,000,000 (Aurumcoin `MAX_MONEY`: 300,000) |
| `script_standard_tests` | expected Bech32/Taproot address strings with `bc` HRP |
| `spend_tests` | fee expectations based on Bitcoin's 1000 sat/kvB wallet minimum (Aurumcoin: 100) |
| `transaction_tests` | SegWit/Taproot test vectors that require the witness flags |
| `util_tests` | money parsing up to 21,000,000 (`MAX_MONEY` 300,000) |
| `validation_tests` | Bitcoin block subsidy schedule (Aurumcoin: 1 AU, halving every 150,000 blocks) |
| `wallet_tests` | coinbase maturity 100 assumed (Aurumcoin: 2) and Bech32 outputs |

Fix applied to the test framework: `TestChain100Setup` mined `COINBASE_MATURITY`
blocks, which is only 2 on Aurumcoin; it now always mines 100 blocks. Before this
fix the shared fixture aborted and 12 further suites hung.

Functional tests (`test/functional`)
------------------------------------

The functional (Python) tests use regtest. The test framework was updated for the
Aurumcoin regtest message start (`AURG`), but the suite is **not run** for
Aurumcoin (status 2026-11-15):

- `test_runner.py` aborts while building the shared test chain cache: the
  framework mines to a Taproot address (`create_deterministic_address_bcrt1_p2tr_op_true`)
  and Aurumcoin rejects all Bech32/Bech32m addresses ("Invalid address").
- The `MiniWallet` helper used by most tests is built on such Taproot outputs,
  and many tests create SegWit wallets or rely on Bitcoin's subsidy and fees.

Adapting the functional suite (legacy-only MiniWallet, Aurumcoin amounts) is a
separate project. Until then the release test procedure below is used.

Release test procedure used instead
-----------------------------------

The Aurumcoin-specific behaviour was verified on the real network and offline:

1. Full validation of the existing chain (height 1,348,395) with script checks.
2. Isolated network test with an Aurumcoin 0.12 node in both directions
   (blocks, difficulty retargets, transactions).
3. Mining on mainnet: blocks 1,348,396 - 1,348,411 accepted by all 0.12 nodes,
   including a transaction created by a 0.12 wallet.
4. Wallet migration of a real Aurumcoin 0.12 wallet (104 keys, 1326 UTXOs);
   balance identical to the UTXO set.
5. Regtest checks: only legacy addresses are created, Bech32/Bech32m addresses
   are rejected for sending and validation, change is always legacy.
