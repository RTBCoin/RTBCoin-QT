// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef RTB_QT_RTBADDRESSVALIDATOR_H
#define RTB_QT_RTBADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class KomodoAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit KomodoAddressEntryValidator(QObject *parent, bool allowZAddresses = false);

    State validate(QString &input, int &pos) const;
private:
    bool _allowZAddresses;
};

/** RTB address widget validator, checks for a valid rtb address.
 */
class KomodoAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit KomodoAddressCheckValidator(QObject *parent, bool allowZAddresses = false);

    State validate(QString &input, int &pos) const;
private:
    bool _allowZAddresses;
};

#endif // RTB_QT_RTBADDRESSVALIDATOR_H
