// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2019 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDateTime>
#include <QFont>
#include <QMetaEnum>
#include <QPixmap>
#include <QDebug>

#include "crypto/crypto.h"
#include "CryptoNoteCore/CryptoNoteBasic.h"
#include "Common/StringTools.h"
#include "CurrencyAdapter.h"
#include "NodeAdapter.h"
#include "TransactionsModel.h"
#include "AddressBookModel.h"
#include "WalletAdapter.h"

namespace WalletGui {

namespace {

QPixmap getTransactionIcon(TransactionType _transactionType) {
  switch (_transactionType) {
  case TransactionType::MINED:
    return QPixmap(":icons/tx-mined");
  case TransactionType::INPUT:
    return QPixmap(":icons/tx-input");
  case TransactionType::OUTPUT:
    return QPixmap(":icons/tx-output");
  case TransactionType::FUSION:
    return QPixmap(":icons/tx-fusion");
  case TransactionType::INOUT:
    return QPixmap(":icons/tx-inout");
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
  connect(&NodeAdapter::instance(), &NodeAdapter::localBlockchainUpdatedSignal, this, &TransactionsModel::localBlockchainUpdated,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &TransactionsModel::reset,
    Qt::QueuedConnection);
}

TransactionsModel::~TransactionsModel() {
}

Qt::ItemFlags TransactionsModel::flags(const QModelIndex& _index) const {
  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsSelectable; // | Qt::ToolTip;
  return flags;
}

int TransactionsModel::columnCount(const QModelIndex& _parent) const {
  return TransactionsModel::staticMetaObject.enumerator(TransactionsModel::staticMetaObject.indexOfEnumerator("Columns")).keyCount();
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
    case COLUMN_SECRET_KEY:
      return tr("Key");
    case COLUMN_ADDRESS:
      return tr("Address");
    case COLUMN_AMOUNT:
      return tr("Amount");
    case COLUMN_FEE:
      return tr("Fee");
    case COLUMN_PAYMENT_ID:
      return tr("PaymentID");
    default:
      break;
    }

  case Qt::EditRole:
    switch(_section) {
    case COLUMN_STATE:
      return QVariant();
    case COLUMN_DATE:
      return tr("Date");
    case COLUMN_TYPE:
      return tr("Type");
    case COLUMN_HASH:
      return tr("Hash");
    case COLUMN_SECRET_KEY:
      return tr("Key");
    case COLUMN_ADDRESS:
      return tr("Address");
    case COLUMN_AMOUNT:
      return tr("Amount");
    case COLUMN_FEE:
      return tr("Fee");
    case COLUMN_PAYMENT_ID:
      return tr("PaymentID");
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
  CryptoNote::TransactionId transactionId = m_transfers.value(_index.row()).first;
  CryptoNote::TransferId transferId = m_transfers.value(_index.row()).second;

  if(!WalletAdapter::instance().getTransaction(transactionId, transaction) ||
    (m_transfers.value(_index.row()).second != CryptoNote::WALLET_LEGACY_INVALID_TRANSFER_ID &&
    !WalletAdapter::instance().getTransfer(transferId, transfer))) {
    return QVariant();
  }

  switch(_role) {
  case Qt::DisplayRole:
    return getDisplayRole(_index);

  case Qt::EditRole:
    return getEditRole(_index);

  case Qt::DecorationRole:
    return getDecorationRole(_index);

  case Qt::TextAlignmentRole:
    return getAlignmentRole(_index);

  case Qt::ToolTipRole:
    return getToolTipRole(_index);

  default:
    return getUserRole(_index, _role, transactionId, transaction, transferId, transfer);
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
  res.append("\"Date\",\"Amount\",\"Fee\",\"Hash\",\"Height\",\"Address\",\"Payment ID\",\"Key\"\n");
  for (quint32 row = 0; row < rowCount(); ++row) {
    QModelIndex ind = index(row, 0);
    res.append("\"").append(ind.sibling(row, COLUMN_DATE).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_AMOUNT).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_FEE).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_HASH).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_HEIGHT).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_ADDRESS).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_PAYMENT_ID).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_SECRET_KEY).data().toString().toUtf8()).append("\"\n");
  }

  return res;
}

QVariant TransactionsModel::getDisplayRole(const QModelIndex& _index) const {
  switch(_index.column()) {
  case COLUMN_DATE: {
    QDateTime date = _index.data(ROLE_DATE).toDateTime();
    return (date.isNull() || !date.isValid() ? "-" : date.toString("dd.MM.yy HH:mm"));
  }

  case COLUMN_HASH:
    return _index.data(ROLE_HASH).toByteArray().toHex().toUpper();

  case COLUMN_SECRET_KEY:
    return _index.data(ROLE_SECRET_KEY).toByteArray().toHex().toUpper();

  case COLUMN_ADDRESS: {
    TransactionType transactionType = static_cast<TransactionType>(_index.data(ROLE_TYPE).value<quint8>());
    QString transactionAddress = _index.data(ROLE_ADDRESS).toString();
    if (transactionType == TransactionType::INPUT || transactionType == TransactionType::MINED ||
        transactionType == TransactionType::INOUT) {
      return QString(tr("me (%1)").arg(WalletAdapter::instance().getAddress()));
    } else if (transactionAddress.isEmpty()) {
      return tr("(n/a)");
    }

    QModelIndex contactIndex = AddressBookModel::instance().indexFromContact(transactionAddress,1);
    QString Contact = contactIndex.data(AddressBookModel::ROLE_LABEL).toString();
    if(!Contact.isEmpty())
      return QString("%1 (%2)").arg(Contact, transactionAddress);

    return transactionAddress;
  }

  case COLUMN_AMOUNT: {
    qint64 amount = _index.data(ROLE_AMOUNT).value<qint64>();
    QString amountStr = CurrencyAdapter::instance().formatAmount(qAbs(amount)).remove(',');
    return (amount < 0 ? "-" + amountStr : amountStr);
  }

  case COLUMN_PAYMENT_ID:
    return _index.data(ROLE_PAYMENT_ID);

  case COLUMN_FEE: {
    qint64 fee = _index.data(ROLE_FEE).value<qint64>();
    return CurrencyAdapter::instance().formatAmount(fee);
  }

  case COLUMN_HEIGHT:
    return QString::number(_index.data(ROLE_HEIGHT).value<quint64>());

  default:
    break;
  }

  return QVariant();
}

QVariant TransactionsModel::getEditRole(const QModelIndex& _index) const {
  switch(_index.column()) {

  case COLUMN_STATE: {
    quint64 numberOfConfirmations = _index.data(ROLE_NUMBER_OF_CONFIRMATIONS).value<quint64>();
    return numberOfConfirmations;
  }

  case COLUMN_DATE: {
    QDateTime date = _index.data(ROLE_DATE).toDateTime();
    return (date);
  }

  case COLUMN_HASH:
    return _index.data(ROLE_HASH).toByteArray().toHex().toUpper();

  case COLUMN_SECRET_KEY:
    return _index.data(ROLE_SECRET_KEY).toByteArray().toHex().toUpper();

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
    QString amountStr = CurrencyAdapter::instance().formatAmount(qAbs(amount)).remove(',');
    if (amount < 0) {
      amountStr.insert(0, "-");
    }
    return (amountStr.toDouble());
  }

  case COLUMN_PAYMENT_ID:
    return _index.data(ROLE_PAYMENT_ID);

  case COLUMN_FEE: {
    qint64 fee = _index.data(ROLE_FEE).value<qint64>();
    return CurrencyAdapter::instance().formatAmount(fee);
  }

  case COLUMN_HEIGHT:
    return QString::number(_index.data(ROLE_HEIGHT).value<quint64>());

  default:
    break;
  }

  return QVariant();
}

QVariant TransactionsModel::getToolTipRole(const QModelIndex& _index) const {
  quint64 numberOfConfirmations = _index.data(ROLE_NUMBER_OF_CONFIRMATIONS).value<quint64>();
  TransactionType transactionType = static_cast<TransactionType>(_index.data(ROLE_TYPE).value<quint8>());

  if(numberOfConfirmations == 0) {
    if (transactionType == TransactionType::INPUT)
      return QString(tr("Incoming transaction, unconfirmed"));

    if (transactionType == TransactionType::MINED)
      return QString(tr("Mined, confirmations"));

    if (transactionType == TransactionType::FUSION)
      return QString(tr("Wallet optimization transaction, unconfirmed"));

    if (transactionType == TransactionType::INOUT)
      return QString(tr("Sent to yourself, unconfirmed"));

    if (transactionType == TransactionType::OUTPUT)
      return QString(tr("Outgoing transaction, unconfirmed"));
  } else {
    if (transactionType == TransactionType::INPUT)
      return QString(tr("Incoming transaction, %n confirmation(s)", "", numberOfConfirmations));

    if (transactionType == TransactionType::MINED)
      return QString(tr("Mined, %n confirmation(s)", "", numberOfConfirmations));

    if (transactionType == TransactionType::FUSION)
      return QString(tr("Wallet optimization transaction, %n confirmation(s)", "", numberOfConfirmations));

    if (transactionType == TransactionType::INOUT)
      return QString(tr("Sent to yourself, %n confirmation(s)", "", numberOfConfirmations));

    if (transactionType == TransactionType::OUTPUT)
      return QString(tr("Outgoing transaction, %n confirmation(s)", "", numberOfConfirmations));
  }
  return QVariant();
}

QVariant TransactionsModel::getDecorationRole(const QModelIndex& _index) const {
  if(_index.column() == COLUMN_STATE) {
    quint64 numberOfConfirmations = _index.data(ROLE_NUMBER_OF_CONFIRMATIONS).value<quint64>();
    if(numberOfConfirmations == 0) {
      return QPixmap(":icons/unconfirmed");
    } else if(numberOfConfirmations < 2) {
      return QPixmap(":icons/clock1");
    } else if(numberOfConfirmations < 4) {
      return QPixmap(":icons/clock2");
    } else if(numberOfConfirmations < 6) {
      return QPixmap(":icons/clock3");
    } else if(numberOfConfirmations < 8) {
      return QPixmap(":icons/clock4");
    } else if(numberOfConfirmations < 10) {
      return QPixmap(":icons/clock5");
    } else {
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
  CryptoNote::WalletLegacyTransaction& _transaction, CryptoNote::TransferId _transferId, CryptoNote::WalletLegacyTransfer& _transfer) const {
  switch(_role) {
  case ROLE_DATE:
    return (_transaction.timestamp > 0 ? QDateTime::fromTime_t(_transaction.timestamp) : QDateTime());

  case ROLE_TYPE: {
    QString transactionAddress = _index.data(ROLE_ADDRESS).toString();
    if(_transaction.isCoinbase) {
      return static_cast<quint8>(TransactionType::MINED);
    } else if (WalletAdapter::instance().isFusionTransaction(_transaction)) {
      return static_cast<quint8>(TransactionType::FUSION);
    } else if (!transactionAddress.compare(WalletAdapter::instance().getAddress())) {
      return static_cast<quint8>(TransactionType::INOUT);
    } else if (_transaction.totalAmount < 0) {
      return static_cast<quint8>(TransactionType::OUTPUT);
    }

    return static_cast<quint8>(TransactionType::INPUT);
  }

  case ROLE_HASH:
    return QByteArray(reinterpret_cast<char*>(&_transaction.hash), sizeof(_transaction.hash));

  case ROLE_SECRET_KEY: {
    if (_transaction.secretKey) {
      Crypto::SecretKey txkey = _transaction.secretKey.get();
      if (txkey != CryptoNote::NULL_SECRET_KEY) {
        return QByteArray(reinterpret_cast<char*>(&txkey), sizeof(txkey));
      }
    }
  }

  case ROLE_ADDRESS:
    return QString::fromStdString(_transfer.address);

  case ROLE_AMOUNT:
    return static_cast<qint64>(_transferId == CryptoNote::WALLET_LEGACY_INVALID_TRANSFER_ID ? _transaction.totalAmount : -_transfer.amount);

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

void TransactionsModel::localBlockchainUpdated(quint64 _height) {
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
