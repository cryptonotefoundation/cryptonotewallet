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
#include "Message.h"
#include "NodeAdapter.h"
#include "TransactionsModel.h"
#include "WalletAdapter.h"

namespace WalletGui {

enum class TransactionType : quint8 {MINED, INPUT, OUTPUT, INOUT, DEPOSIT};

const int TRANSACTIONS_MODEL_COLUMN_COUNT =
  TransactionsModel::staticMetaObject.enumerator(TransactionsModel::staticMetaObject.indexOfEnumerator("Columns")).keyCount();

namespace {

QPixmap getTransactionIcon(TransactionType _transactionType) {
  switch (_transactionType) {
  case TransactionType::MINED:
    return QPixmap(":icons/tx-mined");
  case TransactionType::INPUT:
    return QPixmap(":icons/tx-input");
  case TransactionType::OUTPUT:
    return QPixmap(":icons/tx-output");
  case TransactionType::INOUT:
    return QPixmap(":icons/tx-inout");
  case TransactionType::DEPOSIT:
    return QPixmap(":icons/tx-deposit");
  default:
    break;
  }

  return QPixmap();
}

}

TransactionsModel& TransactionsModel::instance() {
  static TransactionsModel inst;
  return inst;
}

TransactionsModel::TransactionsModel() : QAbstractItemModel() {
  connect(&WalletAdapter::instance(), &WalletAdapter::reloadWalletTransactionsSignal, this, &TransactionsModel::reloadWalletTransactions,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletTransactionCreatedSignal, this,
    static_cast<void(TransactionsModel::*)(CryptoNote::TransactionId)>(&TransactionsModel::appendTransaction), Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletTransactionUpdatedSignal, this, &TransactionsModel::updateWalletTransaction,
    Qt::QueuedConnection);
  connect(&NodeAdapter::instance(), &NodeAdapter::lastKnownBlockHeightUpdatedSignal, this, &TransactionsModel::lastKnownHeightUpdated,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &TransactionsModel::reset,
    Qt::QueuedConnection);
}

TransactionsModel::~TransactionsModel() {
}

Qt::ItemFlags TransactionsModel::flags(const QModelIndex& _index) const {
  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsSelectable;
  if(_index.column() == COLUMN_HASH) {
    flags |= Qt::ItemIsEditable;
  }

  return flags;
}

int TransactionsModel::columnCount(const QModelIndex& _parent) const {
  return TRANSACTIONS_MODEL_COLUMN_COUNT;
}

int TransactionsModel::rowCount(const QModelIndex& _parent) const {
  return m_transfers.size();
}

QVariant TransactionsModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  if(_orientation != Qt::Horizontal) {
    return QVariant();
  }

  switch(_role) {
  case Qt::DisplayRole:
    switch(_section) {
    case COLUMN_STATE:
      return QVariant();
    case COLUMN_DATE:
      return tr("Date");
    case COLUMN_TYPE:
      return tr("Type");
    case COLUMN_HASH:
      return tr("Hash");
    case COLUMN_ADDRESS:
      return tr("Address");
    case COLUMN_AMOUNT:
      return tr("Amount");
    case COLUMN_PAYMENT_ID:
      return tr("PaymentID");
    case COLUMN_MESSAGE:
      return tr("Message");
    default:
      break;
    }

  case Qt::TextAlignmentRole:
    if (_section == COLUMN_AMOUNT) {
      return static_cast<int>(Qt::AlignRight | Qt::AlignVCenter);
    }

    return QVariant();

  case ROLE_COLUMN:
    return _section;
  }

  return QVariant();
}

QVariant TransactionsModel::data(const QModelIndex& _index, int _role) const {
  if(!_index.isValid()) {
    return QVariant();
  }

  CryptoNote::WalletLegacyTransaction transaction;
  CryptoNote::WalletLegacyTransfer transfer;
  CryptoNote::Deposit deposit;
  CryptoNote::TransactionId transactionId = m_transfers.value(_index.row()).first;
  CryptoNote::TransferId transferId = m_transfers.value(_index.row()).second;

  if(!WalletAdapter::instance().getTransaction(transactionId, transaction) ||
    (m_transfers.value(_index.row()).second != CryptoNote::WALLET_LEGACY_INVALID_TRANSFER_ID &&
    !WalletAdapter::instance().getTransfer(transferId, transfer))) {
    return QVariant();
  }

  CryptoNote::DepositId depositId = transaction.firstDepositId;
  if (depositId != CryptoNote::WALLET_LEGACY_INVALID_DEPOSIT_ID) {
    if(!WalletAdapter::instance().getDeposit(depositId, deposit)) {
      return QVariant();
    }
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
    return getUserRole(_index, _role, transactionId, transaction, transferId, transfer, depositId, deposit);
  }

  return QVariant();
}

QModelIndex TransactionsModel::index(int _row, int _column, const QModelIndex& _parent) const {
  if(_parent.isValid()) {
    return QModelIndex();
  }

  return createIndex(_row, _column, _row);
}

QModelIndex TransactionsModel::parent(const QModelIndex& _index) const {
  return QModelIndex();
}

QByteArray TransactionsModel::toCsv() const {
  QByteArray res;
  res.append("\"State\",\"Date\",\"Amount\",\"Fee\",\"Hash\",\"Height\",\"Address\",\"Payment ID\"\n");
  for (quint32 row = 0; row < rowCount(); ++row) {
    QModelIndex ind = index(row, COLUMN_STATE);
    quint64 numberOfConfirmations = ind.data(ROLE_NUMBER_OF_CONFIRMATIONS).value<quint64>();
    QString text = (numberOfConfirmations == 0 ? tr("unconfirmed") : tr("confirmations"));
    res.append("\"").append(tr("%1 / %2").arg(numberOfConfirmations).arg(text).toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_DATE).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_AMOUNT).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_FEE).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_HASH).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_HEIGHT).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_ADDRESS).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_PAYMENT_ID).data().toString().toUtf8()).append("\"\n");
  }

  return res;
}

QVariant TransactionsModel::getDisplayRole(const QModelIndex& _index) const {
  switch(_index.column()) {
  case COLUMN_DATE: {
    QDateTime date = _index.data(ROLE_DATE).toDateTime();
    return (date.isNull() || !date.isValid() ? "-" : date.toString("dd-MM-yy HH:mm"));
  }

  case COLUMN_HASH:
    return _index.data(ROLE_HASH).toByteArray().toHex().toUpper();

  case COLUMN_ADDRESS: {
    TransactionType transactionType = static_cast<TransactionType>(_index.data(ROLE_TYPE).value<quint8>());
    QString transactionAddress = _index.data(ROLE_ADDRESS).toString();
    if (transactionType == TransactionType::INPUT || transactionType == TransactionType::MINED ||
        transactionType == TransactionType::INOUT) {
      return QString(tr("me (%1)").arg(WalletAdapter::instance().getAddress()));
    } else if (transactionAddress.isEmpty()) {
      return tr("(n/a)");
    }

    return transactionAddress;
  }

  case COLUMN_AMOUNT: {
    qint64 amount = _index.data(ROLE_AMOUNT).value<qint64>();
    QString amountStr = CurrencyAdapter::instance().formatAmount(qAbs(amount));
    return (amount < 0 ? "-" + amountStr : amountStr);
  }

  case COLUMN_PAYMENT_ID:
    return _index.data(ROLE_PAYMENT_ID);

  case COLUMN_FEE: {
    qint64 fee = _index.data(ROLE_FEE).value<qint64>();
    return CurrencyAdapter::instance().formatAmount(fee);
  }

  case COLUMN_HEIGHT: {
    quint64 transactionHeight = _index.data(ROLE_HEIGHT).value<quint64>();
    if (transactionHeight == CryptoNote::WALLET_LEGACY_UNCONFIRMED_TRANSACTION_HEIGHT) {
      return QVariant();
    }

    return QString::number(transactionHeight);
  }

  case COLUMN_MESSAGE: {
    QString messageString = _index.data(ROLE_MESSAGE).toString();
    QTextStream messageStream(&messageString);
    return messageStream.readLine();
  }

  default:
    break;
  }

  return QVariant();
}

QVariant TransactionsModel::getDecorationRole(const QModelIndex& _index) const {
  if(_index.column() == COLUMN_STATE) {
    quint64 numberOfConfirmations = _index.data(ROLE_NUMBER_OF_CONFIRMATIONS).value<quint64>();
    switch (numberOfConfirmations) {
    case 0:
      return QPixmap(":icons/unconfirmed");
    case 1:
      return QPixmap(":icons/clock1");
    case 2:
      return QPixmap(":icons/clock2");
    case 3:
      return QPixmap(":icons/clock3");
    case 4:
      return QPixmap(":icons/clock4");
    case 5:
      return QPixmap(":icons/clock5");
    default:
      return QPixmap(":icons/transaction");
    }

  } else if (_index.column() == COLUMN_ADDRESS) {
    return _index.data(ROLE_ICON).value<QPixmap>().scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  }

  return QVariant();
}

QVariant TransactionsModel::getAlignmentRole(const QModelIndex& _index) const {
  return headerData(_index.column(), Qt::Horizontal, Qt::TextAlignmentRole);
}

QVariant TransactionsModel::getUserRole(const QModelIndex& _index, int _role, CryptoNote::TransactionId _transactionId,
  const CryptoNote::WalletLegacyTransaction& _transaction, CryptoNote::TransferId _transferId, const CryptoNote::WalletLegacyTransfer& _transfer,
  CryptoNote::DepositId _depositId, const CryptoNote::Deposit& _deposit) const {
  switch(_role) {
  case ROLE_DATE:
    return (_transaction.timestamp > 0 ? QDateTime::fromTime_t(_transaction.timestamp) : QDateTime());

  case ROLE_TYPE: {
    QString transactionAddress = _index.data(ROLE_ADDRESS).toString();
    if(_transaction.isCoinbase) {
      return static_cast<quint8>(TransactionType::MINED);
    } else if (_transaction.firstDepositId != CryptoNote::WALLET_LEGACY_INVALID_DEPOSIT_ID) {
      return static_cast<quint8>(TransactionType::DEPOSIT);
    } else if (!transactionAddress.compare(WalletAdapter::instance().getAddress())) {
      return static_cast<quint8>(TransactionType::INOUT);
    } else if(_transaction.totalAmount < 0) {
      return static_cast<quint8>(TransactionType::OUTPUT);
    }

    return static_cast<quint8>(TransactionType::INPUT);
  }

  case ROLE_HASH:
    return QByteArray(reinterpret_cast<const char*>(&_transaction.hash), sizeof(_transaction.hash));

  case ROLE_ADDRESS:
    return QString::fromStdString(_transfer.address);

  case ROLE_AMOUNT: {
    TransactionType transactionType = static_cast<TransactionType>(_index.data(ROLE_TYPE).value<quint8>());
    if (transactionType == TransactionType::INPUT || transactionType == TransactionType::MINED) {
      return static_cast<qint64>(_transaction.totalAmount);
    } else if (transactionType == TransactionType::OUTPUT || transactionType == TransactionType::INOUT) {
      if (_transferId == CryptoNote::WALLET_LEGACY_INVALID_TRANSFER_ID) {
        return static_cast<qint64>(_transaction.totalAmount);
      }

      return static_cast<qint64>(-_transfer.amount);
    } else if (transactionType == TransactionType::DEPOSIT) {
      return static_cast<qint64>(-(_transaction.fee + _deposit.amount));
    }

    return QVariant();
  }

  case ROLE_PAYMENT_ID:
    return NodeAdapter::instance().extractPaymentId(_transaction.extra);

  case ROLE_ICON: {
    TransactionType transactionType = static_cast<TransactionType>(_index.data(ROLE_TYPE).value<quint8>());
    return getTransactionIcon(transactionType);
  }

  case ROLE_TRANSACTION_ID:
    return QVariant::fromValue(_transactionId);

  case ROLE_HEIGHT:
    return static_cast<quint64>(_transaction.blockHeight);

  case ROLE_FEE:
    return static_cast<quint64>(_transaction.fee);

  case ROLE_NUMBER_OF_CONFIRMATIONS:
    return (_transaction.blockHeight == CryptoNote::WALLET_LEGACY_UNCONFIRMED_TRANSACTION_HEIGHT ? 0 :
      NodeAdapter::instance().getLastKnownBlockHeight() - _transaction.blockHeight + 1);

  case ROLE_COLUMN:
    return headerData(_index.column(), Qt::Horizontal, ROLE_COLUMN);

  case ROLE_ROW:
    return _index.row();

  case ROLE_MESSAGE: {
    if (_transaction.messages.size() == 0) {
      return QVariant();
    }

    QString messageString = Message(QString::fromStdString(_transaction.messages[0])).getMessage();
    QTextStream messageStream(&messageString);
    return messageStream.readLine();
  }

  case ROLE_MESSAGES: {
    QStringList messageList;
    messageList.reserve(_transaction.messages.size());
    Q_FOREACH (const auto& message, _transaction.messages) {
      messageList << QString::fromStdString(message);
    }

    return messageList;
  }

  case ROLE_DEPOSIT_ID:
    return static_cast<quintptr>(_transaction.firstDepositId);

  case ROLE_DEPOSIT_COUNT:
    return static_cast<quintptr>(_transaction.depositCount);
  }

  return QVariant();
}

void TransactionsModel::reloadWalletTransactions() {
  beginResetModel();
  m_transfers.clear();
  m_transactionRow.clear();
  endResetModel();

  quint32 row_count = 0;
  for (CryptoNote::TransactionId transactionId = 0; transactionId < WalletAdapter::instance().getTransactionCount(); ++transactionId) {
    appendTransaction(transactionId, row_count);
  }

  if (row_count > 0) {
    beginInsertRows(QModelIndex(), 0, row_count - 1);
    endInsertRows();
  }
}

void TransactionsModel::appendTransaction(CryptoNote::TransactionId _transactionId, quint32& _insertedRowCount) {
  CryptoNote::WalletLegacyTransaction transaction;
  if (!WalletAdapter::instance().getTransaction(_transactionId, transaction)) {
    return;
  }

  if (transaction.transferCount) {
    m_transactionRow[_transactionId] = qMakePair(m_transfers.size(), transaction.transferCount);
    for (CryptoNote::TransferId transfer_id = transaction.firstTransferId;
      transfer_id < transaction.firstTransferId + transaction.transferCount; ++transfer_id) {
      m_transfers.append(TransactionTransferId(_transactionId, transfer_id));
      ++_insertedRowCount;
    }
  } else {
    m_transfers.append(TransactionTransferId(_transactionId, CryptoNote::WALLET_LEGACY_INVALID_TRANSFER_ID));
    m_transactionRow[_transactionId] = qMakePair(m_transfers.size() - 1, 1);
    ++_insertedRowCount;
  }
}

void TransactionsModel::appendTransaction(CryptoNote::TransactionId _transactionId) {
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

void TransactionsModel::updateWalletTransaction(CryptoNote::TransactionId _id) {
  quint32 firstRow = m_transactionRow.value(_id).first;
  quint32 lastRow = firstRow + m_transactionRow.value(_id).second - 1;
  Q_EMIT dataChanged(index(firstRow, COLUMN_DATE), index(lastRow, COLUMN_DATE));
}

void TransactionsModel::lastKnownHeightUpdated(quint64 _height) {
  if(rowCount() > 0) {
    Q_EMIT dataChanged(index(0, COLUMN_STATE), index(rowCount() - 1, COLUMN_STATE));
  }
}

void TransactionsModel::reset() {
  beginResetModel();
  m_transfers.clear();
  m_transactionRow.clear();
  endResetModel();
}

}
