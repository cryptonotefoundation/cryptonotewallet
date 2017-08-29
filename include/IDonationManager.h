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

namespace WalletGui {

class IDonationManagerObserver {
public:
  virtual ~IDonationManagerObserver() {}
  virtual void donationManagerOpened() = 0;
  virtual void donationManagerClosed() = 0;
  virtual void donationMiningEnabled(bool _on) = 0;
  virtual void donationMiningAddressChanged(const QString& _address) = 0;
  virtual void donationMiningAmountChanged(int _amount) = 0;
  virtual void donationChangeEnabled(bool _on) = 0;
  virtual void donationChangeAddressChanged(const QString& _address) = 0;
  virtual void donationChangeAmountChanged(int _amount) = 0;
};

class IDonationManager {
public:
  virtual ~IDonationManager() {}
  virtual bool isDonationMiningEnabled() const = 0;
  virtual QString getDonationMiningAddress() const = 0;
  virtual int getDonationMiningAmount() const = 0;

  virtual bool isDonationChangeEnabled() const = 0;
  virtual QString getDonationChangeAddress() const = 0;
  virtual int getDonationChangeAmount() const = 0;

  virtual void setDonationMiningEnabled(bool _on) = 0;
  virtual void setDonationMiningAddress(const QString& _address) = 0;
  virtual void setDonationMiningAmount(int _amount) = 0;

  virtual void setDonationChangeEnabled(bool _on) = 0;
  virtual void setDonationChangeAddress(const QString& _address) = 0;
  virtual void setDonationChangeAmount(int _amount) = 0;

  virtual void addObserver(IDonationManagerObserver* _observer) = 0;
  virtual void removeObserver(IDonationManagerObserver* _observer) = 0;
};

}
