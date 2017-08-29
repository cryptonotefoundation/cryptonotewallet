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
#include <QJsonArray>
#include <QJsonDocument>

#include "AddressBookManager.h"
#include "Settings/Settings.h"
#include "WalletLogger/WalletLogger.h"
#include "INodeAdapter.h"

namespace WalletGui {

namespace {

const char ADDRESS_BOOK_TAG_NAME[] = "addressBook";
const char ADDRESS_ITEM_LABEL_TAG_NAME[] = "label";
const char ADDRESS_ITEM_ADDRESS_TAG_NAME[] = "address";
const char ADDRESS_ITEM_DONATION_TAG_NAME[] = "donation";

const char ADDRESS_URL_DONATION_TAG_NAME[] = "donation";
const char ADDRESS_URL_LABEL_TAG_NAME[] = "label";

const char DONATION_TAG_NAME[] = "donation";
const char DONATION_MINING_TAG_NAME[] = "mining";
const char DONATION_MINING_ADDRESS_TAG_NAME[] = "address";
const char DONATION_MINING_IS_ENABLED_TAG_NAME[] = "enabled";
const char DONATION_MINING_AMOUNT_TAG_NAME[] = "amount";
const char DONATION_CHANGE_TAG_NAME[] = "change";
const char DONATION_CHANGE_ADDRESS_TAG_NAME[] = "address";
const char DONATION_CHANGE_IS_ENABLED_TAG_NAME[] = "enabled";
const char DONATION_CHANGE_AMOUNT_TAG_NAME[] = "amount";

}

AddressBookManager::AddressBookManager(ICryptoNoteAdapter* _cryptoNoteAdapter, QObject* _parent) : QObject(_parent),
  m_cryptoNoteAdapter(_cryptoNoteAdapter) {
  m_cryptoNoteAdapter->addObserver(this);
  m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
}

AddressBookManager::~AddressBookManager() {
}

quintptr AddressBookManager::getAddressCount() const {
  return m_addressBookObject[ADDRESS_BOOK_TAG_NAME].toArray().size();
}

AddressItem AddressBookManager::getAddress(quintptr _addressIndex) const {
  Q_ASSERT(_addressIndex < m_addressBookObject[ADDRESS_BOOK_TAG_NAME].toArray().size());
  QJsonObject addressObject = m_addressBookObject[ADDRESS_BOOK_TAG_NAME].toArray()[_addressIndex].toObject();
  AddressItem result {
    addressObject.value(ADDRESS_ITEM_LABEL_TAG_NAME).toString(),
    addressObject.value(ADDRESS_ITEM_ADDRESS_TAG_NAME).toString(),
    addressObject.value(ADDRESS_ITEM_DONATION_TAG_NAME).toBool()
  };

  return result;
}

quintptr AddressBookManager::findAddressByAddress(const QString& _address) const {
  return m_addressIndexes.value(_address, INVALID_ADDRESS_INDEX);
}

quintptr AddressBookManager::findAddressByLabel(const QString& _label) const {
  return m_labelIndexes.value(_label, INVALID_ADDRESS_INDEX);
}

quintptr AddressBookManager::findAddress(const QString& _label, const QString& _address) const {
  QJsonArray addressArray = m_addressBookObject[ADDRESS_BOOK_TAG_NAME].toArray();
  for (int i = 0; i < addressArray.size(); ++i) {
    QJsonObject addressObject = addressArray[i].toObject();
    if (addressObject.value(ADDRESS_ITEM_LABEL_TAG_NAME).toString() == _label &&
      addressObject.value(ADDRESS_ITEM_ADDRESS_TAG_NAME).toString() == _address) {
      return i;
    }
  }

  return INVALID_ADDRESS_INDEX;
}

quintptr AddressBookManager::findAddress(const QString& _label, const QString& _address, bool _isDonationAddress) const {
  QJsonArray addressArray = m_addressBookObject[ADDRESS_BOOK_TAG_NAME].toArray();
  for (int i = 0; i < addressArray.size(); ++i) {
    QJsonObject addressObject = addressArray[i].toObject();
    if (addressObject.value(ADDRESS_ITEM_LABEL_TAG_NAME).toString() == _label &&
      addressObject.value(ADDRESS_ITEM_ADDRESS_TAG_NAME).toString() == _address &&
      addressObject.value(ADDRESS_ITEM_DONATION_TAG_NAME).toBool() == _isDonationAddress) {
      return i;
    }
  }

  return INVALID_ADDRESS_INDEX;
}

void AddressBookManager::addAddress(const QString& _label, const QString& _address, bool _isDonationAddress) {
  WalletLogger::debug(tr("[AddressBook] Add address: label=\"%1\" address=\"%2\" donation=\"%3\"").arg(_label).
    arg(_address).arg(_isDonationAddress));
  if (findAddressByLabel(_label.trimmed()) != INVALID_ADDRESS_INDEX) {
    WalletLogger::critical(tr("[AddressBook] Add address error. Label already exists: label=\"%1\"").arg(_label));
    return;
  }

  if (findAddressByAddress(_address.trimmed()) != INVALID_ADDRESS_INDEX) {
    WalletLogger::critical(tr("[AddressBook] Add address error. Address already exists: address=\"%2\"").arg(_address));
    return;
  }

  QJsonArray addressArray = m_addressBookObject[ADDRESS_BOOK_TAG_NAME].toArray();
  QJsonObject newAddressObject;
  newAddressObject.insert(ADDRESS_ITEM_LABEL_TAG_NAME, _label);
  newAddressObject.insert(ADDRESS_ITEM_ADDRESS_TAG_NAME, _address);
  newAddressObject.insert(ADDRESS_ITEM_DONATION_TAG_NAME, _isDonationAddress);
  addressArray.append(newAddressObject);
  m_addressBookObject.insert(ADDRESS_BOOK_TAG_NAME, addressArray);
  m_addressIndexes.insert(_address, addressArray.size() - 1);
  m_labelIndexes.insert(_label, addressArray.size() - 1);
  saveAddressBook();
  Q_EMIT addressAddedSignal(addressArray.size() - 1);
}

void AddressBookManager::editAddress(quintptr _addressIndex, const QString& _label, const QString& _address, bool _isDonationAddress) {
  WalletLogger::debug(tr("[AddressBook] Edit address: label=\"%1\" address=\"%2\" donation=\"%3\"").arg(_label).
    arg(_address).arg(_isDonationAddress));
  Q_ASSERT(_addressIndex < m_addressBookObject[ADDRESS_BOOK_TAG_NAME].toArray().size());
  QJsonArray addressArray = m_addressBookObject[ADDRESS_BOOK_TAG_NAME].toArray();
  QJsonObject addressObject = addressArray[_addressIndex].toObject();
  if (getDonationMiningAddress() == addressObject.value(ADDRESS_ITEM_ADDRESS_TAG_NAME).toString() &&
    (!_isDonationAddress || addressObject.value(ADDRESS_ITEM_ADDRESS_TAG_NAME).toString() != _address)) {
    setDonationMiningEnabled(false);
    setDonationMiningAddress(QString());
  }

  if (getDonationChangeAddress() == addressObject.value(ADDRESS_ITEM_ADDRESS_TAG_NAME).toString() &&
    (!_isDonationAddress || addressObject.value(ADDRESS_ITEM_ADDRESS_TAG_NAME).toString() != _address)) {
    setDonationChangeEnabled(false);
    setDonationChangeAddress(QString());
  }

  QString oldAddress = addressObject.value(ADDRESS_ITEM_ADDRESS_TAG_NAME).toString();
  QString oldLabel = addressObject.value(ADDRESS_ITEM_LABEL_TAG_NAME).toString();
  addressObject.insert(ADDRESS_ITEM_LABEL_TAG_NAME, _label);
  addressObject.insert(ADDRESS_ITEM_ADDRESS_TAG_NAME, _address);
  addressObject.insert(ADDRESS_ITEM_DONATION_TAG_NAME, _isDonationAddress);
  addressArray[_addressIndex] = addressObject;
  m_addressBookObject.insert(ADDRESS_BOOK_TAG_NAME, addressArray);
  m_addressIndexes.remove(oldAddress);
  m_labelIndexes.remove(oldLabel);
  m_addressIndexes.insert(_address, _addressIndex);
  m_labelIndexes.insert(_label, _addressIndex);
  saveAddressBook();
  Q_EMIT addressEditedSignal(_addressIndex);
}

void AddressBookManager::removeAddress(quintptr _addressIndex) {
  Q_ASSERT(_addressIndex < m_addressBookObject[ADDRESS_BOOK_TAG_NAME].toArray().size());
  QJsonArray addressArray = m_addressBookObject[ADDRESS_BOOK_TAG_NAME].toArray();
  QJsonObject addressObject = addressArray[_addressIndex].toObject();
  WalletLogger::debug(tr("[AddressBook] Remove address: label=\"%1\" address=\"%2\" donation=\"%3\"").
    arg(addressObject.value(ADDRESS_ITEM_LABEL_TAG_NAME).toString()).
    arg(addressObject.value(ADDRESS_ITEM_ADDRESS_TAG_NAME).toString()).
    arg(addressObject.value(ADDRESS_ITEM_DONATION_TAG_NAME).toBool()));
  if (getDonationMiningAddress() == addressObject.value(ADDRESS_ITEM_ADDRESS_TAG_NAME).toString()) {
    setDonationMiningEnabled(false);
    setDonationMiningAddress(QString());
  }

  if (getDonationChangeAddress() == addressObject.value(ADDRESS_ITEM_ADDRESS_TAG_NAME).toString()) {
    setDonationChangeEnabled(false);
    setDonationChangeAddress(QString());
  }

  addressArray.removeAt(_addressIndex);
  m_addressIndexes.remove(addressObject.value(ADDRESS_ITEM_ADDRESS_TAG_NAME).toString());
  m_labelIndexes.remove(addressObject.value(ADDRESS_ITEM_LABEL_TAG_NAME).toString());
  for (quintptr i = _addressIndex; i < addressArray.size(); ++i) {
    QJsonObject addressObject = addressArray[i].toObject();
    m_addressIndexes[addressObject.value(ADDRESS_ITEM_ADDRESS_TAG_NAME).toString()] = i;
    m_labelIndexes[addressObject.value(ADDRESS_ITEM_LABEL_TAG_NAME).toString()] = i;
  }

  m_addressBookObject.insert(ADDRESS_BOOK_TAG_NAME, addressArray);
  saveAddressBook();
  Q_EMIT addressRemovedSignal(_addressIndex);
}

void AddressBookManager::addObserver(IAddressBookManagerObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  connect(this, SIGNAL(addressBookOpenedSignal()), observer, SLOT(addressBookOpened()));
  connect(this, SIGNAL(addressBookClosedSignal()), observer, SLOT(addressBookClosed()));
  connect(this, SIGNAL(addressAddedSignal(quintptr)), observer, SLOT(addressAdded(quintptr)));
  connect(this, SIGNAL(addressEditedSignal(quintptr)), observer, SLOT(addressEdited(quintptr)));
  connect(this, SIGNAL(addressRemovedSignal(quintptr)), observer, SLOT(addressRemoved(quintptr)));
}

void AddressBookManager::removeObserver(IAddressBookManagerObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  disconnect(this, SIGNAL(addressBookOpenedSignal()), observer, SLOT(addressBookOpened()));
  disconnect(this, SIGNAL(addressBookClosedSignal()), observer, SLOT(addressBookClosed()));
  disconnect(this, SIGNAL(addressAddedSignal(quintptr)), observer, SLOT(addressAdded(quintptr)));
  disconnect(this, SIGNAL(addressEditedSignal(quintptr)), observer, SLOT(addressEdited(quintptr)));
  disconnect(this, SIGNAL(addressRemovedSignal(quintptr)), observer, SLOT(addressRemoved(quintptr)));
}

bool WalletGui::AddressBookManager::isDonationMiningEnabled() const {
  QJsonObject donationObject = m_addressBookObject.value(DONATION_TAG_NAME).toObject();
  return donationObject.value(DONATION_MINING_TAG_NAME).toObject().value(DONATION_MINING_IS_ENABLED_TAG_NAME).toBool(false);
}

QString AddressBookManager::getDonationMiningAddress() const {
  QJsonObject donationObject = m_addressBookObject.value(DONATION_TAG_NAME).toObject();
  return donationObject.value(DONATION_MINING_TAG_NAME).toObject().value(DONATION_MINING_ADDRESS_TAG_NAME).toString();
}

int AddressBookManager::getDonationMiningAmount() const {
  QJsonObject donationObject = m_addressBookObject.value(DONATION_TAG_NAME).toObject();
  return donationObject.value(DONATION_MINING_TAG_NAME).toObject().value(DONATION_MINING_AMOUNT_TAG_NAME).toInt(1);
}

bool AddressBookManager::isDonationChangeEnabled() const {
  QJsonObject donationObject = m_addressBookObject.value(DONATION_TAG_NAME).toObject();
  return donationObject.value(DONATION_CHANGE_TAG_NAME).toObject().value(DONATION_CHANGE_IS_ENABLED_TAG_NAME).toBool(false);
}

QString AddressBookManager::getDonationChangeAddress() const {
  QJsonObject donationObject = m_addressBookObject.value(DONATION_TAG_NAME).toObject();
  return donationObject.value(DONATION_CHANGE_TAG_NAME).toObject().value(DONATION_CHANGE_ADDRESS_TAG_NAME).toString();
}

int AddressBookManager::getDonationChangeAmount() const {
  QJsonObject donationObject = m_addressBookObject.value(DONATION_TAG_NAME).toObject();
  return donationObject.value(DONATION_CHANGE_TAG_NAME).toObject().value(DONATION_CHANGE_AMOUNT_TAG_NAME).toInt(10);
}

void AddressBookManager::setDonationMiningEnabled(bool _on) {
  QJsonObject donationObject = m_addressBookObject.value(DONATION_TAG_NAME).toObject();
  if (donationObject.value(DONATION_MINING_TAG_NAME).toObject().value(DONATION_MINING_IS_ENABLED_TAG_NAME).toBool(false) !=
    _on) {
    QJsonObject donationMiningObject = donationObject.value(DONATION_MINING_TAG_NAME).toObject();
    donationMiningObject.insert(DONATION_MINING_IS_ENABLED_TAG_NAME, _on);
    donationObject.insert(DONATION_MINING_TAG_NAME, donationMiningObject);
    m_addressBookObject.insert(DONATION_TAG_NAME, donationObject);
    saveAddressBook();
    Q_EMIT donationMiningEnabledSignal(_on);
  }
}

void AddressBookManager::setDonationMiningAddress(const QString& _address) {
  QJsonObject donationObject = m_addressBookObject.value(DONATION_TAG_NAME).toObject();
  if (donationObject.value(DONATION_MINING_TAG_NAME).toObject().value(DONATION_MINING_ADDRESS_TAG_NAME).toString() !=
    _address) {
    QJsonObject donationMiningObject = donationObject.value(DONATION_MINING_TAG_NAME).toObject();
    donationMiningObject.insert(DONATION_MINING_ADDRESS_TAG_NAME, _address);
    donationObject.insert(DONATION_MINING_TAG_NAME, donationMiningObject);
    m_addressBookObject.insert(DONATION_TAG_NAME, donationObject);
    saveAddressBook();
    Q_EMIT donationMiningAddressChangedSignal(_address);
  }
}

void AddressBookManager::setDonationMiningAmount(int _amount) {
  QJsonObject donationObject = m_addressBookObject.value(DONATION_TAG_NAME).toObject();
  if (donationObject.value(DONATION_MINING_TAG_NAME).toObject().value(DONATION_MINING_AMOUNT_TAG_NAME).toInt(1) !=
    _amount) {
    QJsonObject donationMiningObject = donationObject.value(DONATION_MINING_TAG_NAME).toObject();
    donationMiningObject.insert(DONATION_MINING_AMOUNT_TAG_NAME, _amount);
    donationObject.insert(DONATION_MINING_TAG_NAME, donationMiningObject);
    m_addressBookObject.insert(DONATION_TAG_NAME, donationObject);
    saveAddressBook();
    Q_EMIT donationMiningAmountChangedSignal(_amount);
  }
}

void AddressBookManager::setDonationChangeEnabled(bool _on) {
  QJsonObject donationObject = m_addressBookObject.value(DONATION_TAG_NAME).toObject();
  if (donationObject.value(DONATION_CHANGE_TAG_NAME).toObject().value(DONATION_CHANGE_IS_ENABLED_TAG_NAME).toBool(false) !=
    _on) {
    QJsonObject donationChangeObject = donationObject.value(DONATION_CHANGE_TAG_NAME).toObject();
    donationChangeObject.insert(DONATION_CHANGE_IS_ENABLED_TAG_NAME, _on);
    donationObject.insert(DONATION_CHANGE_TAG_NAME, donationChangeObject);
    m_addressBookObject.insert(DONATION_TAG_NAME, donationObject);
    saveAddressBook();
    Q_EMIT donationChangeEnabledSignal(_on);
  }
}

void AddressBookManager::setDonationChangeAddress(const QString& _address) {
  QJsonObject donationObject = m_addressBookObject.value(DONATION_TAG_NAME).toObject();
  if (donationObject.value(DONATION_CHANGE_TAG_NAME).toObject().value(DONATION_CHANGE_ADDRESS_TAG_NAME).toString() !=
    _address) {
    QJsonObject donationMiningObject = donationObject.value(DONATION_CHANGE_TAG_NAME).toObject();
    donationMiningObject.insert(DONATION_CHANGE_ADDRESS_TAG_NAME, _address);
    donationObject.insert(DONATION_CHANGE_TAG_NAME, donationMiningObject);
    m_addressBookObject.insert(DONATION_TAG_NAME, donationObject);
    saveAddressBook();
    Q_EMIT donationChangeAddressChangedSignal(_address);
  }
}

void AddressBookManager::setDonationChangeAmount(int _amount) {
  QJsonObject donationObject = m_addressBookObject.value(DONATION_TAG_NAME).toObject();
  if (donationObject.value(DONATION_CHANGE_TAG_NAME).toObject().value(DONATION_CHANGE_AMOUNT_TAG_NAME).toInt(1) !=
    _amount) {
    QJsonObject donationChangeObject = donationObject.value(DONATION_CHANGE_TAG_NAME).toObject();
    donationChangeObject.insert(DONATION_CHANGE_AMOUNT_TAG_NAME, _amount);
    donationObject.insert(DONATION_CHANGE_TAG_NAME, donationChangeObject);
    m_addressBookObject.insert(DONATION_TAG_NAME, donationObject);
    saveAddressBook();
    Q_EMIT donationChangeAmountChangedSignal(_amount);
  }
}

void AddressBookManager::addObserver(IDonationManagerObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  connect(this, SIGNAL(donationManagerOpenedSignal()), observer, SLOT(donationManagerOpened()));
  connect(this, SIGNAL(donationManagerClosedSignal()), observer, SLOT(donationManagerClosed()));
  connect(this, SIGNAL(donationMiningEnabledSignal(bool)), observer, SLOT(donationMiningEnabled(bool)));
  connect(this, SIGNAL(donationMiningAddressChangedSignal(QString)), observer, SLOT(donationMiningAddressChanged(QString)));
  connect(this, SIGNAL(donationMiningAmountChangedSignal(int)), observer, SLOT(donationMiningAmountChanged(int)));
  connect(this, SIGNAL(donationChangeEnabledSignal(bool)), observer, SLOT(donationChangeEnabled(bool)));
  connect(this, SIGNAL(donationChangeAddressChangedSignal(QString)), observer, SLOT(donationChangeAddressChanged(QString)));
  connect(this, SIGNAL(donationChangeAmountChangedSignal(int)), observer, SLOT(donationChangeAmountChanged(int)));
}

void AddressBookManager::removeObserver(IDonationManagerObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  disconnect(this, SIGNAL(donationManagerOpenedSignal()), observer, SLOT(donationManagerOpened()));
  disconnect(this, SIGNAL(donationManagerClosedSignal()), observer, SLOT(donationManagerClosed()));
  disconnect(this, SIGNAL(donationMiningEnabledSignal(bool)), observer, SLOT(donationMiningEnabled(bool)));
  disconnect(this, SIGNAL(donationMiningAddressChangedSignal(QString)), observer, SLOT(donationMiningAddressChanged(QString)));
  disconnect(this, SIGNAL(donationMiningAmountChangedSignal(int)), observer, SLOT(donationMiningAmountChanged(int)));
  disconnect(this, SIGNAL(donationChangeEnabledSignal(bool)), observer, SLOT(donationChangeEnabled(bool)));
  disconnect(this, SIGNAL(donationChangeAddressChangedSignal(QString)), observer, SLOT(donationChangeAddressChanged(QString)));
  disconnect(this, SIGNAL(donationChangeAmountChangedSignal(int)), observer, SLOT(donationChangeAmountChanged(int)));
}

void AddressBookManager::walletOpened() {
  QByteArray userData = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->getUserData();
  bool needOverwrite = false;
  if (userData.isEmpty()) {
    QFile addressBookFile(Settings::instance().getLegacyAddressBookFile());
    if (addressBookFile.open(QIODevice::ReadOnly)) {
      needOverwrite = true;
      userData = addressBookFile.readAll();
    }
  }

  if (!userData.isEmpty()) {
    QJsonDocument doc = QJsonDocument::fromBinaryData(qUncompress(userData));
    if (doc.isNull()) {
      doc = QJsonDocument::fromJson(userData);
      if (!doc.isNull()) {
        needOverwrite = true;
      }
    }

    if (!doc.isNull()) {
      if (doc.isArray()) {
        m_addressBookObject.insert(ADDRESS_BOOK_TAG_NAME, doc.array());
        needOverwrite = true;
      } else  {
        m_addressBookObject = doc.object();
      }

      if (needOverwrite) {
        saveAddressBook();
      }
    }
  }

  buildIndexes();
  Q_EMIT addressBookOpenedSignal();
  Q_EMIT donationManagerOpenedSignal();
}

void AddressBookManager::walletOpenError(int _initStatus) {
  // Do nothing
}

void AddressBookManager::walletClosed() {
  m_addressIndexes.clear();
  m_labelIndexes.clear();
  m_addressBookObject = QJsonObject();
  Q_EMIT addressBookClosedSignal();
  Q_EMIT donationManagerClosedSignal();
}

void AddressBookManager::passwordChanged() {
  // Do nothing
}

void AddressBookManager::synchronizationProgressUpdated(quint32 _current, quint32 _total) {
  // Do nothing
}

void AddressBookManager::synchronizationCompleted() {
  // Do nothing
}

void AddressBookManager::balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) {
  // Do nothing
}

void AddressBookManager::externalTransactionCreated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void AddressBookManager::transactionUpdated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void AddressBookManager::cryptoNoteAdapterInitCompleted(int _status) {
  if (_status == 0) {
    m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
  }
}

void AddressBookManager::cryptoNoteAdapterDeinitCompleted() {
  // Do nothing
}

void AddressBookManager::saveAddressBook() {
  m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->setUserData(qCompress(QJsonDocument(m_addressBookObject).toBinaryData()));
}

void AddressBookManager::buildIndexes() {
  QJsonArray addressArray = m_addressBookObject[ADDRESS_BOOK_TAG_NAME].toArray();
  for (int i = 0; i < addressArray.size(); ++i) {
    QJsonObject addressObject = addressArray[i].toObject();
    m_addressIndexes.insert(addressObject.value(ADDRESS_ITEM_ADDRESS_TAG_NAME).toString(), i);
    m_labelIndexes.insert(addressObject.value(ADDRESS_ITEM_LABEL_TAG_NAME).toString(), i);
  }
}

}
