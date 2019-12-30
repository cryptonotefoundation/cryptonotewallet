// Copyright (c) 2016-2020 The Karbo developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QAbstractItemModel>

#include <INode.h>

namespace WalletGui {

class ConnectionsModel : public QAbstractItemModel {
  Q_OBJECT
  Q_DISABLE_COPY(ConnectionsModel)

public:
  enum Columns {
    COLUMN_START = 0, COLUMN_STATE, COLUMN_ID, COLUMN_HOST, COLUMN_PORT, COLUMN_IS_INCOMING, COLUMN_HEIGHT, COLUMN_LAST_RESPONSE_HEIGHT, COLUMN_VERSION
  };

  enum Roles {
    ROLE_STATE = Qt::UserRole, ROLE_ID, ROLE_HOST, ROLE_PORT, ROLE_START, ROLE_VERSION, ROLE_IS_INCOMING, ROLE_HEIGHT, ROLE_LAST_RESPONSE_HEIGHT
  };

  static ConnectionsModel& instance();
  int columnCount(const QModelIndex& _parent = QModelIndex()) const Q_DECL_OVERRIDE;
  QVariant data(const QModelIndex& _index, int _role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
  Qt::ItemFlags flags(const QModelIndex& _index) const Q_DECL_OVERRIDE;
  QVariant headerData(int _section, Qt::Orientation _orientation, int _role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
  QModelIndex index(int _row, int _column, const QModelIndex& _parent = QModelIndex()) const Q_DECL_OVERRIDE;
  QModelIndex parent(const QModelIndex& _index) const Q_DECL_OVERRIDE;
  int rowCount(const QModelIndex& _parent = QModelIndex()) const Q_DECL_OVERRIDE;

  void refreshConnections();

private:
  ConnectionsModel();
  ~ConnectionsModel();

  std::vector<CryptoNote::p2pConnection> m_connections;
};

}
