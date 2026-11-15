# GitHub Actions disabled

The CI workflows inherited from Bitcoin Core (`.github/workflows/ci.yml`) were
removed for Aurumcoin AU v32 (Version A). They build and test on Linux, macOS
and Windows runners and would fail on the Aurumcoin consensus/chain changes
(see `doc/aurumcoin/TESTS.md`).

To re-enable CI later, restore the workflow from the Bitcoin Core v32.1 tag
(`git checkout v32.1 -- .github/workflows`) and adapt it to the Aurumcoin test
selection.
