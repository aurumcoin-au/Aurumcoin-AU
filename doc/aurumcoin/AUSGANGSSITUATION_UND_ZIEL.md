# Aurumcoin (AU) – Ausgangssituation und Ziel

Stand: 13.11.2026

---

## 1. Ausgangssituation

### Software
- Aurumcoin AU läuft mit der Client-Version **0.12.4**. Technisch ist das **Bitcoin Core 0.12.0** aus dem Jahr 2016 mit wenigen Anpassungen. Die letzte Änderung am Code stammt von 2018.
- Diese Version ist veraltet. Sie lässt sich auf aktuellen Systemen nur noch mit Anpassungen bauen (Portierung auf Ubuntu 24.04 im Commit `c37fe10`). Außerdem braucht sie alte Bibliotheken wie Berkeley DB 4.8 und hat seit Jahren keine Sicherheitsupdates mehr bekommen.
- Die Builds sind teils per `docker commit` entstanden und damit nicht reproduzierbar. Das betrifft zum Beispiel das Image `aurumcoin:ubuntu24-wallet`. Außerdem liegen Build-Ergebnisse mit im Git-Repository.

### Was Aurumcoin von Bitcoin unterscheidet
| Merkmal | Wert |
|---|---|
| Blockbelohnung | 1 AU, Halving alle 150.000 Blöcke |
| Maximale Menge | 300.000 AU |
| Blockzeit | 60 Sekunden |
| Schwierigkeit | Anpassung alle 300 Blöcke, ab Block 1.086.560 alle 2 Blöcke |
| Coinbase-Reife | 2 Blöcke |
| Mining | SHA256, wie Bitcoin |
| Ports | P2P 11080, RPC 21080 |
| Magic Bytes, Adressformat | **identisch mit Bitcoin** |
| Soft-Forks | BIP34/65/66, CSV, SegWit und Taproot sind nie aktiv geworden |

### Netzwerk und Blockchain
- Die Chain **steht seit dem 19.09.2024 still**. Letzter Block ist Höhe **1.348.395**, Hash `00000000a1658908e661068c0e3b0cffdf54e396fdac2de78166ea8267e29378`.
- Im Umlauf sind rund **299.370 AU** in 130.031 Outputs.
- Das Netz besteht praktisch nur noch aus den eigenen Rotations-Nodes AU1 bis AU3 und vereinzelten weiteren 0.12-Nodes.
- Weil die Magic Bytes mit Bitcoin identisch sind, **verbinden sich die AU-Nodes überwiegend mit Bitcoin-Nodes**. Das blockiert Verbindungen und füllt die Adresslisten mit fremden Peers.
- Die vollständige Chain ist gesichert, als Bootstrap-Datei `1348395.AU1.bootstrap.dat` und als Blockdaten der Nodes.

### Erreichter Zwischenstand der Portierung
- Die Aurumcoin-Regeln sind auf **Bitcoin Core v32.1** übertragen. Das ist der Arbeitsstand „Version A“, kompatibel zu 0.12.
- **Konsens-Test bestanden:** Die neue Version hat die komplette Chain bis Block 1.348.395 mit vollständiger Prüfung validiert und keinen Block abgelehnt. Der letzte Block ist identisch mit dem des 0.12-Nodes.
- Die Details stehen in `port-tools/PORTING.md`, der Übertragungsstand in `transfer/`.

---

## 2. Ziel

Aurumcoin soll auf eine **aktuelle, sichere und wartbare Software-Basis** umziehen. Die bestehende Blockchain mit allen Guthaben bleibt dabei erhalten, und das Netz soll wieder lebendig werden.

### Vorgehen in zwei Versionen

**Version A: Übergang, kompatibel zu 0.12**
- Aktuelles Bitcoin Core, im Moment v32.1, mit exakt denselben Konsensregeln wie 0.12.
- Sie läuft parallel zu den alten Nodes, damit die Chain während der Testphase weiterlaufen kann.
- SegWit und Taproot bleiben **abgeschaltet**. Die Wallet erzeugt nur legacy-Adressen.
- Zweck: neue Software unter echten Bedingungen testen, auch im Zusammenspiel mit 0.12-Nodes.

**Version B: final, Trennung vom alten Netz**
- Öffentliches Release mit vorheriger **Ankündigung an alle Nutzer**.
- Ab einer festgelegten Blockhöhe H (Hard Fork):
  - **neue Magic Bytes und Ports**, damit ist auch die Verwechslung mit Bitcoin beseitigt;
  - alte 0.12-Nodes und deren Blöcke werden nicht mehr akzeptiert, über eine Konsensänderung wie eine neue Mindest-Blockversion;
  - die Historie bis H, also alle Guthaben und Adressen, wird unverändert übernommen.
- Noch zu entscheiden:
  - die Schwierigkeit bei H zurücksetzen, damit wieder geschürft werden kann;
  - wer danach schürft (eigene Miner oder ein Pool);
  - ob SegWit und Taproot mit dem Fork eingeschaltet werden.

### Weitere Ziele
- Umbenennung der Programme und Pfade in `aurumcoind`, `aurumcoin-cli`, `.aurumcoin` und `aurumcoin.conf`.
- Reproduzierbarer Build als mehrstufiges Dockerfile statt per `docker commit`.
- Übernahme bestehender Wallets: `wallet.dat` aus 0.12 über `migratewallet` ins neue Format.
- Neue Seed-Nodes und DNS-Seeds für das neue Netz.
- Angepasste Tests (Unit-Tests) und eine Dokumentation für Betreiber und Nutzer.

### Erfolgskriterien
1. Die neue Software validiert die komplette bestehende Chain fehlerfrei (für Version A bereits erfüllt).
2. Neue Blöcke von Version A werden von 0.12-Nodes akzeptiert und umgekehrt.
3. Bestehende Wallets lassen sich ohne Verlust übernehmen.
4. Nach dem Fork (Version B) läuft das Netz stabil, mit regelmäßig neuen Blöcken und ohne Verbindungen zu Bitcoin-Nodes.
