// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "CurrencyAdapter.h"
#include "CryptoNoteWalletConfig.h"
#include "LoggerAdapter.h"

namespace WalletGui {

CurrencyAdapter& CurrencyAdapter::instance() {
  static CurrencyAdapter inst;
  return inst;
}

CurrencyAdapter::CurrencyAdapter() : m_currency(CryptoNote::CurrencyBuilder(LoggerAdapter::instance().getLoggerManager()).currency()) {
}

CurrencyAdapter::~CurrencyAdapter() {
}

const CryptoNote::Currency& CurrencyAdapter::getCurrency() {
  return m_currency;
}

quintptr CurrencyAdapter::getNumberOfDecimalPlaces() const {
  return m_currency.numberOfDecimalPlaces();
}

QString CurrencyAdapter::getCurrencyDisplayName() const {
  return WALLET_CURRENCY_DISPLAY_NAME;
}

QString CurrencyAdapter::getCurrencyName() const {
  return CryptoNote::CRYPTONOTE_NAME;
}

QString CurrencyAdapter::getCurrencyTicker() const {
  return WALLET_CURRENCY_TICKER;
}

quint64 CurrencyAdapter::getMinimumFee() const {
  return m_currency.minimumFee();
}

QString CurrencyAdapter::formatAmount(quint64 _amount) const {
  QString result = QString::number(_amount);
  if (result.length() < getNumberOfDecimalPlaces() + 1) {
    result = result.rightJustified(getNumberOfDecimalPlaces() + 1, '0');
  }

  quint32 dot_pos = result.length() - getNumberOfDecimalPlaces();
  for (quint32 pos = result.length() - 1; pos > dot_pos + 1; --pos) {
    if (result[pos] == '0') {
      result.remove(pos, 1);
    } else {
      break;
    }
  }

  result.insert(dot_pos, ".");
  for (qint32 pos = dot_pos - 3; pos > 0; pos -= 3) {
    if (result[pos - 1].isDigit()) {
      result.insert(pos, ',');
    }
  }

  return result;
}

quint64 CurrencyAdapter::parseAmount(const QString& _amountString) const {
  QString amountString = _amountString.trimmed();
  amountString.remove(',');

  int pointIndex = amountString.indexOf('.');
  int fractionSize;
  if (pointIndex != -1) {
    fractionSize = amountString.length() - pointIndex - 1;
    while (getNumberOfDecimalPlaces() < fractionSize && amountString.right(1) == "0") {
      amountString.remove(amountString.length() - 1, 1);
      --fractionSize;
    }

    if (getNumberOfDecimalPlaces() < fractionSize) {
      return 0;
    }

    amountString.remove(pointIndex, 1);
  } else {
    fractionSize = 0;
  }

  if (amountString.isEmpty()) {
    return 0;
  }

  for (qint32 i = 0; i < getNumberOfDecimalPlaces() - fractionSize; ++i) {
    amountString.append('0');
  }

  return amountString.toULongLong();
}

bool CurrencyAdapter::validateAddress(const QString& _address) const {
  CryptoNote::AccountPublicAddress internalAddress;
  return m_currency.parseAccountAddressString(_address.toStdString(), internalAddress);
}

}
