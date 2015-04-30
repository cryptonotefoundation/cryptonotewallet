// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "WalletAdapter.h"
#include "AddressBookModel.h"
#include "Settings.h"

namespace WalletGui {

AddressBookModel& AddressBookModel::instance() {
  static AddressBookModel inst;
  return inst;
}

AddressBookModel::AddressBookModel() : QAbstractItemModel() {
  connect(&WalletAdapter::instance(), &WalletAdapter::walletInitCompletedSignal, this, &AddressBookModel::walletInitCompleted, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &AddressBookModel::reset, Qt::QueuedConnection);
}

AddressBookModel::~AddressBookModel() {
}

int AddressBookModel::columnCount(const QModelIndex& _parent) const {
  return 2;
}

QVariant AddressBookModel::data(const QModelIndex& _index, int _role) const {
  if (!_index.isValid()) {
    return QVariant();
  }

  QJsonObject address = m_addressBook.at(_index.row()).toObject();

  switch (_role) {
  case Qt::DisplayRole:
    switch (_index.column()) {
    case COLUMN_LABEL:
      return _index.data(ROLE_LABEL);
    case COLUMN_ADDRESS:
      return _index.data(ROLE_ADDRESS);
    default:
      return QVariant();
    }

  case ROLE_LABEL:
    return address.value("label");
  case ROLE_ADDRESS:
    return address.value("address");
  default:
    return QVariant();
  }

  return QVariant();
}

Qt::ItemFlags AddressBookModel::flags(const QModelIndex& _index) const {
  return (Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsSelectable);
}

QVariant AddressBookModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  if (_orientation != Qt::Horizontal || _role != Qt::DisplayRole) {
    return QVariant();
  }

  switch (_section) {
  case COLUMN_LABEL:
    return tr("Label");
  case COLUMN_ADDRESS:
    return tr("Address");
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
  return m_addressBook.size();
}

void AddressBookModel::addAddress(const QString& _label, const QString& _address) {
  beginInsertRows(QModelIndex(), m_addressBook.size(), m_addressBook.size());
  QJsonObject newAddress;
  newAddress.insert("label", _label);
  newAddress.insert("address", _address);
  m_addressBook.append(newAddress);
  endInsertRows();
  saveAddressBook();
}

void AddressBookModel::removeAddress(quint32 _row) {
  if (_row > m_addressBook.size() - 1) {
    return;
  }

  beginRemoveRows(QModelIndex(), _row, _row);
  m_addressBook.removeAt(_row);
  endRemoveRows();
  saveAddressBook();
}

void AddressBookModel::reset() {
  beginResetModel();
  while (!m_addressBook.empty()) {
    m_addressBook.removeFirst();
  }

  endResetModel();
}

void AddressBookModel::saveAddressBook() {
  QFile addressBookFile(Settings::instance().getAddressBookFile());
  if (addressBookFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    QByteArray file_content = QJsonDocument(m_addressBook).toJson(QJsonDocument::Compact);
    addressBookFile.write(file_content);
    addressBookFile.close();
  }
}

void AddressBookModel::walletInitCompleted(int _error, const QString& _error_text) {
  if (!_error) {
    QFile addressBookFile(Settings::instance().getAddressBookFile());
    if (addressBookFile.open(QIODevice::ReadOnly)) {
      QByteArray file_content = addressBookFile.readAll();
      QJsonDocument doc = QJsonDocument::fromJson(file_content);
      if (!doc.isNull()) {
        m_addressBook = doc.array();
      }

      addressBookFile.close();
      if (!m_addressBook.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, m_addressBook.size() - 1);
        endInsertRows();
      }
    }
  }
}

}
