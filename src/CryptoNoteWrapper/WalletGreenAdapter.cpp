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

#include <QCoreApplication>
#include <QThread>

#include "WalletGreenAdapter.h"
#include "WalletGreenWorker.h"

namespace WalletGui {

WalletGreenAdapter::WalletGreenAdapter(const CryptoNote::Currency& _currency, CryptoNote::INode& _node, Logging::ILogger& _logger, QObject* _parent) :
  QObject(_parent), m_currency(_currency), m_node(_node), m_logger(_logger), m_workerThread(new QThread(this)),
  m_worker(nullptr) {
  qRegisterMetaType<FullTransactionInfo>("FullTransactionInfo");
}

WalletGreenAdapter::~WalletGreenAdapter() {
}

IWalletAdapter::WalletInitStatus WalletGreenAdapter::create(const QString& _walletPath, const QString& _password) {
  createWorker();
  WalletInitStatus status = m_worker->create(_walletPath, _password);
  if (status != INIT_SUCCESS) {
    deleteWorker();
  }

  return status;
}

IWalletAdapter::WalletInitStatus WalletGreenAdapter::load(const QString& _walletPath, const QString& _password) {
  createWorker();
  WalletInitStatus status = m_worker->load(_walletPath, _password);
  if (status != INIT_SUCCESS) {
    deleteWorker();
  }

  return status;
}

IWalletAdapter::WalletInitStatus WalletGreenAdapter::loadLegacyKeys(const QString& _legacyKeysFile, const QString& _walletPath, const QString& _password) {
  createWorker();
  WalletInitStatus status = m_worker->loadLegacyKeys(_legacyKeysFile, _walletPath, _password);
  if (status != INIT_SUCCESS) {
    deleteWorker();
  }

  return status;
}

IWalletAdapter::WalletInitStatus WalletGreenAdapter::createWithKeys(const QString& _walletPath, const AccountKeys& _accountKeys) {
  createWorker();
  WalletInitStatus status = m_worker->createWithKeys(_walletPath, _accountKeys);
  if (status != INIT_SUCCESS) {
    deleteWorker();
  }

  return status;
}

IWalletAdapter::WalletSaveStatus WalletGreenAdapter::save(CryptoNote::WalletSaveLevel _saveLevel, bool _saveUserData) {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->save(_saveLevel, _saveUserData);
}

IWalletAdapter::WalletSaveStatus WalletGreenAdapter::exportWallet(const QString& _path, bool _encrypt, CryptoNote::WalletSaveLevel _saveLevel, bool _saveUserData) {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->exportWallet(_path, _encrypt, _saveLevel, _saveUserData);
}

IWalletAdapter::PasswordStatus WalletGreenAdapter::changePassword(const QString& _oldPassword, const QString& _newPassword) {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->changePassword(_oldPassword, _newPassword);
}

bool WalletGreenAdapter::resetPendingTransactions() const {
	Q_ASSERT(m_worker != nullptr);
	return m_worker->resetPendingTransactions();
}

void WalletGreenAdapter::close() {
  m_worker->removeObserver(this);
  deleteWorker();
  Q_EMIT walletClosed();
}

bool WalletGreenAdapter::isOpen() const {
  return m_worker != nullptr && m_worker->isOpen();
}

bool WalletGreenAdapter::isEncrypted() const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->isEncrypted();
}

bool WalletGreenAdapter::isTrackingWallet() const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->isTrackingWallet();
}

QString WalletGreenAdapter::getAddress(quintptr _addressIndex) const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getAddress(_addressIndex);
}

AccountKeys WalletGreenAdapter::getAccountKeys(quintptr _addressIndex) const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getAccountKeys(_addressIndex);
}

quint64 WalletGreenAdapter::getActualBalance() const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getActualBalance();
}

quint64 WalletGreenAdapter::getPendingBalance() const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getPendingBalance();
}

quintptr WalletGreenAdapter::getTransactionCount() const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getTransactionCount();
}

quintptr WalletGreenAdapter::getTransactionTransferCount(quintptr _transactionIndex) const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getTransactionTransferCount(_transactionIndex);
}

bool WalletGreenAdapter::getTransaction(quintptr _transactionIndex, CryptoNote::WalletTransaction& _transaction) const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getTransaction(_transactionIndex, _transaction);
}

bool WalletGreenAdapter::getFullTransactionInfo(quintptr _transactionIndex, FullTransactionInfo& _transactionInfo) const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getFullTransactionInfo(_transactionIndex, _transactionInfo);
}

bool WalletGreenAdapter::getAllTransactions(QHash<quintptr, FullTransactionInfo>& _transactionInfos) const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getAllTransactions(_transactionInfos);
}

bool WalletGreenAdapter::getTransactionTransfer(quintptr _transactionIndex, quintptr _transferIndex, CryptoNote::WalletTransfer& _transfer) const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getTransactionTransfer(_transactionIndex, _transferIndex, _transfer);
}

bool WalletGreenAdapter::isFusionTransaction(quintptr _transactionIndex) const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->isFusionTransaction(_transactionIndex);
}

QByteArray WalletGreenAdapter::getUserData() const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getUserData();
}

IWalletAdapter::SendTransactionStatus WalletGreenAdapter::sendTransaction(const CryptoNote::TransactionParameters& _transactionParameters) {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->sendTransaction(_transactionParameters);
}

bool WalletGreenAdapter::createFusionTransaction(quint64 _threshold, quint64 _mixin, const QString& _destinationAddress) {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->createFusionTransaction(_threshold, _mixin, _destinationAddress);
}

quintptr WalletGreenAdapter::getOutputsToOptimizeCount(quint64 _threshold) const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getOutputsToOptimizeCount(_threshold);
}

void WalletGreenAdapter::setUserData(const QByteArray& _userData) {
  Q_ASSERT(m_worker != nullptr);
  m_worker->setUserData(_userData);
}

void WalletGreenAdapter::addObserver(IWalletAdapterObserver* _observer) {
  QObject* observerObject = dynamic_cast<QObject*>(_observer);
  connect(this, SIGNAL(walletOpenedSignal()), observerObject, SLOT(walletOpened()));
  connect(this, SIGNAL(walletOpenErrorSignal(int)), observerObject, SLOT(walletOpenError(int)));
  connect(this, SIGNAL(walletClosedSignal()), observerObject, SLOT(walletClosed()));
  connect(this, SIGNAL(passwordChangedSignal()), observerObject, SLOT(passwordChanged()));
  connect(this, SIGNAL(synchronizationProgressUpdatedSignal(quint32, quint32)), observerObject,
    SLOT(synchronizationProgressUpdated(quint32, quint32)));
  connect(this, SIGNAL(synchronizationCompletedSignal()), observerObject, SLOT(synchronizationCompleted()));
  connect(this, SIGNAL(balanceUpdatedSignal(quint64, quint64)), observerObject, SLOT(balanceUpdated(quint64, quint64)));
  connect(this, SIGNAL(externalTransactionCreatedSignal(quintptr,FullTransactionInfo)),
    observerObject, SLOT(externalTransactionCreated(quintptr,FullTransactionInfo)));
  connect(this, SIGNAL(transactionUpdatedSignal(quintptr,FullTransactionInfo)),
    observerObject, SLOT(transactionUpdated(quintptr,FullTransactionInfo)));
}

void WalletGreenAdapter::removeObserver(IWalletAdapterObserver* _observer) {
  QObject* observerObject = dynamic_cast<QObject*>(_observer);
  disconnect(this, SIGNAL(walletOpenedSignal()), observerObject, SLOT(walletOpened()));
  disconnect(this, SIGNAL(walletOpenErrorSignal(int)), observerObject, SLOT(walletOpenError(int)));
  disconnect(this, SIGNAL(walletClosedSignal()), observerObject, SLOT(walletClosed()));
  disconnect(this, SIGNAL(passwordChangedSignal()), observerObject, SLOT(passwordChanged()));
  disconnect(this, SIGNAL(synchronizationProgressUpdatedSignal(quint32, quint32)), observerObject,
    SLOT(synchronizationProgressUpdated(quint32, quint32)));
  disconnect(this, SIGNAL(synchronizationCompletedSignal()), observerObject, SLOT(synchronizationCompleted()));
  disconnect(this, SIGNAL(balanceUpdatedSignal(quint64, quint64)), observerObject, SLOT(balanceUpdated(quint64, quint64)));
  disconnect(this, SIGNAL(externalTransactionCreatedSignal(quintptr,FullTransactionInfo)),
    observerObject, SLOT(externalTransactionCreated(quintptr,FullTransactionInfo)));
  disconnect(this, SIGNAL(transactionUpdatedSignal(quintptr,FullTransactionInfo)),
    observerObject, SLOT(transactionUpdated(quintptr,FullTransactionInfo)));
}

void WalletGreenAdapter::walletOpened() {
  Q_EMIT walletOpenedSignal();
}

void WalletGreenAdapter::walletOpenError(int _initStatus) {
  Q_EMIT walletOpenErrorSignal(_initStatus);
}

void WalletGreenAdapter::walletClosed() {
  Q_EMIT walletClosedSignal();
}

void WalletGreenAdapter::passwordChanged() {
  Q_EMIT passwordChangedSignal();
}

void WalletGreenAdapter::synchronizationProgressUpdated(quint32 _current, quint32 _total) {
  Q_EMIT synchronizationProgressUpdatedSignal(_current, _total);
}

void WalletGreenAdapter::synchronizationCompleted() {
  Q_EMIT synchronizationCompletedSignal();
}

void WalletGreenAdapter::balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) {
  Q_EMIT balanceUpdatedSignal(_actualBalance, _pendingBalance);
}

void WalletGreenAdapter::externalTransactionCreated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  Q_EMIT externalTransactionCreatedSignal(_transactionId, _transaction);
}

void WalletGreenAdapter::transactionUpdated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  Q_EMIT transactionUpdatedSignal(_transactionId, _transaction);
}

void WalletGreenAdapter::createWorker() {
  Q_ASSERT(!m_workerThread->isRunning());
  Q_ASSERT(m_worker == nullptr);
  m_worker = new WalletGreenWorker(m_currency, m_node, m_logger, nullptr);
  m_worker->addObserver(this);
  QObject* workerObject = dynamic_cast<QObject*>(m_worker);
  workerObject->moveToThread(m_workerThread);
  m_workerThread->start();
}

void WalletGreenAdapter::deleteWorker() {
  Q_ASSERT(m_workerThread->isRunning());
  Q_ASSERT(m_worker != nullptr);
  m_worker->close();
  m_workerThread->quit();
  m_workerThread->wait();
  dynamic_cast<QObject*>(m_worker)->deleteLater();
  m_worker = nullptr;
}

}
