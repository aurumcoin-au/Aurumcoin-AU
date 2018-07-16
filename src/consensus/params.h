// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef AURUMCOIN_CONSENSUS_PARAMS_H
#define AURUMCOIN_CONSENSUS_PARAMS_H

#include "uint256.h"

namespace Consensus {
/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    int nSubsidyHalvingInterval;
    /** Used to check majorities for block version upgrade */
    int nMajorityEnforceBlockUpgrade;
    int nMajorityRejectBlockOutdated;
    int nMajorityWindow;
    /** Block height and hash at which BIP34 becomes active */
    int BIP34Height;
    uint256 BIP34Hash;
    /** Proof of work parameters */
    uint256 powLimit;
    bool fPowAllowMinDifficultyBlocks;
    bool fPowNoRetargeting;
    int64_t nPowTargetSpacing;
    int64_t nPowTargetTimespan1;
    int64_t nPowTargetTimespan2;
    int64_t nPowRetargetBlockStart;
    int64_t DifficultyAdjustmentInterval1() const { return nPowTargetTimespan1 / nPowTargetSpacing; }
    int64_t DifficultyAdjustmentInterval2() const { return nPowTargetTimespan2 / nPowTargetSpacing; }
};
} // namespace Consensus

#endif // AURUMCOIN_CONSENSUS_PARAMS_H
