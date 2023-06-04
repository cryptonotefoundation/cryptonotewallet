// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2015 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "PoolModel.h"
#include "Settings.h"
#include "QUrl"

namespace WalletGui {

PoolModel::PoolModel(QObject* _parent) : QStringListModel(_parent) {
  setStringList(Settings::instance().getMiningPoolList());
}

PoolModel::~PoolModel() {
}

void PoolModel::addPool(const QString& _host, quint16 _port) {
  insertRow(rowCount());
  setData(index(rowCount() - 1, 0), QString("%1:%2").arg(_host).arg(_port));
}

QVariant PoolModel::data(const QModelIndex& _index, int _role) const {
  if (!_index.isValid()) {
    return QVariant();
  }

  switch (_role) {
  case ROLE_HOST:
    return QUrl::fromUserInput(stringList()[_index.row()]).host();
  case ROLE_PORT:
    return QUrl::fromUserInput(stringList()[_index.row()]).port();
  default:
    break;
  }

  return QStringListModel::data(_index, _role);
}

Qt::ItemFlags PoolModel::flags(const QModelIndex& _index) const {
  return (Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsSelectable);
}

QVariant PoolModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  if (_orientation != Qt::Horizontal || _role != Qt::DisplayRole) {
    return QVariant();
  }

  return tr("Pool URL");
}

bool PoolModel::setData(const QModelIndex& _index, const QVariant& _value, int _role) {
  bool res = QStringListModel::setData(_index, _value, _role);
  Settings::instance().setMiningPoolList(stringList());
  return res;
}

}
