# Aurumcoin → Bitcoin Core v32.1 – Portierungsplan

Stand: 2026-11-13. Arbeitsverzeichnis im WSL: `~/aurumcoin/core` (Branch `aurumcoin-port`),
Referenz: `~/aurumcoin/legacy-0.12` (Aurumcoin AU 0.12.4), Vergleichsbasis `~/aurumcoin/up012` (Bitcoin 0.12.0, umbenannt).

## Basis
Aurumcoin AU 0.12.4 = **Bitcoin Core 0.12.0** + wenige Änderungen (ohne BIP9/CSV aus 0.12.1).

## Aurumcoin-Konsensregeln (aus dem Diff gegen 0.12.0)

| Regel | Bitcoin | Aurumcoin | v32-Stelle |
|---|---|---|---|
| Blockbelohnung | 50 BTC | **1 AU** | `GetBlockSubsidy` (validation.cpp) |
| Halving | 210000 | **150000** | chainparams |
| MAX_MONEY | 21 Mio | **300000** | consensus/amount.h |
| COINBASE_MATURITY | 100 | **2** | consensus/consensus.h |
| Blockabstand | 600 s | **60 s** | chainparams |
| Retarget | 2 Wochen | Phase 1: Timespan 18000 s (Intervall 300); ab Höhe **1086560**: Timespan 120 s (Intervall 2) | pow.cpp + consensus/params.h |
| powLimit | 00000000ff… | gleich | – |
| Genesis | – | Zeit 1411421930, Nonce 2552177790, Bits 0x1d00ffff, "Mtgox is down", Hash 0000000040a867…251955 | chainparams |
| Magic Bytes | f9beb4d9 | **gleich wie Bitcoin** | chainparams |
| Port P2P / RPC | 8333/8332 | **11080 / 21080** (Testnet/Regtest 5744/5745) | chainparams, chainparamsbase |
| Adress-Präfixe | 0/5/128, xpub/xprv | **gleich wie Bitcoin** | chainparams |
| Einheit | BTC | **AU** | CURRENCY_UNIT |
| DNS-Seeds | – | seed.cointech.net, seed.aurumcoin-au.com | chainparams |

### Soft-Forks, die auf der Aurumcoin-Chain NIE aktiv wurden
- **BIP34/BIP66/BIP65**: Supermajority-Schwellen 4750/5750 bei Fenster 1000 → unerreichbar.
  → In v32: `BIP34Height`, `BIP65Height`, `BIP66Height` = nie (INT_MAX).
- **CSV (BIP68/112/113)**: in 0.12.0 nicht vorhanden → `CSVHeight` = nie.
- **SegWit/Taproot**: User-Entscheidung: abgeschaltet → `SegwitHeight` = nie.

### Fallstricke
1. **pow.cpp nutzt `chainActive.Height()`** statt der Höhe des geprüften Blocks.
   Intervall-Wahl mit `>=`, Timespan-Wahl mit `>` → an der Grenze 1086560 gemischt.
   Port: deterministisch über `pindexLast->nHeight` nachbilden (entspricht synchronem Node an der Spitze),
   gegen echte Chain verifizieren.
2. **v32 setzt P2SH+WITNESS+TAPROOT-Script-Flags ab Genesis** (`GetBlockScriptFlags`).
   Legacy behandelt Witness-Programme als anyone-can-spend → mögliche Chain-Spaltung.
   Port: WITNESS/TAPROOT nur setzen, wenn SegWit aktiv.
3. **BIP30**: Legacy überspringt BIP30 (BIP34Height=1 + Hash von Block 1 passt), erzwingt aber BIP34 nie.
   v32 koppelt beides an `BIP34Height`. Port: eigener Parameter für den BIP30-Skip;
   `BIP34_IMPLIES_BIP30_LIMIT` (1983702) für Aurumcoin entfernen.
4. **NODE_WITNESS**: v32 verlangt das Dienstbit von ausgehenden Peers (`GetDesirableServiceFlags`)
   → alle 0.12-Nodes würden getrennt. Port: NODE_WITNESS nicht verlangen, solange SegWit aus.
5. **Wallet**: Standard-Adresstyp in v32 ist bech32 → bei SegWit aus gefährlich/unbrauchbar.
   Port: Standard `legacy`; bech32/bech32m-Adressen ablehnen. Eigener `bech32_hrp` trotzdem setzen (z. B. "au").
6. Checkpoints gibt es in v32 nicht mehr → `nMinimumChainWork` / `defaultAssumeValid` nach erstem Sync setzen.
7. `m_assumeutxo_data`, `chainTxData`, `m_headers_sync_params`, Fixed Seeds: Bitcoin-Werte entfernen/neu erzeugen.

## Arbeitsschritte
1. [x] Test-Build v32.1 unverändert (24 min, OK)
2. [x] Konsens-Parameter (chainparams, amount, consensus.h, Subsidy, Ports)
3. [x] pow.cpp Zwei-Phasen-Retarget
4. [x] Script-Flags/BIP30/Deployments wie Legacy
5. [x] P2P: NODE_WITNESS nicht verlangen
6. [x] Wallet: legacy-Adressen als Standard, bech32 aus
7. [x] Umbenennung bitcoin → aurumcoin (Binaries, Datenverzeichnis `.aurumcoin`, Config `aurumcoin.conf`)
   - 2026-11-13: Programme aurumcoind/-cli/-tx/-util/-wallet/aurumcoin (Wrapper), test_aurumcoin
     (zentral über OUTPUT_NAME in cmake/module/InstallBinaryComponent.cmake, CMake-Targets heißen weiter bitcoin*),
     CLIENT_NAME "Aurumcoin AU", User-Agent /Aurum:32.1.0/ (wie 0.12: /Aurum:0.12.4/),
     Datenverzeichnis ~/.aurumcoin, Windows %APPDATA%\Aurumcoin (0.12-Ort) sonst %LOCALAPPDATA%\Aurumcoin,
     macOS ~/Library/Application Support/Aurumcoin, aurumcoin.conf, aurumcoind.pid,
     URLs → github.com/aurumcoin-au/Aurumcoin-AU, RPC-Hilfebeispiele aurumcoin-cli / Port 21080.
     Copyright-Zeile "The Bitcoin Core developers" bleibt (MIT-Lizenz).
   - Bekannte Unit-Test-Fehler aus Version A (nicht Umbenennung): TestChain100Setup (fester Regtest-Hash),
     util_ParseMoney, rpc_parse_monetary_values (MAX_MONEY) → Schritt 8.
8. [ ] Unit-Tests anpassen
9. [~] Konsens-Test: kompletter Sync der echten Chain, Vergleich mit 0.12-Referenz-Node
   - 2026-11-13: Bootstrap 1348395.AU1 komplett eingelesen (-assumevalid=0), 0 abgelehnte Blöcke,
     Spitze 00000000a1658908e661068c0e3b0cffdf54e396fdac2de78166ea8267e29378 = Legacy-Spitze.
     chainwork e530b7beb5ec34e7ec, UTXO 130031 Outputs / 299370.53706425 AU (max. möglich 299406.796875).
   - 2026-11-13 (neuer Rechner C:\AurumCoin, WSL 16 GB): Wiederholung mit gleichem Patch, -dbcache=4000:
     Ergebnis identisch (Spitze, chainwork, 130031 Outputs / 299370.53706425 AU), 0 Fehler, Dauer 241 s.
   - offen: Live-Test gegen 0.12-Nodes (P2P), neue Blöcke schürfen (Regtest/Testumgebung)
10. [ ] Wallet-Migration einer wallet.dat-Kopie testen
11. [ ] Dockerfile (mehrstufig)

## Umgesetzt in Version A (Branch aurumcoin-port, noch nicht committet)
- `consensus/params.h`: nPowTargetTimespan2, nPowRetargetBlockStart, DifficultyAdjustmentIntervalAt(), PowTargetTimespanAt(),
  legacy_bip30_skip_height/hash, witness_flags_require_segwit, nInitialSubsidy
- `pow.cpp`: GetNextWorkRequired, CalculateNextWorkRequired, PermittedDifficultyTransition phasenabhängig
- `validation.cpp`: Subsidy aus Parameter, Script-Flags WITNESS/TAPROOT nur mit SegWit, BIP30 wie Legacy,
  Mempool lehnt Witness-Daten und Witness-Outputs ab solange SegWit inaktiv
- `kernel/chainparams.cpp`: Aurumcoin-Mainnet (Genesis, Ports, Seeds, Deployments nie, bech32_hrp "au")
- `consensus/amount.h` MAX_MONEY 300000, `consensus/consensus.h` COINBASE_MATURITY 2, `policy/feerate.h` AU,
  `chainparamsbase.cpp` RPC 21080
- `net_processing.cpp`/`protocol.h`/`net.cpp`: kein NODE_WITNESS verlangt, DNS-Seeds ohne x9-Präfix
- `wallet/wallet.h/.cpp`: Standard legacy, nur legacy-Adressen und -Wechselgeld
- Testnet3/4, Signet, Regtest: noch Bitcoin-Parameter (außer globalen Konstanten) → später
