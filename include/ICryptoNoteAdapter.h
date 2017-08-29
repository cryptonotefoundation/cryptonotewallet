// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Chavezcoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Chavezcoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Chavezcoin.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <QObject>

#include "INodeAdapter.h"

namespace WalletGui {

class ICryptoNoteAdapterObserver {
public:
  virtual ~ICryptoNoteAdapterObserver() {}
  virtual void cryptoNoteAdapterInitCompleted(int _status) = 0;
  virtual void cryptoNoteAdapterDeinitCompleted() = 0;
};

class ICryptoNoteAdapter {
public:
  virtual ~ICryptoNoteAdapter() {}

  virtual int init(ConnectionMethod _connectionMethod, quint16 _localDaemonPort, const QUrl& _remoteDaemonUrl) = 0;
  virtual void deinit() = 0;

  virtual INodeAdapter* getNodeAdapter() const = 0;
  virtual bool isValidAddress(const QString& _address) const = 0;
  virtual bool isValidPaymentId(const QString& _paymentId) const = 0;
  virtual QString getCurrencyTicker() const = 0;
  virtual quint64 getMinimalFee() const = 0;
  virtual quint64 getTargetTime() const = 0;
  virtual QString formatAmount(qint64 _amount) const = 0;
  virtual QString formatUnsignedAmount(quint64 _amount) const = 0;
  virtual QString formatAmountToShort(qint64 _amount) const = 0;
  virtual qint64 parseAmount(const QString& _amountString) const = 0;
  virtual quint64 parseUnsignedAmount(const QString& _amountString) const = 0;
  virtual std::string convertPaymentToExtra(const QString& _paymentIdString) const = 0;
  virtual QString extractPaymentIdFromExtra(const std::string& _extra) const = 0;
  virtual void addObserver(ICryptoNoteAdapterObserver* _observer) = 0;
  virtual void removeObserver(ICryptoNoteAdapterObserver* _observer) = 0;
};

}
