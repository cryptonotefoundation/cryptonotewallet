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

#include <QMetaEnum>

#include "WalletStateModel.h"
#include "ICryptoNoteAdapter.h"
#include "INodeAdapter.h"

namespace WalletGui {

namespace {

const quint32 SYNC_DISTANCE = 5;

}

WalletStateModel::WalletStateModel(ICryptoNoteAdapter* m_cryptoNoteAdapter, QObject* _parent) : QAbstractItemModel(_parent),
  m_cryptoNoteAdapter(m_cryptoNoteAdapter), m_isOpen(m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->isOpen()),
  m_isEncrypted(m_isOpen ? m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->isEncrypted() : false),
  m_address(m_isOpen ? m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->getAddress(0) : ""),
  m_actualBalance(0), m_pendingBalance(0), m_totalBalance(0), m_currentHeight(0), m_totalHeight(0), m_isSynchronized(false),
  m_columnCount(WalletStateModel::staticMetaObject.enumerator(WalletStateModel::staticMetaObject.indexOfEnumerator("Columns")).keyCount()) {
  m_cryptoNoteAdapter->addObserver(this);
  m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
}

WalletStateModel::~WalletStateModel() {
}

Qt::ItemFlags WalletStateModel::flags(const QModelIndex& _index) const {
  return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
}

int WalletStateModel::columnCount(const QModelIndex& _parent) const {
  return m_columnCount;
}

int WalletStateModel::rowCount(const QModelIndex& _parent) const {
  return 1;
}

QVariant WalletStateModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  if (_role != Qt::DisplayRole) {
    return QVariant();
  }

  switch (_section) {
  case COLUMN_IS_OPEN:
    return tr("Open");
  case COLUMN_IS_CLOSED:
    return tr("Closed");
  case COLUMN_IS_ENCRYPTED:
    return tr("Encrypted");
  case COLUMN_IS_NOT_ENCRYPTED:
    return tr("Not encrypted");
  case COLUMN_ADDRESS:
    return tr("Address");
  case COLUMN_ACTUAL_BALANCE:
    return tr("Actual balance");
  case COLUMN_PENDING_BALANCE:
    return tr("Locked amount");
  case COLUMN_TOTAL_BALANCE:
    return tr("Total balance");
  }

  return QVariant();
}

QVariant WalletStateModel::data(const QModelIndex& _index, int _role) const {
  if (!_index.isValid()) {
    return QVariant();
  }

  switch (_role) {
  case Qt::DisplayRole:
  case Qt::EditRole:
    return getDisplayRole(_index);

  default:
    return getUserRoleRole(_index, _role);
  }


  return QVariant();
}

QModelIndex WalletStateModel::index(int _row, int _column, const QModelIndex& _parent) const {
  if (!hasIndex(_row, _column, _parent)) {
    return QModelIndex();
  }

  return createIndex(_row, _column, _row);
}

QModelIndex WalletStateModel::parent(const QModelIndex& _index) const {
  return QModelIndex();
}

void WalletStateModel::walletOpened() {
  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  m_isOpen = true;
  m_isEncrypted = walletAdapter->isEncrypted();
  m_address = walletAdapter->getAddress(0);
  m_actualBalance = walletAdapter->getActualBalance();
  m_pendingBalance = walletAdapter->getPendingBalance();
  m_totalBalance = m_actualBalance + m_pendingBalance;
  Q_EMIT dataChanged(index(0, 0), index(0, columnCount() - 1));
  Q_EMIT dataChanged(index(0, COLUMN_IS_SYNCHRONIZED), index(0, COLUMN_IS_SYNCHRONIZED));
  Q_EMIT dataChanged(index(0, COLUMN_ABOUT_TO_BE_SYNCHRONIZED), index(0, COLUMN_ABOUT_TO_BE_SYNCHRONIZED));
}

void WalletStateModel::walletOpenError(int _initStatus) {
  // Do nothing
}

void WalletStateModel::walletClosed() {
  m_isOpen = false;
  m_isEncrypted = false;
  m_address = "";
  m_actualBalance = 0;
  m_pendingBalance = 0;
  m_totalBalance = 0;
  m_currentHeight = 0;
  m_totalHeight = 0;
  m_isSynchronized = false;
  Q_EMIT dataChanged(index(0, 0), index(0, columnCount() - 1));
}

void WalletStateModel::passwordChanged() {
  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  m_isEncrypted = walletAdapter->isEncrypted();
  Q_EMIT dataChanged(index(0, COLUMN_IS_ENCRYPTED), index(0, COLUMN_IS_ENCRYPTED));
}

void WalletStateModel::synchronizationProgressUpdated(quint32 _current, quint32 _total) {
  m_currentHeight = _current;
  m_totalHeight = _total;
  if (m_isSynchronized) {
    m_isSynchronized = false;
    Q_EMIT dataChanged(index(0, COLUMN_IS_SYNCHRONIZED), index(0, COLUMN_IS_SYNCHRONIZED));
  }

  if (_total - _current == SYNC_DISTANCE) {
    Q_EMIT dataChanged(index(0, COLUMN_ABOUT_TO_BE_SYNCHRONIZED), index(0, COLUMN_ABOUT_TO_BE_SYNCHRONIZED));
  }
}

void WalletStateModel::synchronizationCompleted() {
  m_isSynchronized = (m_currentHeight == m_totalHeight);
  Q_EMIT dataChanged(index(0, COLUMN_ABOUT_TO_BE_SYNCHRONIZED), index(0, COLUMN_ABOUT_TO_BE_SYNCHRONIZED));
  Q_EMIT dataChanged(index(0, COLUMN_IS_SYNCHRONIZED), index(0, COLUMN_IS_SYNCHRONIZED));
}

void WalletStateModel::balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) {
  m_actualBalance = _actualBalance;
  m_pendingBalance = _pendingBalance;
  m_totalBalance = m_actualBalance + m_pendingBalance;
  Q_EMIT dataChanged(index(0, 0), index(0, columnCount() - 1));
}

void WalletStateModel::externalTransactionCreated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void WalletStateModel::transactionUpdated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void WalletStateModel::cryptoNoteAdapterInitCompleted(int _status) {
  if (_status == 0) {
    m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
  }
}

void WalletStateModel::cryptoNoteAdapterDeinitCompleted() {
  // Do nothing
}

QVariant WalletStateModel::getDisplayRole(const QModelIndex& _index) const {
  switch (_index.column()) {
  case COLUMN_IS_OPEN:
    return _index.data(ROLE_IS_OPEN);
  case COLUMN_IS_CLOSED:
    return !_index.data(ROLE_IS_OPEN).toBool();
  case COLUMN_IS_ENCRYPTED:
    return _index.data(ROLE_IS_ENCRYPTED);
  case COLUMN_IS_NOT_ENCRYPTED:
    return !_index.data(ROLE_IS_ENCRYPTED).toBool();
  case COLUMN_ADDRESS:
    return _index.data(ROLE_ADDRESS);
  case COLUMN_ACTUAL_BALANCE:
    return m_cryptoNoteAdapter->formatUnsignedAmount(_index.data(ROLE_ACTUAL_BALANCE).value<quint64>());
  case COLUMN_PENDING_BALANCE:
    return m_cryptoNoteAdapter->formatUnsignedAmount(_index.data(ROLE_PENDING_BALANCE).value<quint64>());
  case COLUMN_TOTAL_BALANCE:
    return m_cryptoNoteAdapter->formatUnsignedAmount(_index.data(ROLE_TOTAL_BALANCE).value<quint64>());
  case COLUMN_IS_SYNCHRONIZED:
    return _index.data(ROLE_IS_SYNCHRONIZED);
  case COLUMN_IS_NOT_SYNCHRONIZED:
    return !_index.data(ROLE_IS_SYNCHRONIZED).toBool();
  case COLUMN_ABOUT_TO_BE_SYNCHRONIZED:
    return _index.data(ROLE_ABOUT_TO_BE_SYNCHRONIZED);
  }

  return QVariant();
}

QVariant WalletStateModel::getUserRoleRole(const QModelIndex& _index, int _role) const {
  switch (_role) {
  case ROLE_IS_OPEN:
    return m_isOpen;
  case ROLE_IS_ENCRYPTED:
    return m_isEncrypted;
  case ROLE_ADDRESS:
    return m_address;
  case ROLE_ACTUAL_BALANCE:
    return m_actualBalance;
  case ROLE_PENDING_BALANCE:
    return m_pendingBalance;
  case ROLE_TOTAL_BALANCE:
    return m_totalBalance;
  case ROLE_IS_SYNCHRONIZED:
    return m_isSynchronized;
  case ROLE_ABOUT_TO_BE_SYNCHRONIZED:
    return m_isSynchronized || (m_totalHeight > 0 && (m_totalHeight - m_currentHeight < 5 || m_totalHeight < m_currentHeight));
  }

  return QVariant();
}

}
