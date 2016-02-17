// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDateTime>
#include <QDebug>
#include <QMetaEnum>

#include "DepositModel.h"

#include "TransactionsModel.h"
#include "CurrencyAdapter.h"
#include "NodeAdapter.h"
#include "WalletAdapter.h"

Q_DECLARE_METATYPE(CryptoNote::TransactionId)

namespace WalletGui {

Q_DECL_CONSTEXPR quint32 YEAR_SECONDS = 365 * 24 * 60 * 60;

namespace {

QDateTime getExpectedTimeForHeight(quint64 _height) {
  quint64 lastLocalBlockHeight = NodeAdapter::instance().getLastLocalBlockHeight();
  QDateTime localLocalBlockTimestamp = NodeAdapter::instance().getLastLocalBlockTimestamp();

  return localLocalBlockTimestamp.addSecs((_height - lastLocalBlockHeight) * CurrencyAdapter::instance().getDifficultyTarget());
}

}

enum class MessageType : quint8 {INPUT, OUTPUT};

const int DEPOSIT_MODEL_COLUMN_COUNT =
  DepositModel::staticMetaObject.enumerator(DepositModel::staticMetaObject.indexOfEnumerator("Columns")).keyCount();

DepositModel& DepositModel::instance() {
  static DepositModel inst;
  return inst;
}

DepositModel::DepositModel() : QAbstractItemModel(), m_depositCount(0) {
  connect(&WalletAdapter::instance(), &WalletAdapter::reloadWalletTransactionsSignal, this, &DepositModel::reloadWalletDeposits,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletTransactionCreatedSignal, this,
    static_cast<void(DepositModel::*)(CryptoNote::TransactionId)>(&DepositModel::transactionCreated), Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletTransactionUpdatedSignal, this,
    &DepositModel::transactionUpdated, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &DepositModel::reset,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletDepositsUpdatedSignal, this, &DepositModel::depositsUpdated,
    Qt::QueuedConnection);
}

DepositModel::~DepositModel() {
}

Qt::ItemFlags DepositModel::flags(const QModelIndex& _index) const {
  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsSelectable;

  return flags;
}

int DepositModel::columnCount(const QModelIndex& _parent) const {
  return DEPOSIT_MODEL_COLUMN_COUNT;
}

int DepositModel::rowCount(const QModelIndex& _parent) const {
  return m_depositCount;
}

QVariant DepositModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  if(_orientation != Qt::Horizontal) {
    return QVariant();
  }

  switch(_role) {
  case Qt::DisplayRole:
    switch(_section) {
    case COLUMN_STATE:
      return tr("Status");
    case COLUMN_AMOUNT:
      return tr("Amount");
    case COLUMN_INTEREST:
      return tr("Interest");
    case COLUMN_SUM:
      return tr("Sum");
    case COLUMN_YEAR_RATE:
      return tr("Rate");
    case COLUMN_TERM:
      return tr("Term");
    case COLUMN_UNLOCK_HEIGHT:
      return tr("Unlock height");
    case COLUMN_UNLOCK_TIME:
      return tr("Unlock time");
    case COLUMN_CREATRING_TRANSACTION_HASH:
      return tr("Creating transaction");
    case COLUMN_CREATING_HEIGHT:
      return tr("Creating height");
    case COLUMN_CREATING_TIME:
      return tr("Creating time");
    case COLUMN_SPENDING_TRANSACTION_HASH:
      return tr("Spending transaction");
    case COLUMN_SPENDING_HEIGHT:
      return tr("Spending height");
    case COLUMN_SPENDING_TIME:
      return tr("Spending time");
    default:
      break;
    }

  case ROLE_COLUMN:
    return _section;
  }

  return QVariant();
}

QVariant DepositModel::data(const QModelIndex& _index, int _role) const {
  if(!_index.isValid()) {
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
    return getUserRole(_index, _role);
  }

  return QVariant();
}

QModelIndex DepositModel::index(int _row, int _column, const QModelIndex& _parent) const {
  if(_parent.isValid()) {
    return QModelIndex();
  }

  return createIndex(_row, _column, _row);
}

QModelIndex DepositModel::parent(const QModelIndex& _index) const {
  return QModelIndex();
}

qreal DepositModel::calculateRate(quint64 _amount, quint64 _interest, quint32 _term) {
  quint64 termInSeconds = _term * CurrencyAdapter::instance().getDifficultyTarget();
  return (static_cast<qreal>(_interest)) / _amount * (static_cast<qreal>(YEAR_SECONDS) / termInSeconds);
}

QVariant DepositModel::getDisplayRole(const QModelIndex& _index) const {
  switch(_index.column()) {
  case COLUMN_STATE: {
    DepositState depositState = static_cast<DepositState>(_index.data(ROLE_STATE).toInt());
    switch (depositState) {
    case STATE_LOCKED:
      return tr("Locked");
    case STATE_UNLOCKED:
      return tr("Unlocked");
    case STATE_SPENT:
      return tr("Spent");
    }
  }

  case COLUMN_AMOUNT:
    return CurrencyAdapter::instance().formatAmount(_index.data(ROLE_DEPOSIT_AMOUNT).value<quint64>());
  case COLUMN_INTEREST:
    return CurrencyAdapter::instance().formatAmount(_index.data(ROLE_DEPOSIT_INTEREST).value<quint64>());
  case COLUMN_SUM:
    return CurrencyAdapter::instance().formatAmount(_index.data(ROLE_DEPOSIT_AMOUNT).value<quint64>() +
      _index.data(ROLE_DEPOSIT_INTEREST).value<quint64>());
  case COLUMN_YEAR_RATE: {
    quint64 amount = _index.data(ROLE_DEPOSIT_AMOUNT).value<quint64>();
    quint64 interest = _index.data(ROLE_DEPOSIT_INTEREST).value<quint64>();
    quint32 term = _index.data(ROLE_DEPOSIT_TERM).value<quint32>();
    qreal yearRate = calculateRate(amount, interest, term);
    return QString("%1 %").arg(QString::number(yearRate * 100, 'f', 2));
  }
  case COLUMN_TERM:
    return _index.data(ROLE_DEPOSIT_TERM);
  case COLUMN_UNLOCK_HEIGHT: {
    quint64 unlockHeight = _index.data(ROLE_UNLOCK_HEIGHT).value<quint64>();
    if (unlockHeight == CryptoNote::WALLET_LEGACY_UNCONFIRMED_TRANSACTION_HEIGHT) {
      return "-";
    }

    return unlockHeight;
  }

  case COLUMN_UNLOCK_TIME: {
    DepositState depositState = static_cast<DepositState>(_index.data(ROLE_STATE).toInt());
    if (depositState == STATE_LOCKED) {
      quint64 unlockHeight = _index.data(ROLE_UNLOCK_HEIGHT).value<quint64>();
      if (unlockHeight == CryptoNote::WALLET_LEGACY_UNCONFIRMED_TRANSACTION_HEIGHT) {
        return "-";
      }

      return getExpectedTimeForHeight(unlockHeight).toString("dd-MM-yy HH:mm");
    } else {
      return QDateTime();
    }
  }

  case COLUMN_CREATRING_TRANSACTION_HASH: {
    CryptoNote::TransactionId creatingTransactionId = _index.data(ROLE_CREATING_TRANSACTION_ID).value<CryptoNote::TransactionId>();
    return TransactionsModel::instance().index(creatingTransactionId, TransactionsModel::COLUMN_HASH).data();
  }

  case COLUMN_CREATING_HEIGHT: {
    CryptoNote::TransactionId creatingTransactionId = _index.data(ROLE_CREATING_TRANSACTION_ID).value<CryptoNote::TransactionId>();
    if (creatingTransactionId == CryptoNote::WALLET_LEGACY_INVALID_TRANSACTION_ID) {
      return "-";
    }

    return TransactionsModel::instance().index(creatingTransactionId, TransactionsModel::COLUMN_HEIGHT).data();
  }

  case COLUMN_CREATING_TIME: {
    CryptoNote::TransactionId creatingTransactionId = _index.data(ROLE_CREATING_TRANSACTION_ID).value<CryptoNote::TransactionId>();
    if (creatingTransactionId == CryptoNote::WALLET_LEGACY_INVALID_TRANSACTION_ID) {
      return "-";
    }

    return TransactionsModel::instance().index(creatingTransactionId, TransactionsModel::COLUMN_DATE).data();
  }

  case COLUMN_SPENDING_TRANSACTION_HASH: {
    CryptoNote::TransactionId spendingTransactionId = _index.data(ROLE_SPENDING_TRANSACTION_ID).value<CryptoNote::TransactionId>();
    if (spendingTransactionId == CryptoNote::WALLET_LEGACY_INVALID_TRANSACTION_ID) {
      return "-";
    }

    return TransactionsModel::instance().index(spendingTransactionId, TransactionsModel::COLUMN_HASH).data();
  }

  case COLUMN_SPENDING_HEIGHT: {
    CryptoNote::TransactionId spendingTransactionId = _index.data(ROLE_SPENDING_TRANSACTION_ID).value<CryptoNote::TransactionId>();
    if (spendingTransactionId == CryptoNote::WALLET_LEGACY_INVALID_TRANSACTION_ID) {
      return "-";
    }

    return TransactionsModel::instance().index(spendingTransactionId, TransactionsModel::COLUMN_HEIGHT).data();
  }

  case COLUMN_SPENDING_TIME: {
    CryptoNote::TransactionId spendingTransactionId = _index.data(ROLE_SPENDING_TRANSACTION_ID).value<CryptoNote::TransactionId>();
    if (spendingTransactionId == CryptoNote::WALLET_LEGACY_INVALID_TRANSACTION_ID) {
      return "-";
    }

    return TransactionsModel::instance().index(spendingTransactionId, TransactionsModel::COLUMN_DATE).data();
  }

  default:
    break;
  }

  return QVariant();
}

QVariant DepositModel::getDecorationRole(const QModelIndex& _index) const {
  return QVariant();
}

QVariant DepositModel::getAlignmentRole(const QModelIndex& _index) const {
  return headerData(_index.column(), Qt::Horizontal, Qt::TextAlignmentRole);
}

QVariant DepositModel::getUserRole(const QModelIndex& _index, int _role) const {
  CryptoNote::Deposit deposit;

  if(!WalletAdapter::instance().getDeposit(_index.row(), deposit)) {
    return QVariant();
  }

  switch(_role) {
  case ROLE_DEPOSIT_TERM:
    return deposit.term;

  case ROLE_DEPOSIT_AMOUNT:
    return static_cast<quint64>(deposit.amount);

  case ROLE_DEPOSIT_INTEREST:
    return static_cast<quint64>(deposit.interest);

  case ROLE_STATE:
    if (deposit.locked) {
      return static_cast<int>(STATE_LOCKED);
    } else if (deposit.spendingTransactionId == CryptoNote::WALLET_LEGACY_INVALID_TRANSACTION_ID) {
      return static_cast<int>(STATE_UNLOCKED);
    } else {
      return static_cast<int>(STATE_SPENT);
    }

    return QVariant();

  case ROLE_CREATING_TRANSACTION_ID:
    return static_cast<quintptr>(deposit.creatingTransactionId);

  case ROLE_SPENDING_TRANSACTION_ID:
    return static_cast<quintptr>(deposit.spendingTransactionId);

  case ROLE_UNLOCK_HEIGHT: {
    CryptoNote::TransactionId creatingTransactionId = _index.data(ROLE_CREATING_TRANSACTION_ID).value<CryptoNote::TransactionId>();
    quint64 creatingHeight = TransactionsModel::instance().index(creatingTransactionId, 0).
      data(TransactionsModel::ROLE_HEIGHT).value<quint64>();
    if (creatingHeight == CryptoNote::WALLET_LEGACY_UNCONFIRMED_TRANSACTION_HEIGHT) {
      return static_cast<const quint64>(CryptoNote::WALLET_LEGACY_UNCONFIRMED_TRANSACTION_HEIGHT);
    }

    return creatingHeight + _index.data(ROLE_DEPOSIT_TERM).value<quint32>();
  }

  case ROLE_ROW:
    return _index.row();

  case ROLE_COLUMN:
    return headerData(_index.column(), Qt::Horizontal, ROLE_COLUMN);

  }

  return QVariant();
}

void DepositModel::reloadWalletDeposits() {
  reset();

  if (WalletAdapter::instance().getDepositCount() == 0) {
    return;
  }

  beginInsertRows(QModelIndex(), 0, WalletAdapter::instance().getDepositCount() - 1);
  m_depositCount = WalletAdapter::instance().getDepositCount();
  endInsertRows();
}

void DepositModel::appendDeposit(CryptoNote::DepositId _depositId) {
  if (_depositId < m_depositCount) {
    return;
  }

  beginInsertRows(QModelIndex(), m_depositCount, _depositId);
  m_depositCount = _depositId + 1;
  endInsertRows();
}

void DepositModel::transactionCreated(CryptoNote::TransactionId _transactionId) {
  if (m_depositCount < WalletAdapter::instance().getDepositCount()) {
    appendDeposit(WalletAdapter::instance().getDepositCount() - 1);
  }
}

void DepositModel::reset() {
  beginResetModel();
  m_depositCount = 0;
  endResetModel();
}

void DepositModel::depositsUpdated(const QVector<CryptoNote::DepositId>& _depositIds) {
  Q_FOREACH (const auto& depositId, _depositIds) {
    Q_EMIT dataChanged(index(depositId, 0), index(depositId, columnCount() - 1));
  }
}

void DepositModel::transactionUpdated(CryptoNote::TransactionId _transactionId) {
  QModelIndex transactionIndex = TransactionsModel::instance().index(_transactionId, 0);
  if (!transactionIndex.isValid()) {
    return;
  }

  quintptr firstDepositId = transactionIndex.data(TransactionsModel::ROLE_DEPOSIT_ID).value<quintptr>();
  quintptr depositCount = transactionIndex.data(TransactionsModel::ROLE_DEPOSIT_COUNT).value<quintptr>();
  Q_EMIT dataChanged(index(firstDepositId, 0), index(firstDepositId + depositCount - 1, columnCount() - 1));
}

}
