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

#include "CryptoNote.h"
#include "IWallet.h"

namespace CryptoNote {
  struct WalletTransaction;
  struct WalletTransfer;
  struct TransactionParameters;
}

namespace WalletGui {

struct AccountKeys {
  CryptoNote::KeyPair viewKeys;
  CryptoNote::KeyPair spendKeys;
};

struct FullTransactionInfo {
  CryptoNote::WalletTransaction walletTransaction;
  bool isFusionTransaction;
  QList<CryptoNote::WalletTransfer> transfers;
};

class IWalletAdapterObserver {
public:
  virtual ~IWalletAdapterObserver() {}
  virtual void walletOpened() = 0;
  virtual void walletOpenError(int _initStatus) = 0;
  virtual void walletClosed() = 0;
  virtual void passwordChanged() = 0;
  virtual void synchronizationProgressUpdated(quint32 _current, quint32 _total) = 0;
  virtual void synchronizationCompleted() = 0;
  virtual void balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) = 0;
  virtual void externalTransactionCreated(quintptr _transactionId, const FullTransactionInfo& _transaction) = 0;
  virtual void transactionUpdated(quintptr _transactionId, const FullTransactionInfo& _transaction) = 0;
};

class IWalletAdapter {
public:
  enum WalletInitStatus {
    INIT_SUCCESS = 0, INIT_WRONG_PASSWORD, INIT_INTERNAL_ERROR
  };

  enum SendTransactionStatus {
    SEND_SUCCESS = 0, SEND_BIG_TRANSACTION_SIZE, SEND_WRONG_AMOUNT, SEND_BIG_MIXIN, SEND_INTERNAL_ERROR
  };

  enum WalletSaveStatus {
    SAVE_SUCCESS = 0, SAVE_ERROR
  };

  enum PasswordStatus {
    PASSWORD_SUCCESS = 0, PASSWORD_ERROR
  };

  virtual ~IWalletAdapter() {}

  virtual WalletInitStatus create(const QString& _walletPath, const QString& _password) = 0;
  virtual WalletInitStatus load(const QString& _walletPath, const QString& _password) = 0;
  virtual WalletInitStatus loadLegacyKeys(const QString& _legacyKeysFile, const QString& _walletPath, const QString& _password) = 0;
  virtual WalletInitStatus createWithKeys(const QString& _walletPath, const AccountKeys& _accountKeys) = 0;
  virtual WalletSaveStatus save(CryptoNote::WalletSaveLevel _saveLevel, bool _saveUserData) = 0;
  virtual WalletSaveStatus exportWallet(const QString& _path, bool _encrypt, CryptoNote::WalletSaveLevel _saveLevel, bool _saveUserData) = 0;
  virtual PasswordStatus changePassword(const QString& _oldPassword, const QString& _newPassword) = 0;
  virtual void close() = 0;

  virtual bool isOpen() const = 0;
  virtual bool isEncrypted() const = 0;
  virtual bool isTrackingWallet() const = 0;
  virtual QString getAddress(quintptr _addressIndex) const = 0;
  virtual AccountKeys getAccountKeys(quintptr _addressIndex) const = 0;
  virtual quint64 getActualBalance() const = 0;
  virtual quint64 getPendingBalance() const = 0;
  virtual quintptr getTransactionCount() const = 0;
  virtual quintptr getTransactionTransferCount(quintptr _transactionIndex) const = 0;
  virtual bool getTransaction(quintptr _transactionIndex, CryptoNote::WalletTransaction& _transaction) const = 0;
  virtual bool getFullTransactionInfo(quintptr _transactionIndex, FullTransactionInfo& _transactionInfo) const = 0;
  virtual bool getAllTransactions(QHash<quintptr, FullTransactionInfo>& _transactionInfos) const = 0;
  virtual bool getTransactionTransfer(quintptr _transactionIndex, quintptr _transferIndex, CryptoNote::WalletTransfer& _transfer) const = 0;
  virtual bool isFusionTransaction(quintptr _transactionIndex) const = 0;
  virtual QByteArray getUserData() const = 0;

  virtual SendTransactionStatus sendTransaction(const CryptoNote::TransactionParameters& _transactionParameters) = 0;
  virtual bool createFusionTransaction(quint64 _threshold, quint64 _mixin, const QString& _destinationAddress) = 0;
  virtual quintptr getOutputsToOptimizeCount(quint64 _threshold) const = 0;
  virtual void setUserData(const QByteArray& _userData) = 0;

  virtual void addObserver(IWalletAdapterObserver* _observer) = 0;
  virtual void removeObserver(IWalletAdapterObserver* _observer) = 0;
};

}

Q_DECLARE_TYPEINFO(WalletGui::FullTransactionInfo, Q_MOVABLE_TYPE);
