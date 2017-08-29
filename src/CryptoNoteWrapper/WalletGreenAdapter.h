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

#include "IWalletAdapter.h"

namespace CryptoNote {
  class Currency;
  class INode;
}

namespace Logging {
  class ILogger;
}

namespace WalletGui {

class WalletGreenAdapter : public QObject, public IWalletAdapter, public IWalletAdapterObserver {
  Q_OBJECT
  Q_DISABLE_COPY(WalletGreenAdapter)

public:
  WalletGreenAdapter(const CryptoNote::Currency& _currency, CryptoNote::INode& _node, Logging::ILogger& _logger, QObject* _parent);
  virtual ~WalletGreenAdapter();

  // IWalletAdapter
  virtual WalletInitStatus create(const QString& _walletPath, const QString& _password) override;
  virtual WalletInitStatus load(const QString& _walletPath, const QString& _password) override;
  virtual WalletInitStatus loadLegacyKeys(const QString& _legacyKeysFile, const QString& _walletPath, const QString& _password) override;
  virtual WalletInitStatus createWithKeys(const QString& _walletPath, const AccountKeys& _accountKeys) override;
  virtual WalletSaveStatus save(CryptoNote::WalletSaveLevel _saveLevel, bool _saveUserData) override;
  virtual WalletSaveStatus exportWallet(const QString& _path, bool _encrypt, CryptoNote::WalletSaveLevel _saveLevel, bool _saveUserData) override;
  virtual PasswordStatus changePassword(const QString& _oldPassword, const QString& _newPassword) override;
  virtual void close() override;

  virtual bool isOpen() const override;
  virtual bool isEncrypted() const override;
  virtual bool isTrackingWallet() const override;
  virtual QString getAddress(quintptr _addressIndex) const override;
  virtual AccountKeys getAccountKeys(quintptr _addressIndex) const override;
  virtual quint64 getActualBalance() const override;
  virtual quint64 getPendingBalance() const override;
  virtual quintptr getTransactionCount() const override;
  virtual quintptr getTransactionTransferCount(quintptr _transactionIndex) const override;
  virtual bool getTransaction(quintptr _transactionIndex, CryptoNote::WalletTransaction& _transaction) const override;
  virtual bool getFullTransactionInfo(quintptr _transactionIndex, FullTransactionInfo& _transactionInfo) const override;
  virtual bool getAllTransactions(QHash<quintptr, FullTransactionInfo>& _transactionInfos) const override;
  virtual bool getTransactionTransfer(quintptr _transactionIndex, quintptr _transferIndex, CryptoNote::WalletTransfer& _transfer) const override;
  virtual bool isFusionTransaction(quintptr _transactionIndex) const override;
  virtual QByteArray getUserData() const override;

  virtual SendTransactionStatus sendTransaction(const CryptoNote::TransactionParameters& _transactionParameters) override;
  virtual bool createFusionTransaction(quint64 _threshold, quint64 _mixin, const QString& _destinationAddress) override;
  virtual quintptr getOutputsToOptimizeCount(quint64 _threshold) const override;
  virtual void setUserData(const QByteArray& _userData) override;

  virtual void addObserver(IWalletAdapterObserver* _observer) override;
  virtual void removeObserver(IWalletAdapterObserver* _observer) override;

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

private:
  const CryptoNote::Currency& m_currency;
  CryptoNote::INode& m_node;
  Logging::ILogger& m_logger;

  QThread* m_workerThread;
  IWalletAdapter* m_worker;

  void createWorker();
  void deleteWorker();

Q_SIGNALS:
  void walletOpenedSignal();
  void walletOpenErrorSignal(int _initStatus);
  void walletClosedSignal();
  void passwordChangedSignal();
  void synchronizationProgressUpdatedSignal(quint32 _current, quint32 _total);
  void synchronizationCompletedSignal();
  void balanceUpdatedSignal(quint64 _actualBalance, quint64 _pendingBalance);
  void externalTransactionCreatedSignal(quintptr _transactionId, const FullTransactionInfo& _transaction);
  void transactionUpdatedSignal(quintptr _transactionId, const FullTransactionInfo& _transaction);
};

}
