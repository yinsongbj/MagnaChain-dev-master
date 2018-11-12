// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2016-2018 The CellLink Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CELLLINK_SCRIPT_ISMINE_H
#define CELLLINK_SCRIPT_ISMINE_H

#include "script/standard.h"

#include <stdint.h>

class CellKeyStore;
class CellScript;

/** IsMine() return codes */
enum isminetype
{
    ISMINE_NO = 0,
    //! Indicates that we don't know how to create a scriptSig that would solve this if we were given the appropriate private keys
    ISMINE_WATCH_UNSOLVABLE = 1,
    //! Indicates that we know how to create a scriptSig that would solve this if we were given the appropriate private keys
    ISMINE_WATCH_SOLVABLE = 2,
    ISMINE_WATCH_ONLY = ISMINE_WATCH_SOLVABLE | ISMINE_WATCH_UNSOLVABLE,
    ISMINE_SPENDABLE = 4,
    ISMINE_ALL = ISMINE_WATCH_ONLY | ISMINE_SPENDABLE
};
/** used for bitflags of isminetype */
typedef uint8_t isminefilter;

/* isInvalid becomes true when the script is found invalid by consensus or policy. This will terminate the recursion
 * and return a ISMINE_NO immediately, as an invalid script should never be considered as "mine". This is needed as
 * different SIGVERSION may have different network rules. Currently the only use of isInvalid is indicate uncompressed
 * keys in SIGVERSION_WITNESS_V0 script, but could also be used in similar cases in the future
 */
isminetype IsMine(const CellKeyStore& keystore, const CellScript& scriptPubKey, bool& isInvalid, SigVersion = SIGVERSION_BASE);
isminetype IsMine(const CellKeyStore& keystore, const CellScript& scriptPubKey, SigVersion = SIGVERSION_BASE);
isminetype IsMine(const CellKeyStore& keystore, const CellTxDestination& dest, bool& isInvalid, SigVersion = SIGVERSION_BASE);
isminetype IsMine(const CellKeyStore& keystore, const CellTxDestination& dest, SigVersion = SIGVERSION_BASE);

#endif // CELLLINK_SCRIPT_ISMINE_H