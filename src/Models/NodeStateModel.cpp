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

#include "NodeStateModel.h"

namespace WalletGui {

namespace {

const quint32 INVALID_BLOCK_COUNT = std::numeric_limits<quint32>::max();
const quint64 INVALID_DIFFICULTY = std::numeric_limits<quint64>::max();
const QStringList RATE_PREFIXES = {"", "k", "M", "G", "T", "P", "E", "Z", "Y"};

}

NodeStateModel::NodeStateModel(ICryptoNoteAdapter* _cryptoNoteAdapter, QObject* _parent) : QAbstractItemModel(_parent),
  m_cryptoNoteAdapter(_cryptoNoteAdapter), m_columnCount(NodeStateModel::staticMetaObject.enumerator(NodeStateModel::staticMetaObject.indexOfEnumerator("Columns")).keyCount()),
  m_nodeType(NodeType::UNKNOWN), m_isConnected(true) {
  INodeAdapter* nodeAdapter = m_cryptoNoteAdapter->getNodeAdapter();
  m_cryptoNoteAdapter->addObserver(this);
  nodeAdapter->addObserver(this);
  m_nodeType = nodeAdapter->getNodeType();
  m_peerCount = nodeAdapter->getPeerCount();
  m_knownBlockCount = nodeAdapter->getKnownBlockCount();
  m_lastLocalBlockInfo = nodeAdapter->getLastLocalBlockInfo();
  if (m_knownBlockCount == 0) {
    m_knownBlockCount = m_lastLocalBlockInfo.index + 1;
  }
}

NodeStateModel::~NodeStateModel() {
}

Qt::ItemFlags NodeStateModel::flags(const QModelIndex& _index) const {
  return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
}

int NodeStateModel::columnCount(const QModelIndex& _parent) const {
  return m_columnCount;
}

int NodeStateModel::rowCount(const QModelIndex& _parent) const {
  return 1;
}

QVariant NodeStateModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  return QVariant();
}

QVariant NodeStateModel::data(const QModelIndex& _index, int _role) const {
  if (!_index.isValid()) {
    return QVariant();
  }

  switch (_role) {
  case Qt::DisplayRole:
  case Qt::EditRole:
    return getDisplayRole(_index);
  default:
    return getUserRole(_role);
  }

  return QVariant();
}

QModelIndex NodeStateModel::index(int _row, int _column, const QModelIndex& _parent) const {
  if (!hasIndex(_row, _column, _parent)) {
    return QModelIndex();
  }

  return createIndex(_row, _column);
}

QModelIndex NodeStateModel::parent(const QModelIndex& _index) const {
  return QModelIndex();
}

void NodeStateModel::initCompleted(int _status) {
  // Do nothing
}

void NodeStateModel::deinitCompleted() {
  // Do nothing
}

void NodeStateModel::peerCountUpdated(quintptr _count) {
  if (m_peerCount != _count) {
    m_peerCount = _count;
    Q_EMIT dataChanged(index(0, COLUMN_PEER_COUNT), index(0, COLUMN_PEER_COUNT), QVector<int>() << Qt::EditRole << ROLE_PEER_COUNT);
  }
}

void NodeStateModel::localBlockchainUpdated(CryptoNote::BlockHeaderInfo _lastLocalBlockInfo) {
  if (std::memcmp(&_lastLocalBlockInfo.hash, &m_lastLocalBlockInfo.hash, sizeof(m_lastLocalBlockInfo.hash)) != 0) {
    m_lastLocalBlockInfo = _lastLocalBlockInfo;
    Q_EMIT dataChanged(index(0, 0), index(0, m_columnCount - 1), QVector<int>() << Qt::EditRole << ROLE_LOCAL_BLOCK_COUNT <<
      ROLE_LAST_LOCAL_BLOCK_TIMESTAMP << ROLE_LAST_LOCAL_BLOCK_DIFFICULTY << ROLE_NETWORK_HASHRATE);
  }
}

void NodeStateModel::lastKnownBlockHeightUpdated(quint32 _height) {
  quint32 knownBlockCount = _height + 1;
  if (m_knownBlockCount != knownBlockCount) {
    m_knownBlockCount = knownBlockCount;
    Q_EMIT dataChanged(index(0, COLUMN_KNOWN_BLOCK_COUNT), index(0, COLUMN_KNOWN_BLOCK_COUNT),
      QVector<int>() << Qt::EditRole << ROLE_KNOWN_BLOCK_COUNT);
  }
}

void NodeStateModel::connectionStatusUpdated(bool _connected) {
  m_isConnected = _connected;
}

void NodeStateModel::cryptoNoteAdapterInitCompleted(int _status) {
  if (_status == 0) {
    beginResetModel();
    INodeAdapter* nodeAdapter = m_cryptoNoteAdapter->getNodeAdapter();
    nodeAdapter->addObserver(this);
    m_nodeType = nodeAdapter->getNodeType();
    m_peerCount = nodeAdapter->getPeerCount();
    m_lastLocalBlockInfo = nodeAdapter->getLastLocalBlockInfo();
    m_knownBlockCount = nodeAdapter->getKnownBlockCount();
    endResetModel();

    Q_EMIT dataChanged(index(0, 0), index(0, m_columnCount - 1));
  }
}

void NodeStateModel::cryptoNoteAdapterDeinitCompleted() {
  beginResetModel();
  reset();
  endResetModel();
  Q_EMIT dataChanged(index(0, 0), index(0, m_columnCount - 1));
}

QVariant NodeStateModel::getDisplayRole(const QModelIndex& _index) const {
  switch (_index.column()) {
  case COLUMN_NODE_TYPE:
    return _index.data(ROLE_NODE_TYPE);
  case COLUMN_PEER_COUNT: {
    if (m_peerCount == 0) {
      return tr("Disconnected");
    } else if (m_peerCount == 1) {
      return tr("1 peer");
    } else {
      return tr("%1 peers").arg(m_peerCount);
    }
  }

  case COLUMN_LOCAL_BLOCK_COUNT:
    return QString::number(m_lastLocalBlockInfo.index + 1);
  case COLUMN_KNOWN_BLOCK_COUNT:
    return QString::number(m_knownBlockCount);
  case COLUMN_LAST_LOCAL_BLOCK_TIMESTAMP:
    if (m_lastLocalBlockInfo.timestamp == 0) {
      return tr("Unknown");
    }

    return QLocale(QLocale::English).toString(QDateTime::fromTime_t(m_lastLocalBlockInfo.timestamp).toUTC(), "dd MMM yyyy, HH:mm:ss");
  case COLUMN_LAST_LOCAL_BLOCK_DIFFICULTY:
    return QString::number(m_lastLocalBlockInfo.difficulty);
  case COLUMN_NETWORK_HASHRATE:
    return formatHashRate(m_lastLocalBlockInfo.difficulty / m_cryptoNoteAdapter->getTargetTime());
  }

  return QVariant();
}

QVariant NodeStateModel::getUserRole(int _role) const {
  INodeAdapter* nodeAdapter = m_cryptoNoteAdapter->getNodeAdapter();
  if (nodeAdapter == nullptr) {
    return QVariant();
  }

  switch (_role) {
  case ROLE_NODE_TYPE:
    return static_cast<int>(m_nodeType);
  case ROLE_PEER_COUNT:
    return m_peerCount;
  case ROLE_LOCAL_BLOCK_COUNT:
    return m_lastLocalBlockInfo.index + 1;
  case ROLE_KNOWN_BLOCK_COUNT:
    return m_knownBlockCount;
  case ROLE_LAST_LOCAL_BLOCK_TIMESTAMP:
    return static_cast<quint64>(m_lastLocalBlockInfo.timestamp);
  case ROLE_LAST_LOCAL_BLOCK_DIFFICULTY:
    return static_cast<quint64>(m_lastLocalBlockInfo.difficulty);
  case ROLE_NETWORK_HASHRATE:
    return m_lastLocalBlockInfo.difficulty / m_cryptoNoteAdapter->getTargetTime();
  case ROLE_CONNECTION_STATE:
    return QVariant();
  }

  return QVariant();
}

void NodeStateModel::reset() {
  m_nodeType = NodeType::UNKNOWN;
  m_peerCount = 0;
  m_knownBlockCount = INVALID_BLOCK_COUNT;
  std::memset(&m_lastLocalBlockInfo, 0, sizeof(m_lastLocalBlockInfo));
}

QString NodeStateModel::formatHashRate(quint64 _hashRate) {
  quint64 intPart = _hashRate;
  quint64 decimalPart = 0;
  int i = 0;
  while (_hashRate >= 1000 && i < RATE_PREFIXES.size()) {
    ++i;
    intPart = _hashRate / 1000;
    decimalPart = _hashRate % 1000;
    _hashRate /= 1000;
  }

  if (decimalPart > 0) {
    return QString("%1.%2 %3H/s").arg(intPart).arg(decimalPart, 3, 10, QChar('0')).arg(RATE_PREFIXES[i]);
  }

  return QString("%1 %2H/s").arg(intPart).arg(RATE_PREFIXES[i]);
}

}
