// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef AURUMCOIN_QT_AURUMCOINADDRESSVALIDATOR_H
#define AURUMCOIN_QT_AURUMCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class AurumcoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit AurumcoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Aurumcoin address widget validator, checks for a valid aurumcoin address.
 */
class AurumcoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit AurumcoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // AURUMCOIN_QT_AURUMCOINADDRESSVALIDATOR_H
