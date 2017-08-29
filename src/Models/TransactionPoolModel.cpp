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

#include <QDateTime>
#include <QLocale>
#include <QMetaEnum>

#include "TransactionPoolModel.h"
#include "ICryptoNoteAdapter.h"
#include "INodeAdapter.h"

#include <BlockchainExplorerData.h>

namespace WalletGui {

namespace {

const QByteArray NULL_PAYMENT_ID(sizeof(Crypto::Hash), 0);

}

TransactionPoolModel::TransactionPoolModel(ICryptoNoteAdapter* _cryptoNoteAdapter, QObject* _parent) :
  QAbstractItemModel(_parent), m_cryptoNoteAdapter(_cryptoNoteAdapter), m_transactions(),
  m_isSynchronized(false),
  m_columnCount(TransactionPoolModel::staticMetaObject.enumerator(TransactionPoolModel::staticMetaObject.indexOfEnumerator("Columns")).keyCount()) {
  m_cryptoNoteAdapter->addObserver(this);
  if (m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter() != nullptr) {
    m_isSynchronized = m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter()->isBlockchainSynchronized();
    m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter()->addObserver(this);
    if (m_isSynchronized) {
      QVector<CryptoNote::TransactionDetails> transactions;
      if (m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter()->getPoolState(transactions) ==
        IBlockChainExplorerAdapter::POOL_SUCCESS) {
        m_rowHashes.clear();
        m_transactions.clear();
        for (const auto& transaction : transactions) {
          QByteArray hash(reinterpret_cast<const char*>(&transaction.hash), sizeof(transaction.hash));
          m_rowHashes.prepend(hash);
          m_transactions[hash] = transaction;
        }
      } else {
        m_isSynchronized = false;
      }
    }
  }
}

TransactionPoolModel::~TransactionPoolModel() {
}

Qt::ItemFlags TransactionPoolModel::flags(const QModelIndex &_index) const {
  return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsSelectable;
}

int TransactionPoolModel::columnCount(const QModelIndex &_parent) const {
  return m_columnCount;
}

int TransactionPoolModel::rowCount(const QModelIndex &_parent) const {
  return m_rowHashes.size();
}

QVariant TransactionPoolModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  if (_orientation != Qt::Horizontal) {
    return QVariant();
  }

  switch (_role) {
  case Qt::DisplayRole: {
    switch (_section) {
    case COLUMN_TIME:
      return tr("Time (UTC)");
    case COLUMN_AMOUNT:
      return tr("Amount");
    case COLUMN_FEE:
      return tr("Fee");
    case COLUMN_SIZE:
      return tr("Size (bytes)");
    case COLUMN_HASH:
      return tr("Hash");
    case COLUMN_MIXIN:
      return tr("Mixin");
    case COLUMN_PAYMENT_ID:
      return tr("Payment ID");
    }
  }

  case Qt::TextAlignmentRole: {
    if (_section == COLUMN_TIME) {
      return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
    }

    return static_cast<int>(Qt::AlignRight | Qt::AlignVCenter);
  }

  case ROLE_COLUMN:
    return _section;
  }

  return QVariant();
}

QVariant TransactionPoolModel::data(const QModelIndex &_index, int _role) const {
  if (!_index.isValid()) {
    return QVariant();
  }

  switch (_role) {
  case Qt::DisplayRole:
  case Qt::EditRole:
    return getDisplayRole(_index);

  case Qt::TextAlignmentRole:
    return headerData(_index.column(), Qt::Horizontal, _role);

  default:
    return getUserRole(_index, _role);
  }

  return QVariant();
}

QModelIndex TransactionPoolModel::index(int _row, int _column, const QModelIndex &_parent) const {
  if (_parent.isValid()) {
    return QModelIndex();
  }

  return createIndex(_row, _column, _row);
}

QModelIndex	TransactionPoolModel::parent(const QModelIndex &_index) const {
  return QModelIndex();
}

void TransactionPoolModel::blockChainUpdated(const QVector<CryptoNote::BlockDetails>& _newBlocks,
  const QVector<CryptoNote::BlockDetails>& _orphanedBlocks) {
  // Do nothing
}

void TransactionPoolModel::blockchainSynchronized(const CryptoNote::BlockDetails& _topBlock) {
  if (!m_isSynchronized) {
    m_isSynchronized = true;
    QVector<CryptoNote::TransactionDetails> transactions;
    if (m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter()->getPoolState(transactions) ==
      IBlockChainExplorerAdapter::POOL_SUCCESS) {
      m_rowHashes.clear();
      m_transactions.clear();
      for (const auto& transaction : transactions) {
        QByteArray hash(reinterpret_cast<const char*>(&transaction.hash), sizeof(transaction.hash));
        m_rowHashes.prepend(hash);
        m_transactions[hash] = transaction;
      }
    } else {
      m_isSynchronized = false;
    }
  }
}

void TransactionPoolModel::cryptoNoteAdapterInitCompleted(int _status) {
  if (_status == 0) {
    beginResetModel();
    m_rowHashes.clear();
    m_transactions.clear();
    if (m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter() != nullptr) {
      m_isSynchronized = m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter()->isBlockchainSynchronized();
      m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter()->addObserver(this);
      if (m_isSynchronized) {
        QVector<CryptoNote::TransactionDetails> transactions;
        if (m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter()->getPoolState(transactions) ==
          IBlockChainExplorerAdapter::POOL_SUCCESS) {
          for (const auto& transaction : transactions) {
            QByteArray hash(reinterpret_cast<const char*>(&transaction.hash), sizeof(transaction.hash));
            m_rowHashes.prepend(hash);
            m_transactions[hash] = transaction;
          }
        } else {
          m_isSynchronized = false;
        }
      }
    }

    endResetModel();
  }
}

void TransactionPoolModel::cryptoNoteAdapterDeinitCompleted() {
  // Do nothing
}

int TransactionPoolModel::findProxyColumn(QAbstractItemModel* _proxyModel, int _originColumn) {
  for (int i = 0; i < _proxyModel->columnCount(); ++i) {
    if (_proxyModel->headerData(i, Qt::Horizontal, ROLE_COLUMN).toInt() == _originColumn) {
      return i;
    }
  }

  return -1;
}

void TransactionPoolModel::poolUpdated(const QVector<CryptoNote::TransactionDetails>& _newTransactions,
  const QVector<Crypto::Hash>& _removedTransactions) {
  beginResetModel();
  for (const auto& transaction : _newTransactions) {
    QByteArray hash(reinterpret_cast<const char*>(&transaction.hash), sizeof(transaction.hash));
    if (m_transactions.contains(hash)) {
      continue;
    }

    m_rowHashes.prepend(hash);
    m_transactions[hash] = transaction;
  }

  for (const Crypto::Hash& cryptoHash : _removedTransactions) {
    QByteArray hash(reinterpret_cast<const char*>(&cryptoHash), sizeof(cryptoHash));
    if (!m_transactions.contains(hash)) {
      continue;
    }

    m_rowHashes.removeAll(hash);
    m_transactions.remove(hash);
  }

  endResetModel();
}

QVariant TransactionPoolModel::getDisplayRole(const QModelIndex& _index) const {
  switch (_index.column()) {
  case COLUMN_TIME: {
    QDateTime time = _index.data(ROLE_TIME).toDateTime();
    if (time.isNull()) {
      return "-";
    }

    return QLocale(QLocale::English).toString(time, "dd MMM yyyy, hh:mm:ss");
  }

  case COLUMN_AMOUNT:
    return m_cryptoNoteAdapter->formatUnsignedAmount(_index.data(ROLE_AMOUNT).value<quint64>());
  case COLUMN_FEE:
    return m_cryptoNoteAdapter->formatUnsignedAmount(_index.data(ROLE_FEE).value<quint64>());
  case COLUMN_SIZE:
    return _index.data(ROLE_SIZE);
  case COLUMN_HASH:
    return _index.data(ROLE_HASH).toByteArray().toHex();
  case COLUMN_MIXIN:
    return QString::number(_index.data(ROLE_MIXIN).value<quint64>());
  case COLUMN_PAYMENT_ID: {
    QByteArray paymentId = _index.data(ROLE_PAYMENT_ID).toByteArray();
    if (paymentId != NULL_PAYMENT_ID) {
      return paymentId.toHex();
    }

    return QString();
  }

  default:
    break;
  }

  return QVariant();
}

QVariant TransactionPoolModel::getUserRole(const QModelIndex& _index, int _role) const {
  CryptoNote::TransactionDetails transaction = m_transactions[m_rowHashes[_index.row()]];
  switch (_role) {
  case ROLE_TIME:
    if (transaction.timestamp == 0) {
      return QDateTime();
    }

    return QDateTime::fromTime_t(transaction.timestamp, Qt::UTC);
  case ROLE_AMOUNT:
    return static_cast<quint64>(transaction.totalOutputsAmount);
  case ROLE_FEE:
    return static_cast<quint64>(transaction.fee);
  case ROLE_SIZE:
    return static_cast<quint64>(transaction.size);
  case ROLE_HASH:
    return QByteArray(reinterpret_cast<char*>(&transaction.hash), sizeof(transaction.hash));
  case ROLE_MIXIN:
    return static_cast<quint64>(transaction.mixin);
  case ROLE_PAYMENT_ID:
    return QByteArray(reinterpret_cast<char*>(&transaction.paymentId), sizeof(transaction.paymentId));
  case ROLE_COLUMN:
    return headerData(_index.column(), Qt::Horizontal, ROLE_COLUMN);
  default:
    break;
  }

  return QVariant();
}

}
