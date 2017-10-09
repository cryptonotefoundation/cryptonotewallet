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
#include <QDateTime>
#include <QFont>
#include <QMetaEnum>
#include <QPixmap>
#include <QTimerEvent>
#include <QUrl>
#include <QtGlobal>

#include "MinerModel.h"
#include "Settings/Settings.h"
#include "IMiningManager.h"
#include "IPoolMiner.h"
#include "Style/Style.h"

namespace WalletGui {

namespace {

const int LAST_CONNECTION_ERROR_TIMER_INTERVAL = 1000;

}

MinerModel::MinerModel(IMiningManager* _miningManager, QObject* _parent) :
  QAbstractItemModel(_parent), m_miningManager(_miningManager), m_lastConnectionErrorTimerId(-1),
  m_columnCount(MinerModel::staticMetaObject.enumerator(MinerModel::staticMetaObject.indexOfEnumerator("Columns")).keyCount()) {
  m_miningManager->addObserver(this);
  qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
}

MinerModel::~MinerModel() {
}

int MinerModel::columnCount(const QModelIndex& _parent) const {
  return m_columnCount;
}

int MinerModel::rowCount(const QModelIndex& _parent) const {
  return m_miningManager->getMinerCount();
}

Qt::DropActions MinerModel::supportedDragActions() const {
  return Qt::MoveAction;
}

Qt::DropActions MinerModel::supportedDropActions() const {
  return Qt::MoveAction;
}

QVariant MinerModel::data(const QModelIndex& _index, int _role) const {
  if (!_index.isValid()) {
    return QVariant();
  }

  switch(_role) {
  case Qt::DisplayRole:
  case Qt::EditRole:
    return getDislayRole(_index);
  case Qt::DecorationRole:
    return getDecorationRole(_index);
  case Qt::TextAlignmentRole:
    return headerData(_index.column(), Qt::Horizontal, _role);
  case Qt::ToolTipRole:
    if (_index.column() == COLUMN_REMOVE) {
      return tr("Remove pool from the list");
    }

    return QVariant();

  case Qt::FontRole:
    if (_index.column() == COLUMN_REMOVE) {
      QFont font;
      font.setPixelSize(Style::FontSize::FONT_LARGE);
      return font;
    }

    return QVariant();

  case Qt::ForegroundRole:
    return _index.column() == COLUMN_REMOVE ? QColor(Settings::instance().getCurrentStyle().fontColorGray()) : QVariant();
  default:
    return getUserRoles(_index, _role);
  }

  return QVariant();
}

Qt::ItemFlags MinerModel::flags(const QModelIndex& _index) const {
  if (!_index.isValid()) {
    return 0;
  }

  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsDropEnabled;
  if (_index.column() != COLUMN_REMOVE) {
    flags |= Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
  }

  return flags;
}

QVariant MinerModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  if (_orientation != Qt::Horizontal) {
    return QVariant();
  }

  switch(_role) {
  case Qt::DisplayRole: {
    switch(_section) {
    case COLUMN_POOL_URL:
      return tr("Pool URL");
    case COLUMN_DIFFICULTY:
      return tr("Difficulty");
    case COLUMN_GOOD_SHARES:
      return tr("Good shares");
    case COLUMN_BAD_SHARES:
      return tr("Bad shares");
    case COLUMN_DONATED_SHARES:
      return tr("Donated shares");
    case COLUMN_CONNECTION_ERROR_COUNT:
      return tr("Connection error");
    case COLUMN_LAST_CONNECTION_ERROR_TIME:
      return tr("Last connection error");
    case COLUMN_REMOVE:
      return QVariant();
    case COLUMN_HASHRATE:
      return tr("Hashrate");
  }
  }

  case Qt::TextAlignmentRole: {
    switch(_section) {
    case COLUMN_POOL_URL:
      return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
    }

    return static_cast<int>(Qt::AlignRight | Qt::AlignVCenter);
  }

  default:
    break;
  }

  return QVariant();
}

QModelIndex MinerModel::index(int _row, int _column, const QModelIndex& _parent) const {
  if (_parent.isValid()) {
    return QModelIndex();
  }

  return createIndex(_row, _column, _row);
}

bool MinerModel::moveRows(const QModelIndex& _sourceParent, int _sourceRow, int _count, const QModelIndex& _destinationParent,
  int _destinationChild) {
  Q_ASSERT(!_sourceParent.isValid());
  Q_ASSERT(!_destinationParent.isValid());
  Q_ASSERT(_sourceRow + _count - 1 < rowCount());
  if (_sourceRow == _destinationChild) {
    return false;
  }

  beginResetModel();
  for (qint32 i = 0; i < _count; ++i) {
    m_miningManager->moveMiner(_sourceRow, _destinationChild);
  }

  endResetModel();
  return true;
}

QModelIndex MinerModel::parent(const QModelIndex& _index) const {
  return QModelIndex();
}

bool MinerModel::removeRows(int _row, int _count, const QModelIndex& _parent) {
  Q_ASSERT(!_parent.isValid());
  Q_ASSERT(_row + _count - 1 < rowCount());
  beginRemoveRows(QModelIndex(), _row, _row + _count - 1);
  for (qint32 i = 0; i < _count; ++i) {
    m_miningManager->removeMiner(_row);
  }

  endRemoveRows();
  return true;
}

void MinerModel::minersLoaded() {
  beginResetModel();
  endResetModel();
}

void MinerModel::minersUnloaded() {
  beginResetModel();
  endResetModel();
}

void MinerModel::miningStarted() {
  if (m_lastConnectionErrorTimerId == -1) {
    m_lastConnectionErrorTimerId = startTimer(LAST_CONNECTION_ERROR_TIMER_INTERVAL)  ;
  }

  Q_EMIT dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

void MinerModel::miningStopped() {
  if (m_lastConnectionErrorTimerId != -1) {
    killTimer(m_lastConnectionErrorTimerId);
    m_lastConnectionErrorTimerId = -1;
  }
  Q_EMIT dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

void MinerModel::activeMinerChanged(quintptr _minerIndex) {
  Q_EMIT dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

void MinerModel::schedulePolicyChanged(int _schedulePolicy) {
  // Do nothing
}

void MinerModel::cpuCoreCountChanged(quint32 _cpuCoreCount) {
  // Do nothing
}

void MinerModel::minerAdded(quintptr _minerIndex) {
  beginInsertRows(QModelIndex(), _minerIndex, _minerIndex);
  endInsertRows();
}

void MinerModel::minerRemoved(quintptr _minerIndex) {
  beginRemoveRows(QModelIndex(), _minerIndex, _minerIndex);
  endRemoveRows();
}

void MinerModel::stateChanged(quintptr _minerIndex, int _newState) {
  Q_EMIT dataChanged(index(_minerIndex, 0), index(_minerIndex, columnCount() - 1), QVector<int>() << ROLE_STATE);
}

void MinerModel::hashRateChanged(quintptr _minerIndex, quint32 _hashRate) {
  QModelIndex minerIndex = index(_minerIndex, COLUMN_HASHRATE);
  Q_EMIT dataChanged(minerIndex, minerIndex);
}

void MinerModel::alternateHashRateChanged(quintptr _minerIndex, quint32 _hashRate) {
  QModelIndex minerIndex = index(_minerIndex, COLUMN_DONATION_HASHRATE);
  Q_EMIT dataChanged(minerIndex, minerIndex);
}

void MinerModel::difficultyChanged(quintptr _minerIndex, quint32 _difficulty) {
  QModelIndex minerIndex = index(_minerIndex, COLUMN_DIFFICULTY);
  Q_EMIT dataChanged(minerIndex, minerIndex);
}

void MinerModel::goodShareCountChanged(quintptr _minerIndex, quint32 _goodShareCount) {
  QModelIndex minerIndex = index(_minerIndex, COLUMN_GOOD_SHARES);
  Q_EMIT dataChanged(minerIndex, minerIndex);
}

void MinerModel::badShareCountChanged(quintptr _minerIndex, quint32 _badShareCount) {
  QModelIndex minerIndex = index(_minerIndex, COLUMN_BAD_SHARES);
  Q_EMIT dataChanged(minerIndex, minerIndex);
}

void MinerModel::goodAlternateShareCountChanged(quintptr _minerIndex, quint32 _goodShareCount) {
  QModelIndex minerIndex = index(_minerIndex, COLUMN_DONATED_SHARES);
  Q_EMIT dataChanged(minerIndex, minerIndex);
}

void MinerModel::connectionErrorCountChanged(quintptr _minerIndex, quint32 _connectionErrorCount) {
  QModelIndex minerIndex = index(_minerIndex, COLUMN_CONNECTION_ERROR_COUNT);
  Q_EMIT dataChanged(minerIndex, minerIndex);
}

void MinerModel::lastConnectionErrorTimeChanged(quintptr _minerIndex, const QDateTime& _connectionErrorTime) {
  QModelIndex minerIndex = index(_minerIndex, COLUMN_LAST_CONNECTION_ERROR_TIME);
  Q_EMIT dataChanged(minerIndex, minerIndex);
}

void MinerModel::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_lastConnectionErrorTimerId) {
    if (rowCount() == 0) {
      return;
    }

    Q_EMIT dataChanged(index(0, COLUMN_LAST_CONNECTION_ERROR_TIME), index(rowCount() - 1, COLUMN_LAST_CONNECTION_ERROR_TIME));
    return;
  }

  QAbstractItemModel::timerEvent(_event);
}

QVariant MinerModel::getDecorationRole(const QModelIndex& _index) const {
  if (_index.column() == COLUMN_POOL_URL) {
    QPixmap statePixmap(6, 6);
    switch (_index.data(ROLE_STATE).toInt()) {
    case IPoolMiner::STATE_RUNNING:
      statePixmap.fill(Settings::instance().getCurrentStyle().fontColorGreen());
      break;
    case IPoolMiner::STATE_ERROR:
      statePixmap.fill(Settings::instance().getCurrentStyle().fontColorRed());
      break;
    case IPoolMiner::STATE_STOPPED:
      statePixmap.fill(Settings::instance().getCurrentStyle().fontColorGray());
      break;
    default:
      break;
    }

    return statePixmap;
  } else if (_index.column() == COLUMN_REMOVE) {
    return QPixmap(":icons/delete_pool");
  }

  return QVariant();
}

QVariant MinerModel::getDislayRole(const QModelIndex& _index) const {
  switch (_index.column()) {
  case COLUMN_POOL_URL:
    return QString("%1:%2").arg(_index.data(ROLE_POOL_HOST).toString()).arg(_index.data(ROLE_POOL_PORT).value<quint16>());
  case COLUMN_DIFFICULTY:
    return _index.data(ROLE_DIFFICULTY);
  case COLUMN_GOOD_SHARES:
    return _index.data(ROLE_GOOD_SHARES);
  case COLUMN_BAD_SHARES:
    return _index.data(ROLE_BAD_SHARES);
  case COLUMN_DONATED_SHARES:
    return _index.data(ROLE_DONATED_SHARES);
  case COLUMN_CONNECTION_ERROR_COUNT:
    return _index.data(ROLE_CONNECTION_ERROR_COUNT);
  case COLUMN_LAST_CONNECTION_ERROR_TIME: {
    QDateTime lastConnectionErrorTime = _index.data(ROLE_LAST_CONNECTION_ERROR_TIME).toDateTime();
    if (!lastConnectionErrorTime.isValid()) {
      return QVariant();
    }

    return tr("%1 seconds ago").arg(lastConnectionErrorTime.secsTo(QDateTime::currentDateTime()));
  }

  case COLUMN_REMOVE:
      return QVariant();

  case COLUMN_HASHRATE: {
    quint32 hashrate = _index.data(ROLE_HASHRATE).toUInt();
    return tr("%1 H/s").arg(hashrate);
  }

  case COLUMN_DONATION_HASHRATE: {
    quint32 hashrate = _index.data(ROLE_DONATION_HASHRATE).toUInt();
    return tr("%1 H/s").arg(hashrate);
  }
  }

  return QVariant();
}

QVariant MinerModel::getUserRoles(const QModelIndex& _index, int _role) const {
  IPoolMiner* miner = m_miningManager->getMiner(_index.row());
  switch (_role) {
  case ROLE_POOL_HOST:
    return miner->getPoolHost();
  case ROLE_POOL_PORT:
    return miner->getPoolPort();
  case ROLE_DIFFICULTY:
    return miner->getDifficulty();
  case ROLE_GOOD_SHARES:
    return miner->getGoodShareCount();
  case ROLE_BAD_SHARES:
    return miner->getBadShareCount();
  case ROLE_DONATED_SHARES:
    return miner->getGoodAlternateShareCount();
  case ROLE_CONNECTION_ERROR_COUNT:
    return miner->getConnectionErrorCount();
  case ROLE_LAST_CONNECTION_ERROR_TIME:
    return miner->getLastConnectionErrorTime();
  case ROLE_STATE:
    return miner->getCurrentState();
  case ROLE_HASHRATE:
    return miner->getHashRate();
  case ROLE_DONATION_HASHRATE:
    return miner->getAlternateHashRate();
  }

  return QVariant();
}

}
