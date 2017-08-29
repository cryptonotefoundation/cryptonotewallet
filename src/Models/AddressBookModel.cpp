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

#include <QFile>
#include <QFont>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>
#include <QPixmap>
#include <QSize>

#include "AddressBookModel.h"
#include "Settings/Settings.h"
#include "IAddressBookManager.h"

namespace WalletGui {

AddressBookModel::AddressBookModel(IAddressBookManager* _addressBookManager, QObject* _parent) : QAbstractItemModel(_parent),
  m_addressBookManager(_addressBookManager),
  m_columnCount(AddressBookModel::staticMetaObject.enumerator(AddressBookModel::staticMetaObject.indexOfEnumerator("Columns")).keyCount()),
  m_rowCount(0) {
  m_addressBookManager->addObserver(this);
}

AddressBookModel::~AddressBookModel() {
}

int AddressBookModel::columnCount(const QModelIndex& _parent) const {
  return m_columnCount;
}

QVariant AddressBookModel::data(const QModelIndex& _index, int _role) const {
  if (!_index.isValid()) {
    return QVariant();
  }

  switch(_role) {
  case Qt::DisplayRole:
    return getDisplayRole(_index);
  case Qt::DecorationRole:
    return getDecorationRole(_index);
  case Qt::TextAlignmentRole:
    return headerData(_index.column(), Qt::Horizontal, _role);
  default:
    return getUserRole(_index, _role);
    break;
  }

  return QVariant();
}

Qt::ItemFlags AddressBookModel::flags(const QModelIndex& _index) const {
  return (Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsSelectable);
}

QVariant AddressBookModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  if (_orientation != Qt::Horizontal) {
    return QVariant();
  }

  switch(_role) {
  case Qt::DisplayRole: {
    switch(_section) {
    case COLUMN_LABEL:
      return tr("Label");
    case COLUMN_ADDRESS:
      return tr("Address");
    case COLUMN_DONATION:
      return tr("Donation");
    }

    break;
  }

  case Qt::TextAlignmentRole: {
    switch(_section) {
    case COLUMN_LABEL:
      return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
    case COLUMN_ADDRESS:
      return static_cast<int>(Qt::AlignRight | Qt::AlignVCenter);
    case COLUMN_DONATION:
      return static_cast<int>(Qt::AlignCenter);
    }

    break;
  }

  case ROLE_COLUMN:
    return _section;
  default:
    break;
  }

  return QVariant();
}

QModelIndex AddressBookModel::index(int _row, int _column, const QModelIndex& _parent) const {
  if (_parent.isValid()) {
    return QModelIndex();
  }

  return createIndex(_row, _column, _row);
}

QModelIndex AddressBookModel::parent(const QModelIndex& _index) const {
  return QModelIndex();
}

int AddressBookModel::rowCount(const QModelIndex& _parent) const {
  return m_rowCount;
}

void AddressBookModel::addressBookOpened() {
  if (m_addressBookManager->getAddressCount() > 0) {
    beginInsertRows(QModelIndex(), 0, m_addressBookManager->getAddressCount() - 1);
    m_rowCount = m_addressBookManager->getAddressCount();
    endInsertRows();
  }
}

void AddressBookModel::addressBookClosed() {
  if (m_rowCount > 0) {
    beginRemoveRows(QModelIndex(), 0, m_rowCount - 1);
    m_rowCount = 0;
    endRemoveRows();
  }
}

void AddressBookModel::addressAdded(quintptr _addressIndex) {
  quintptr newRowCount = m_addressBookManager->getAddressCount();
  if (m_rowCount < newRowCount) {
    beginInsertRows(QModelIndex(), m_rowCount, newRowCount - 1);
    m_rowCount = newRowCount;
    endInsertRows();
  }
}

void AddressBookModel::addressEdited(quintptr _addressIndex) {
  Q_EMIT dataChanged(index(_addressIndex, 0), index(_addressIndex, columnCount() - 1));
}

void AddressBookModel::addressRemoved(quintptr _addressIndex) {
  beginRemoveRows(QModelIndex(), _addressIndex, _addressIndex);
  m_rowCount = m_addressBookManager->getAddressCount();
  endRemoveRows();
}

QVariant AddressBookModel::getDisplayRole(const QModelIndex& _index) const {
  switch(_index.column()) {
  case COLUMN_LABEL:
    return _index.data(ROLE_LABEL);
  case COLUMN_ADDRESS:
    return _index.data(ROLE_ADDRESS);
  default:
    break;
  }

  return QVariant();
}

QVariant AddressBookModel::getDecorationRole(const QModelIndex& _index) const {
  if (_index.column() == COLUMN_DONATION && _index.data(ROLE_IS_DONATION_ADDRESS).toBool()) {
    return QPixmap(":icons/donation");
  }

  return QVariant();
}

QVariant AddressBookModel::getUserRole(const QModelIndex& _index, int _role) const {
  AddressItem addressItem = m_addressBookManager->getAddress(_index.row());
  switch (_role) {
  case ROLE_LABEL:
    return addressItem.label;
  case ROLE_ADDRESS:
    return addressItem.address;
  case ROLE_IS_DONATION_ADDRESS:
    return addressItem.isDonationAddress;
  case ROLE_COLUMN:
    return headerData(_index.column(), Qt::Horizontal, _role);
  case ROLE_ROW:
    return _index.row();
  }

  return QVariant();
}

}
