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

#include <QColor>
#include <QEventLoop>
#include <QFont>
#include <QLocale>
#include <QMetaEnum>
#include <QSize>
#include <QThread>

#include <limits>

#include "BlockchainModel.h"
#include "ICryptoNoteAdapter.h"
#include "IBlockChainExplorerAdapter.h"
#include "Models/NodeStateModel.h"

Q_DECLARE_METATYPE(WalletGui::BlockchainModel::Type)

namespace WalletGui {

namespace {

const quint32 INVALID_BLOCK_INDEX = std::numeric_limits<quint32>::max();
const QByteArray NULL_PAYMENT_ID(sizeof(Crypto::Hash), 0);
const int PRELOADED_BLOCK_COUNT = 100;

}

BlockchainModel::BlockchainModel(ICryptoNoteAdapter* _cryptoNoteAdapter, QAbstractItemModel* _nodeStateModel, QObject* _parent) : QAbstractItemModel(_parent),
  m_cryptoNoteAdapter(_cryptoNoteAdapter), m_nodeStateModel(_nodeStateModel),
  m_columnCount(BlockchainModel::staticMetaObject.enumerator(BlockchainModel::staticMetaObject.indexOfEnumerator("Columns")).keyCount()),
  m_lastLocalBlockIndex(INVALID_BLOCK_INDEX) {
  m_cryptoNoteAdapter->addObserver(this);
  if (m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter() != nullptr) {
    m_lastLocalBlockIndex = m_nodeStateModel->index(0, 0).data(NodeStateModel::ROLE_LOCAL_BLOCK_COUNT).value<quint32>() - 1;
  }

  connect(m_nodeStateModel, &QAbstractItemModel::dataChanged, this, &BlockchainModel::nodeStateChanged);
}

BlockchainModel::~BlockchainModel() {
}

Qt::ItemFlags BlockchainModel::flags(const QModelIndex &_index) const {
  return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

int BlockchainModel::columnCount(const QModelIndex& _parent) const {
  return m_columnCount;
}

int BlockchainModel::rowCount(const QModelIndex &_parent) const {
  if(!_parent.isValid()) {
    return m_lastLocalBlockIndex == INVALID_BLOCK_INDEX ? 0 : m_lastLocalBlockIndex + 1;
  }

  return _parent.data(ROLE_BLOCK_TRANSACTION_COUNT).toUInt();
}

QVariant BlockchainModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  if(_orientation != Qt::Horizontal) {
    return QVariant();
  }

  switch(_role) {
  case Qt::DisplayRole:
    switch(_section) {
    case COLUMN_BLOCK_HEIGHT:
      return tr("Height");
    case COLUMN_BLOCK_HASH:
      return tr("Hash");
    case COLUMN_BLOCK_TIME:
      return tr("Time (UTC)");
    case COLUMN_BLOCK_DIFFICULTY:
      return tr("Difficulty");
    case COLUMN_BLOCK_IS_ORPHANED:
      return tr("Orphaned");
    case COLUMN_BLOCK_TRANSACTION_COUNT:
      return tr("Tx count");
    case COLUMN_BLOCK_TRANSACTIONS_SIZE:
      return tr("Transactions size");
    case COLUMN_BLOCK_SIZE:
      return tr("Size");
    case COLUMN_BLOCK_VERSION:
      return tr("Version");
    case COLUMN_BLOCK_TOTAL_COIN_COUNT:
      return tr("Total coins");
    case COLUMN_BLOCK_TOTAL_TRANSACTION_COUNT:
      return tr("Total transactions");
    case COLUMN_BLOCK_BASE_REWARD:
      return tr("Base reward");
    case COLUMN_BLOCK_REWARD_PENALTY:
      return tr("Reward penalty");
    case COLUMN_BLOCK_REWARD:
      return tr("Reward");
    case COLUMN_BLOCK_FEE:
      return tr("Transactions fee");

    case COLUMN_TRANSACTION_HASH:
      return tr("Hash");
    case COLUMN_TRANSACTION_SIZE:
      return tr("Size");
    case COLUMN_TRANSACTION_FEE:
      return tr("Fee");
    case COLUMN_TRANSACTION_INPUT_AMOUNT:
      return tr("Input amount");
    case COLUMN_TRANSACTION_OUTPUT_AMOUNT:
      return tr("Output amount");
    case COLUMN_TRANSACTION_MIXIN:
      return tr("Mixin");
    case COLUMN_TRANSACTION_UNLOCK_TIME:
      return tr("Unlock time (UTC)");
    case COLUMN_TRANSACTION_TIME:
      return tr("Timestamp (UTC)");
    case COLUMN_TRANSACTION_PAYMENT_ID:
      return tr("Payment ID");
    case COLUMN_TRANSACTION_IS_IN_BLOCKCHAIN:
      return tr("In blockchain");
    case COLUMN_TRANSACTION_BLOCK_HASH:
      return tr("Block hash");
    case COLUMN_TRANSACTION_BLOCK_HEIGHT:
      return tr("Block height");
    }

    break;
  case Qt::TextAlignmentRole: {
    if(_section == COLUMN_BLOCK_TIME || _section == COLUMN_TRANSACTION_UNLOCK_TIME || _section == COLUMN_TRANSACTION_TIME) {
      return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
    }

    return static_cast<int>(Qt::AlignRight | Qt::AlignVCenter);
  }

  case ROLE_COLUMN:
    return _section;
  }

  return QVariant();
}

QVariant BlockchainModel::data(const QModelIndex &_index, int _role) const {
  if(!_index.isValid()) {
    return QVariant();
  }

  IBlockChainExplorerAdapter* blockChainExplorerAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter();

  CryptoNote::TransactionDetails tx;
  quint32 blockHeight = _index.internalId() == INVALID_BLOCK_INDEX ? _index.row() : _index.internalId();
  CryptoNote::BlockDetails *block;

  if(_role >= Qt::UserRole && _role != ROLE_BLOCK_HEIGHT && _role != ROLE_ITEM_TYPE) {
    block = blockChainExplorerAdapter->getBlock(blockHeight);
    if (block == nullptr) {
     return QVariant();
    }

    if(_index.internalId() != INVALID_BLOCK_INDEX) {
      tx = block->transactions.at(_index.row());
    }
  }

  switch(_role) {
  case Qt::DisplayRole:
  case Qt::EditRole: {
    switch(_index.column()) {
    case COLUMN_BLOCK_HEIGHT:
      return _index.data(ROLE_BLOCK_HEIGHT);
    case COLUMN_BLOCK_HASH:
      return QString::fromLatin1(_index.data(ROLE_BLOCK_HASH).toByteArray().toHex());
    case COLUMN_BLOCK_TIME: {
      QDateTime time = _index.data(ROLE_BLOCK_TIME).toDateTime();
      if (time.isNull()) {
        return "-";
      }

      return QLocale(QLocale::English).toString(time, "dd MMM yyyy, hh:mm:ss");
    }

    case COLUMN_BLOCK_DIFFICULTY:
      return _index.data(ROLE_BLOCK_DIFFICULTY).value<quint64>();
    case COLUMN_BLOCK_IS_ORPHANED:
      return _index.data(ROLE_BLOCK_IS_ORPHANED).toBool() ? tr("Yes") : tr("No");
    case COLUMN_BLOCK_TRANSACTION_COUNT:
      return _index.data(ROLE_BLOCK_TRANSACTION_COUNT).value<quint64>();
    case COLUMN_BLOCK_TRANSACTIONS_SIZE:
      return _index.data(ROLE_BLOCK_TRANSACTIONS_SIZE).value<quint64>();
    case COLUMN_BLOCK_SIZE:
      return _index.data(ROLE_BLOCK_SIZE).value<quint64>();
    case COLUMN_BLOCK_VERSION:
      return QString("%1.%2").arg(_index.data(ROLE_BLOCK_MAJOR_VERSION).value<quint8>()).arg(_index.data(ROLE_BLOCK_MINOR_VERSION).value<quint8>());
    case COLUMN_BLOCK_TOTAL_COIN_COUNT:
      return m_cryptoNoteAdapter->formatUnsignedAmount(_index.data(ROLE_BLOCK_TOTAL_COIN_COUNT).value<quint64>());
    case COLUMN_BLOCK_TOTAL_TRANSACTION_COUNT:
      return _index.data(ROLE_BLOCK_TOTAL_TRANSACTION_COUNT).value<quint64>();
    case COLUMN_BLOCK_BASE_REWARD:
      return m_cryptoNoteAdapter->formatUnsignedAmount(_index.data(ROLE_BLOCK_BASE_REWARD).value<quint64>());
    case COLUMN_BLOCK_REWARD_PENALTY: {
      qreal penalty = _index.data(ROLE_BLOCK_REWARD_PENALTY).value<qreal>() * 100;
      return penalty == 0 ? tr("No") : QLocale(QLocale::English).toString(penalty, 'f').append(" %");
    }

    case COLUMN_BLOCK_REWARD:
      return m_cryptoNoteAdapter->formatUnsignedAmount(_index.data(ROLE_BLOCK_REWARD).value<quint64>());
    case COLUMN_BLOCK_FEE:
      return m_cryptoNoteAdapter->formatUnsignedAmount(_index.data(ROLE_BLOCK_FEE).value<quint64>());
    case COLUMN_TRANSACTION_HASH:
      return QString::fromLatin1(_index.data(ROLE_TRANSACTION_HASH).toByteArray().toHex());
    case COLUMN_TRANSACTION_SIZE:
      return _index.data(ROLE_TRANSACTION_SIZE).value<quint64>();
    case COLUMN_TRANSACTION_FEE:
      return m_cryptoNoteAdapter->formatUnsignedAmount(_index.data(ROLE_TRANSACTION_FEE).value<quint64>());
    case COLUMN_TRANSACTION_INPUT_AMOUNT:
      return m_cryptoNoteAdapter->formatUnsignedAmount(_index.data(ROLE_TRANSACTION_INPUT_AMOUNT).value<quint64>());
    case COLUMN_TRANSACTION_OUTPUT_AMOUNT:
      return m_cryptoNoteAdapter->formatUnsignedAmount(_index.data(ROLE_TRANSACTION_OUTPUT_AMOUNT).value<quint64>());
    case COLUMN_TRANSACTION_MIXIN:
      return _index.data(ROLE_TRANSACTION_MIXIN);
    case COLUMN_TRANSACTION_UNLOCK_TIME: {
      QDateTime time = _index.data(ROLE_TRANSACTION_UNLOCK_TIME).toDateTime();
      return QLocale(QLocale::English).toString(time, "dd MMM yyyy, hh:mm:ss");
    }
    case COLUMN_TRANSACTION_TIME: {
      QDateTime time = _index.data(ROLE_TRANSACTION_UNLOCK_TIME).toDateTime();
      return QLocale(QLocale::English).toString(time, "dd MMM yyyy, hh:mm:ss");
    }
    case COLUMN_TRANSACTION_PAYMENT_ID: {
      QByteArray payment_id = _index.data(ROLE_TRANSACTION_PAYMENT_ID).toByteArray();
      return payment_id == NULL_PAYMENT_ID ? QByteArray() : QString::fromLatin1(payment_id.toHex());
    }
    case COLUMN_TRANSACTION_IS_IN_BLOCKCHAIN:
      return _index.data(ROLE_TRANSACTION_IS_IN_BLOCKCHAIN);
    case COLUMN_TRANSACTION_BLOCK_HASH:
      return QString::fromLatin1(_index.data(ROLE_TRANSACTION_BLOCK_HASH).toByteArray().toHex());
    case COLUMN_TRANSACTION_BLOCK_HEIGHT:
      return _index.data(ROLE_TRANSACTION_BLOCK_HEIGHT).value<quint32>();
    }

    break;
  }

  case Qt::ForegroundRole:
    return _index.column() == COLUMN_BLOCK_HASH || _index.column() == COLUMN_BLOCK_HEIGHT ? QColor("#0580e8") : QVariant();
  case Qt::TextAlignmentRole:
    return headerData(_index.column(), Qt::Horizontal, _role);
  case ROLE_ITEM_TYPE:
    return _index.internalId() != INVALID_BLOCK_INDEX ? TYPE_TRANSACTION : TYPE_BLOCK;
  case ROLE_BLOCK_HEIGHT:
    return blockHeight;
  case ROLE_BLOCK_HASH:
    return QByteArray(reinterpret_cast<char*>(&block->hash), sizeof(block->hash));
  case ROLE_BLOCK_TIME:
    if (block->timestamp == 0) {
      return QDateTime();
    }

    return QDateTime::fromTime_t(block -> timestamp, Qt::UTC);
  case ROLE_BLOCK_DIFFICULTY:
    return static_cast<quint64>(block -> difficulty);
  case ROLE_BLOCK_IS_ORPHANED:
    return block->isAlternative;
  case ROLE_BLOCK_TRANSACTION_COUNT:
    return static_cast<quint64>(block -> transactions.size());
  case ROLE_BLOCK_TRANSACTIONS_SIZE:
    return static_cast<quint64>(block -> transactionsCumulativeSize);
  case ROLE_BLOCK_SIZE:
    return static_cast<quint64>(block -> blockSize);
  case ROLE_BLOCK_MAJOR_VERSION:
    return block -> majorVersion;
  case ROLE_BLOCK_MINOR_VERSION:
    return block -> minorVersion;
  case ROLE_BLOCK_TOTAL_COIN_COUNT:
    return static_cast<quint64>(block -> alreadyGeneratedCoins);
  case ROLE_BLOCK_TOTAL_TRANSACTION_COUNT:
    return static_cast<quint64>(block -> alreadyGeneratedTransactions);
  case ROLE_BLOCK_BASE_REWARD:
    return static_cast<quint64>(block -> baseReward);
  case ROLE_BLOCK_REWARD_PENALTY:
    return block -> penalty;
  case ROLE_BLOCK_REWARD:
    return static_cast<quint64>(block -> reward);
  case ROLE_BLOCK_FEE:
    return static_cast<quint64>(block -> totalFeeAmount);
  case ROLE_COLUMN:
    return headerData(_index.column(), Qt::Horizontal, ROLE_COLUMN);

  case ROLE_TRANSACTION_HASH:
    return QByteArray(reinterpret_cast<char*>(&tx.hash), sizeof(tx.hash));
  case ROLE_TRANSACTION_SIZE:
    return static_cast<quint64>(tx.size);
  case ROLE_TRANSACTION_FEE:
    return static_cast<quint64>(tx.fee);
  case ROLE_TRANSACTION_INPUT_AMOUNT:
    return static_cast<quint64>(tx.totalInputsAmount);
  case ROLE_TRANSACTION_OUTPUT_AMOUNT:
    return static_cast<quint64>(tx.totalOutputsAmount);
  case ROLE_TRANSACTION_MIXIN:
    return static_cast<quint64>(tx.mixin);
  case ROLE_TRANSACTION_UNLOCK_TIME:
    return QDateTime::fromTime_t(tx.unlockTime, Qt::UTC);
  case ROLE_TRANSACTION_TIME:
    return QDateTime::fromTime_t(tx.timestamp, Qt::UTC);
  case ROLE_TRANSACTION_PAYMENT_ID:
    return QByteArray(reinterpret_cast<char*>(&tx.paymentId), sizeof(tx.paymentId));
  case ROLE_TRANSACTION_IS_IN_BLOCKCHAIN:
    return tx.inBlockchain;
  case ROLE_TRANSACTION_BLOCK_HASH:
    return QByteArray(reinterpret_cast<char*>(&tx.blockHash), sizeof(tx.blockHash));
  case ROLE_TRANSACTION_BLOCK_HEIGHT:
    return static_cast<quint32>(tx.blockIndex);
  default:
    break;
  }

  return QVariant();
}

QModelIndex BlockchainModel::index(int _row, int _column, const QModelIndex &_parent) const {
  if(!_parent.isValid()) {
    return _row > m_lastLocalBlockIndex ? QModelIndex() : createIndex(_row, _column, INVALID_BLOCK_INDEX);
  }

  return _row >= _parent.data(ROLE_BLOCK_TRANSACTION_COUNT).toUInt() ? QModelIndex() :
    createIndex(_row, _column, _parent.data(ROLE_BLOCK_HEIGHT).value<quint32>());
}

QModelIndex	BlockchainModel::parent(const QModelIndex &_index) const {
  if(!_index.isValid() || _index.internalId() == INVALID_BLOCK_INDEX) {
    return QModelIndex();
  }

  return createIndex(_index.internalId(), 0, INVALID_BLOCK_INDEX);
}

void BlockchainModel::cryptoNoteAdapterInitCompleted(int _status) {
  if (_status == 0) {
    if (m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter() == nullptr) {
      beginResetModel();
      m_lastLocalBlockIndex = INVALID_BLOCK_INDEX;
      endResetModel();
      return;
    }

    beginResetModel();
    m_lastLocalBlockIndex = m_nodeStateModel->index(0, 0).data(NodeStateModel::ROLE_LOCAL_BLOCK_COUNT).value<quint32>() - 1;
    if (m_lastLocalBlockIndex < PRELOADED_BLOCK_COUNT) {
      m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter()->preloadBlocks(0, m_lastLocalBlockIndex);
    } else {
      m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter()->preloadBlocks(m_lastLocalBlockIndex - PRELOADED_BLOCK_COUNT, m_lastLocalBlockIndex);
    }
    endResetModel();
  }
}

void BlockchainModel::cryptoNoteAdapterDeinitCompleted() {
  // Do nothing
}

void BlockchainModel::nodeStateChanged(const QModelIndex& _topLeft, const QModelIndex& _bottomRight, const QVector<int>& _roles) {
  if (_roles.contains(NodeStateModel::ROLE_LOCAL_BLOCK_COUNT) && rowCount() > 0) {
    quint32 lastLocalBlockIndex = m_nodeStateModel->index(0, 0).data(NodeStateModel::ROLE_LOCAL_BLOCK_COUNT).value<quint32>() - 1;
    if (m_lastLocalBlockIndex == INVALID_BLOCK_INDEX) {
      beginResetModel();
      m_lastLocalBlockIndex = lastLocalBlockIndex;
      endResetModel();
      return;
    }

    if(lastLocalBlockIndex > m_lastLocalBlockIndex) {
      beginInsertRows(QModelIndex(), m_lastLocalBlockIndex + 1, lastLocalBlockIndex);
      m_lastLocalBlockIndex = lastLocalBlockIndex;
      endInsertRows();
    }
  }
}

}
