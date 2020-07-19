// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2015 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <iostream>
#include <QIcon>
#include "NodeModel.h"
#include "Settings.h"
#include "QUrl"

namespace WalletGui {

NodeModel::NodeModel(QObject* parent) : QAbstractTableModel(parent),
                                        m_nodesCurrentIndex(0) {
  m_RpcNodesList = QVector<NodeSetting>(Settings::instance().getRpcNodesList());
}

NodeModel::~NodeModel() {
}

QVariant NodeModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  const NodeSetting &data = m_RpcNodesList.at(index.row());
  QVariant value;
  if (role == Qt::DisplayRole) {
    if (index.column() == 1) value = QVariant(data.host);
    else if (index.column() == 2) value = QVariant(data.port);
    else if (index.column() == 3) value = QVariant(data.path);
  } else if (role == Qt::DecorationRole && index.column() == 0) {
    QString encryptionIconPath = data.ssl ? ":icons/encrypted" : ":icons/decrypted";
    QPixmap encryptionIcon = QPixmap(encryptionIconPath).scaled(16, 16, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    value = QVariant(encryptionIcon);
  } else if (role == Qt::CheckStateRole && index.column() == 0) {
    if (m_nodesCurrentIndex == index.row()) return Qt::Checked;
    else return Qt::Unchecked;
  }
  return value;
}

int NodeModel::rowCount(const QModelIndex &parent) const {
  return m_RpcNodesList.count();
}

int NodeModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 4;
}

Qt::ItemFlags NodeModel::flags(const QModelIndex& _index) const {
  return (Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsSelectable);
}

void NodeModel::pushCurrentIndex(int currentIndex) {
  m_nodesCurrentIndex = currentIndex;
}

void NodeModel::addNode(const NodeSetting &nodeSetting) {
  beginResetModel();
  m_RpcNodesList.append(nodeSetting);
  endResetModel();
  Settings::instance().setRpcNodesList(m_RpcNodesList);
}

void NodeModel::delNode(const int &index) {
  beginResetModel();
  m_RpcNodesList.remove(index);
  endResetModel();
  Settings::instance().setRpcNodesList(m_RpcNodesList);
}

int NodeModel::getIndexByData(const NodeSetting &nodeSetting) const {
  int index = -1;
  for (int i = 0; i < m_RpcNodesList.count(); i++) {
    if (m_RpcNodesList[i].host == nodeSetting.host &&
        m_RpcNodesList[i].port == nodeSetting.port &&
        m_RpcNodesList[i].path == nodeSetting.path &&
        m_RpcNodesList[i].ssl == nodeSetting.ssl) {
      index = i;
      break;
    }
  }
  return index;
}

NodeSetting NodeModel::getDataByIndex(const int &index) const {
  return m_RpcNodesList[index];
}

}

