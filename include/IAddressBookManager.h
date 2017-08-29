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

const quintptr INVALID_ADDRESS_INDEX = std::numeric_limits<quintptr>::max();

struct AddressItem {
  QString label;
  QString address;
  bool isDonationAddress;
};

class IAddressBookManagerObserver {
public:
  virtual ~IAddressBookManagerObserver() {}
  virtual void addressBookOpened() = 0;
  virtual void addressBookClosed() = 0;
  virtual void addressAdded(quintptr _addressIndex) = 0;
  virtual void addressEdited(quintptr _addressIndex) = 0;
  virtual void addressRemoved(quintptr _addressIndex) = 0;
};

class IAddressBookManager {
public:
  virtual ~IAddressBookManager() {}
  virtual quintptr getAddressCount() const = 0;
  virtual AddressItem getAddress(quintptr _addressIndex) const = 0;
  virtual quintptr findAddressByAddress(const QString& _address) const = 0;
  virtual quintptr findAddressByLabel(const QString& _label) const = 0;
  virtual quintptr findAddress(const QString& _label, const QString& _address) const = 0;
  virtual quintptr findAddress(const QString& _label, const QString& _address, bool _isDonationAddress) const = 0;
  virtual void addAddress(const QString& _label, const QString& _address, bool _isDonationAddress) = 0;
  virtual void editAddress(quintptr _addressIndex, const QString& _label, const QString& _address, bool _isDonationAddress) = 0;
  virtual void removeAddress(quintptr _addressIndex) = 0;

  virtual void addObserver(IAddressBookManagerObserver* _observer) = 0;
  virtual void removeObserver(IAddressBookManagerObserver* _observer) = 0;
};

}
