// Copyright (c) 2016-2020 The Karbo developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "ConnectionsModel.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

#include <QDateTime>

#include "Common/StringTools.h"
#include "NodeAdapter.h"

namespace WalletGui {

inline QString getStateString(CryptoNote::p2pConnection::state s) {
  switch (s)  {
  case CryptoNote::p2pConnection::state_befor_handshake:
    return "before handshake";
  case CryptoNote::p2pConnection::state_synchronizing:
    return "synchronizing";
  case CryptoNote::p2pConnection::state_idle:
    return "idle";
  case CryptoNote::p2pConnection::state_normal:
    return "normal";
  case CryptoNote::p2pConnection::state_sync_required:
    return "sync required";
  case CryptoNote::p2pConnection::state_pool_sync_required:
    return "pool sync required";
  case CryptoNote::p2pConnection::state_shutdown:
    return "shutdown";
  default:
    return "unknown";
  }
}

ConnectionsModel& ConnectionsModel::instance() {
  static ConnectionsModel inst;
  return inst;
}

ConnectionsModel::ConnectionsModel() : QAbstractItemModel() {
  connect(&NodeAdapter::instance(), &NodeAdapter::nodeInitCompletedSignal, this, &ConnectionsModel::refreshConnections, Qt::QueuedConnection);
  connect(&NodeAdapter::instance(), &NodeAdapter::peerCountUpdatedSignal, this, &ConnectionsModel::refreshConnections, Qt::QueuedConnection);
  connect(&NodeAdapter::instance(), &NodeAdapter::lastKnownBlockHeightUpdatedSignal, this, &ConnectionsModel::refreshConnections, Qt::QueuedConnection);
}

ConnectionsModel::~ConnectionsModel() {
}

QModelIndex ConnectionsModel::index(int _row, int _column, const QModelIndex& _parent) const {
  if (_parent.isValid()) {
    return QModelIndex();
  }

  return createIndex(_row, _column, _row);
}

QModelIndex ConnectionsModel::parent(const QModelIndex& _index) const {
  return QModelIndex();
}

int ConnectionsModel::columnCount(const QModelIndex& _parent) const {
  return 9;
}

int ConnectionsModel::rowCount(const QModelIndex& _parent) const {
  return m_connections.size();
}

QVariant ConnectionsModel::data(const QModelIndex& _index, int _role) const {
  if (!_index.isValid()) {
    return QVariant();
  }

  switch (_role) {
  case Qt::DisplayRole:
    switch (_index.column()) {
    case COLUMN_STATE:
      return _index.data(ROLE_STATE);
    case COLUMN_ID:
      return _index.data(ROLE_ID);
    case COLUMN_HOST:
      return _index.data(ROLE_HOST);
    case COLUMN_PORT:
      return _index.data(ROLE_PORT);
    case COLUMN_START:
      return _index.data(ROLE_START);
    case COLUMN_VERSION:
      return _index.data(ROLE_VERSION);
    case COLUMN_IS_INCOMING:
      return _index.data(ROLE_IS_INCOMING);
    case COLUMN_HEIGHT:
      return _index.data(ROLE_HEIGHT);
    case COLUMN_LAST_RESPONSE_HEIGHT:
      return _index.data(ROLE_LAST_RESPONSE_HEIGHT);
    default:
      return QVariant();
    }

  case ROLE_ID:
    return QString::fromStdString(boost::lexical_cast<std::string>(m_connections[_index.row()].connection_id));

  case ROLE_HOST:
    return QString::fromStdString(Common::ipAddressToString(m_connections[_index.row()].remote_ip));

  case ROLE_PORT:
    return m_connections[_index.row()].remote_port;

  case ROLE_STATE:
    return getStateString(m_connections[_index.row()].connection_state);

  case ROLE_START: {
    QDateTime date = QDateTime::fromTime_t(m_connections[_index.row()].started);
    return (date.isNull() || !date.isValid() ? "-" : date.toString("dd.MM.yy HH:mm"));
  }

  case ROLE_VERSION:
    return m_connections[_index.row()].version;

  case ROLE_IS_INCOMING:
    return m_connections[_index.row()].is_incoming ? tr("Incoming") : tr("Outgoing");

  case ROLE_HEIGHT:
    return m_connections[_index.row()].remote_blockchain_height;

  case ROLE_LAST_RESPONSE_HEIGHT:
    return m_connections[_index.row()].last_response_height;

  default:
    return QVariant();
  }

  return QVariant();
}

Qt::ItemFlags ConnectionsModel::flags(const QModelIndex& _index) const {
  return (Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsSelectable);
}

QVariant ConnectionsModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  if (_orientation != Qt::Horizontal || _role != Qt::DisplayRole) {
    return QVariant();
  }

  switch (_section) {
  case COLUMN_STATE:
    return tr("State");
  case COLUMN_ID:
    return tr("Id");
  case COLUMN_HOST:
    return tr("Host");
  case COLUMN_PORT:
    return tr("Port");
  case COLUMN_START:
    return tr("Start");
  case COLUMN_VERSION:
    return tr("Version");
  case COLUMN_IS_INCOMING:
    return tr("Type");
  case COLUMN_HEIGHT:
    return tr("Height");
  case COLUMN_LAST_RESPONSE_HEIGHT:
    return tr("Last resp. height");
  }

  return QVariant();
}

void ConnectionsModel::refreshConnections() {
  m_connections = NodeAdapter::instance().getConnections();
  beginInsertRows(QModelIndex(), m_connections.size(), m_connections.size());
  endInsertRows();
}

}
