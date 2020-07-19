// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2015 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QAbstractListModel>
#include "Settings.h"

namespace WalletGui {

class NodeModel : public QAbstractTableModel {
  Q_OBJECT

public:
  explicit NodeModel(QObject *parent = 0);
  ~NodeModel();
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  Qt::ItemFlags flags(const QModelIndex& _index) const Q_DECL_OVERRIDE;
  void pushCurrentIndex(int currentIndex);
  void addNode(const NodeSetting &nodeSetting);
  void delNode(const int &index);
  int getIndexByData(const NodeSetting &nodeSetting) const;
  NodeSetting getDataByIndex(const int &index) const;

private:
  QVector<NodeSetting> m_RpcNodesList;
  int m_nodesCurrentIndex;
};

}

