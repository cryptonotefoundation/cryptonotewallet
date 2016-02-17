// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDateTime>
#include <QFont>
#include <QMetaEnum>
#include <QPixmap>
#include <QTextStream>

#include "CurrencyAdapter.h"
#include "NodeAdapter.h"
#include "MessagesModel.h"
#include "WalletAdapter.h"

namespace WalletGui {

enum class MessageType : quint8 {INPUT, OUTPUT};

const int MESSAGES_MODEL_COLUMN_COUNT =
  MessagesModel::staticMetaObject.enumerator(MessagesModel::staticMetaObject.indexOfEnumerator("Columns")).keyCount();

const QString MessagesModel::HEADER_REPLY_TO_KEY = "Reply-To";

MessagesModel& MessagesModel::instance() {
  static MessagesModel inst;
  return inst;
}

MessagesModel::MessagesModel() : QAbstractItemModel() {
  connect(&WalletAdapter::instance(), &WalletAdapter::reloadWalletTransactionsSignal, this, &MessagesModel::reloadWalletTransactions,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletTransactionCreatedSignal, this,
    static_cast<void(MessagesModel::*)(CryptoNote::TransactionId)>(&MessagesModel::appendTransaction), Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletTransactionUpdatedSignal, this, &MessagesModel::updateWalletTransaction,
    Qt::QueuedConnection);
  connect(&NodeAdapter::instance(), &NodeAdapter::lastKnownBlockHeightUpdatedSignal, this, &MessagesModel::lastKnownHeightUpdated,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &MessagesModel::reset,
    Qt::QueuedConnection);
}

MessagesModel::~MessagesModel() {
}

Qt::ItemFlags MessagesModel::flags(const QModelIndex& _index) const {
  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsSelectable;

  return flags;
}

int MessagesModel::columnCount(const QModelIndex& _parent) const {
  return MESSAGES_MODEL_COLUMN_COUNT;
}

int MessagesModel::rowCount(const QModelIndex& _parent) const {
  return m_messages.size();
}

QVariant MessagesModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  if(_orientation != Qt::Horizontal) {
    return QVariant();
  }

  switch(_role) {
  case Qt::DisplayRole:
    switch(_section) {
    case COLUMN_DATE:
      return tr("Date");
    case COLUMN_TYPE:
      return tr("Type");
    case COLUMN_HEIGHT:
      return tr("Height");
    case COLUMN_MESSAGE:
      return tr("Message");
    case COLUMN_HASH:
      return tr("Transaction hash");
    case COLUMN_AMOUNT:
      return tr("Amount");
    case COLUMN_MESSAGE_SIZE:
      return tr("Message size");
    default:
      break;
    }

  case ROLE_COLUMN:
    return _section;
  }

  return QVariant();
}

QVariant MessagesModel::data(const QModelIndex& _index, int _role) const {
  if(!_index.isValid()) {
    return QVariant();
  }

  CryptoNote::WalletLegacyTransaction transaction;
  CryptoNote::TransactionId transactionId = m_messages.value(_index.row()).first;
  Message message = m_messages.value(_index.row()).second;

  if(!WalletAdapter::instance().getTransaction(transactionId, transaction)) {
    return QVariant();
  }

  switch(_role) {
  case Qt::DisplayRole:
  case Qt::EditRole:
    return getDisplayRole(_index);

  case Qt::DecorationRole:
    return getDecorationRole(_index);

  case Qt::TextAlignmentRole:
    return getAlignmentRole(_index);

  default:
    return getUserRole(_index, _role, transactionId, transaction, message);
  }

  return QVariant();
}

QModelIndex MessagesModel::index(int _row, int _column, const QModelIndex& _parent) const {
  if(_parent.isValid()) {
    return QModelIndex();
  }

  return createIndex(_row, _column, _row);
}

QModelIndex MessagesModel::parent(const QModelIndex& _index) const {
  return QModelIndex();
}

QVariant MessagesModel::getDisplayRole(const QModelIndex& _index) const {
  switch(_index.column()) {
  case COLUMN_DATE: {
    QDateTime date = _index.data(ROLE_DATE).toDateTime();
    return (date.isNull() || !date.isValid() ? "-" : date.toString("dd-MM-yy HH:mm"));
  }

  case COLUMN_TYPE: {
    MessageType messageType = static_cast<MessageType>(_index.data(ROLE_TYPE).value<quint8>());
    if (messageType == MessageType::OUTPUT) {
      return tr("Out");
    } else if(messageType == MessageType::INPUT) {
      return tr("In");
    }

    return QVariant();
  }

  case COLUMN_HEIGHT: {
    quint64 height = _index.data(ROLE_HEIGHT).value<quint64>();
    return (height == CryptoNote::WALLET_LEGACY_UNCONFIRMED_TRANSACTION_HEIGHT ? "-" : QString::number(height));
  }

  case COLUMN_MESSAGE: {
    QString messageString = _index.data(ROLE_MESSAGE).toString();
    QTextStream messageStream(&messageString);
    return messageStream.readLine();
  }

  case COLUMN_HASH:
    return _index.data(ROLE_HASH).toByteArray().toHex().toUpper();

  case COLUMN_AMOUNT: {
    qint64 amount = _index.data(MessagesModel::ROLE_AMOUNT).value<qint64>();
    return CurrencyAdapter::instance().formatAmount(qAbs(amount));
  }

  case COLUMN_MESSAGE_SIZE:
    return QString::number(_index.data(ROLE_MESSAGE_SIZE).value<quint32>());

  case COLUMN_FULL_MESSAGE:
    return _index.data(ROLE_FULL_MESSAGE);

  case COLUMN_HAS_REPLY_TO:
    return !_index.data(ROLE_HEADER_REPLY_TO).toString().isEmpty();

  default:
    break;
  }

  return QVariant();
}

QVariant MessagesModel::getDecorationRole(const QModelIndex& _index) const {
  return QVariant();
}

QVariant MessagesModel::getAlignmentRole(const QModelIndex& _index) const {
  return headerData(_index.column(), Qt::Horizontal, Qt::TextAlignmentRole);
}

QVariant MessagesModel::getUserRole(const QModelIndex& _index, int _role, CryptoNote::TransactionId _transactionId,
  CryptoNote::WalletLegacyTransaction& _transaction, const Message& _message) const {
  switch(_role) {
  case ROLE_DATE:
    return (_transaction.timestamp > 0 ? QDateTime::fromTime_t(_transaction.timestamp) : QDateTime());

  case ROLE_TYPE: {
    if(_transaction.totalAmount < 0) {
      return static_cast<quint8>(MessageType::OUTPUT);
    }

    return static_cast<quint8>(MessageType::INPUT);
  }

  case ROLE_HEIGHT:
    return static_cast<quint64>(_transaction.blockHeight);

  case ROLE_MESSAGE:
    return _message.getMessage();

  case ROLE_FULL_MESSAGE:
    return _message.getFullMessage();

  case ROLE_HEADER_REPLY_TO:
    return _message.getHeaderValue(HEADER_REPLY_TO_KEY);

  case ROLE_HASH:
    return QByteArray(reinterpret_cast<char*>(&_transaction.hash), sizeof(_transaction.hash));

  case ROLE_AMOUNT:
    return static_cast<qint64>(_transaction.totalAmount);

  case ROLE_MESSAGE_SIZE:
    return _message.getMessage().size();

  case ROLE_ROW:
    return _index.row();
  }

  return QVariant();
}

void MessagesModel::reloadWalletTransactions() {
  beginResetModel();
  m_transactionRow.clear();
  m_messages.clear();
  endResetModel();

  quint32 rowCount = 0;
  for (CryptoNote::TransactionId transactionId = 0; transactionId < WalletAdapter::instance().getTransactionCount(); ++transactionId) {
    appendTransaction(transactionId, rowCount);
  }

  if (rowCount > 0) {
    beginInsertRows(QModelIndex(), 0, rowCount - 1);
    endInsertRows();
  }
}

void MessagesModel::appendTransaction(CryptoNote::TransactionId _transactionId, quint32& _insertedRowCount) {
  CryptoNote::WalletLegacyTransaction transaction;
  if (!WalletAdapter::instance().getTransaction(_transactionId, transaction)) {
    return;
  }

  m_transactionRow.insert(_transactionId, qMakePair(std::numeric_limits<quint32>::max(), std::numeric_limits<quint32>::max()));
  if (transaction.messages.empty()) {
    return;
  }

  m_transactionRow[_transactionId] = qMakePair(m_messages.size(), transaction.messages.size());
  for (quint32 i = 0; i < transaction.messages.size(); ++i) {
    Message message(QString::fromStdString(transaction.messages[i]));
    m_messages.append(TransactionMessageId(_transactionId, std::move(message)));
    ++_insertedRowCount;
  }
}

void MessagesModel::appendTransaction(CryptoNote::TransactionId _transactionId) {
  if (m_transactionRow.contains(_transactionId)) {
    return;
  }

  quint32 oldRowCount = rowCount();
  quint32 insertedRowCount = 0;
  for (quint64 transactionId = m_transactionRow.size(); transactionId <= _transactionId; ++transactionId) {
    appendTransaction(transactionId, insertedRowCount);
  }

  if (insertedRowCount > 0) {
    beginInsertRows(QModelIndex(), oldRowCount, oldRowCount + insertedRowCount - 1);
    endInsertRows();
  }
}

void MessagesModel::updateWalletTransaction(CryptoNote::TransactionId _id) {
  quint32 firstRow = m_transactionRow.value(_id).first;
  quint32 lastRow = firstRow + m_transactionRow.value(_id).second - 1;
  Q_EMIT dataChanged(index(firstRow, COLUMN_DATE), index(lastRow, COLUMN_HEIGHT));
}

void MessagesModel::lastKnownHeightUpdated(quint64 _height) {
  if(rowCount() > 0) {
    Q_EMIT dataChanged(index(0, COLUMN_DATE), index(rowCount() - 1, COLUMN_HEIGHT));
  }
}

void MessagesModel::reset() {
  beginResetModel();
  m_messages.clear();
  m_transactionRow.clear();
  endResetModel();
}

}
