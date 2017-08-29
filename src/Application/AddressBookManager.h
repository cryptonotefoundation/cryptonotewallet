// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Intensecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Intensecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Intensecoin.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <QObject>
#include <QHash>
#include <QJsonObject>

#include "IAddressBookManager.h"
#include "ICryptoNoteAdapter.h"
#include "IDonationManager.h"
#include "IWalletAdapter.h"

namespace WalletGui {

class AddressBookManager : public QObject, public IAddressBookManager, public IDonationManager, public IWalletAdapterObserver,
  public ICryptoNoteAdapterObserver {
  Q_OBJECT
  Q_DISABLE_COPY(AddressBookManager)

public:
  AddressBookManager(ICryptoNoteAdapter* _cryptoNoteAdapter, QObject* _parent);
  virtual ~AddressBookManager();

  // IAddressBookManager
  virtual quintptr getAddressCount() const override;
  virtual AddressItem getAddress(quintptr _addressIndex) const override;
  virtual quintptr findAddressByAddress(const QString& _address) const override;
  virtual quintptr findAddressByLabel(const QString& _label) const override;
  virtual quintptr findAddress(const QString& _label, const QString& _address) const override;
  virtual quintptr findAddress(const QString& _label, const QString& _address, bool _isDonationAddress) const override;
  virtual void addAddress(const QString& _label, const QString& _address, bool _isDonationAddress) override;
  virtual void editAddress(quintptr _addressIndex, const QString& _label, const QString& _address, bool _isDonationAddress) override;
  virtual void removeAddress(quintptr _addressIndex) override;
  virtual void addObserver(IAddressBookManagerObserver* _observer) override;
  virtual void removeObserver(IAddressBookManagerObserver* _observer) override;

  // IDonationManager
  virtual bool isDonationMiningEnabled() const override;
  virtual QString getDonationMiningAddress() const override;
  virtual int getDonationMiningAmount() const override;
  virtual bool isDonationChangeEnabled() const override;
  virtual QString getDonationChangeAddress() const override;
  virtual int getDonationChangeAmount() const override;
  virtual void setDonationMiningEnabled(bool _on) override;
  virtual void setDonationMiningAddress(const QString& _address) override;
  virtual void setDonationMiningAmount(int _amount) override;
  virtual void setDonationChangeEnabled(bool _on) override;
  virtual void setDonationChangeAddress(const QString& _address) override;
  virtual void setDonationChangeAmount(int _amount) override;
  virtual void addObserver(IDonationManagerObserver* _observer) override;
  virtual void removeObserver(IDonationManagerObserver* _observer) override;

  // IWalletAdapterObserver
  Q_SLOT virtual void walletOpened() override;
  Q_SLOT virtual void walletOpenError(int _initStatus) override;
  Q_SLOT virtual void walletClosed() override;
  Q_SLOT virtual void passwordChanged() override;
  Q_SLOT virtual void synchronizationProgressUpdated(quint32 _current, quint32 _total) override;
  Q_SLOT virtual void synchronizationCompleted() override;
  Q_SLOT virtual void balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) override;
  Q_SLOT virtual void externalTransactionCreated(quintptr _transactionId, const FullTransactionInfo& _transaction) override;
  Q_SLOT virtual void transactionUpdated(quintptr _transactionId, const FullTransactionInfo& _transaction) override;

  // ICryptoNoteAdapterObserver
  Q_SLOT virtual void cryptoNoteAdapterInitCompleted(int _status) override;
  Q_SLOT virtual void cryptoNoteAdapterDeinitCompleted() override;

private:
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  QJsonObject m_addressBookObject;
  QHash<QString, quintptr> m_addressIndexes;
  QHash<QString, quintptr> m_labelIndexes;

  void saveAddressBook();
  void buildIndexes();

Q_SIGNALS:
  void addressBookOpenedSignal();
  void addressBookClosedSignal();
  void addressAddedSignal(quintptr _addressIndex);
  void addressEditedSignal(quintptr _addressIndex);
  void addressRemovedSignal(quintptr _addressIndex);

  void donationManagerOpenedSignal();
  void donationManagerClosedSignal();
  void donationMiningEnabledSignal(bool _on);
  void donationMiningAddressChangedSignal(const QString& _address);
  void donationMiningAmountChangedSignal(int _amount);
  void donationChangeEnabledSignal(bool _on);
  void donationChangeAddressChangedSignal(const QString& _address);
  void donationChangeAmountChangedSignal(int _amount);
};

}
