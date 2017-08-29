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

#include <QDateTime>
#include <QFont>
#include <QLocale>
#include <QMetaEnum>
#include <QPixmap>
#include <QPixmapCache>
#include <QTimerEvent>

#include "TransactionsModel.h"
#include "NodeStateModel.h"
#include "WalletLogger/WalletLogger.h"
#include "ICryptoNoteAdapter.h"

Q_DECLARE_METATYPE(QList<CryptoNote::WalletTransfer>)

namespace WalletGui {

namespace {

const int SYNC_TIMER_INTERVAL = 1000;

}

enum class TransactionType : quint8 {
  MINED, INPUT, OUTPUT, FUSION, DONATION
};

namespace {

TransactionType getTransactionType(const FullTransactionInfo &_transaction) {
  if (_transaction.walletTransaction.isBase) {
    return TransactionType::MINED;
  } else if (_transaction.isFusionTransaction) {
    return TransactionType::FUSION;
  } else if (_transaction.walletTransaction.totalAmount < 0) {
    return TransactionType::OUTPUT;
  }

  return TransactionType::INPUT;
}

QString getIconPrefix(TransactionType _type) {
  switch(_type) {
  case TransactionType::INPUT:
    return "inp_";
  case TransactionType::OUTPUT:
    return "out_";
  case TransactionType::MINED:
    return "mined_";
  case TransactionType::FUSION:
    return "opt_";
  case TransactionType::DONATION:
    return "donation_";
  }

  return QString();
}

}

TransactionsModel::TransactionsModel(ICryptoNoteAdapter* _cryptoNoteAdapter, IOptimizationManager* _optimizationManager, QAbstractItemModel* _nodeStateModel, QObject* _parent) :
  QAbstractItemModel(_parent), m_cryptoNoteAdapter(_cryptoNoteAdapter), m_nodeStateModel(_nodeStateModel),
  m_columnCount(TransactionsModel::staticMetaObject.enumerator(TransactionsModel::staticMetaObject.indexOfEnumerator("Columns")).keyCount()),
  m_syncTimerId(-1), m_lastVisibleTransactionIndex(CryptoNote::WALLET_INVALID_TRANSACTION_ID), m_isSynchronized(false) {
  m_cryptoNoteAdapter->addObserver(this);
  INodeAdapter* nodeAdapter = m_cryptoNoteAdapter->getNodeAdapter();
  IWalletAdapter* walletAdapter = nodeAdapter->getWalletAdapter();
  walletAdapter->addObserver(this);
  _optimizationManager->addObserver(this);
  connect(m_nodeStateModel, &QAbstractItemModel::dataChanged, this, &TransactionsModel::nodeStateChanged);
}

TransactionsModel::~TransactionsModel() {
}

Qt::ItemFlags TransactionsModel::flags(const QModelIndex& _index) const {
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
}

int TransactionsModel::columnCount(const QModelIndex& _parent) const {
  return m_columnCount;
}

int TransactionsModel::rowCount(const QModelIndex& _parent) const {
  if (m_lastVisibleTransactionIndex == CryptoNote::WALLET_INVALID_TRANSACTION_ID) {
    return 0;
  }

  return m_lastVisibleTransactionIndex + 1;
}

QVariant TransactionsModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  if (_orientation != Qt::Horizontal) {
    return QVariant();
  }

  switch(_role) {
  case Qt::DisplayRole:
    switch(_section) {
    case COLUMN_STATE:
      return tr("State");
    case COLUMN_TIME:
      return tr("Date");
    case COLUMN_HASH:
      return tr("Hash");
    case COLUMN_TRANSFERS:
      return tr("Transfers");
    case COLUMN_AMOUNT:
      return tr("Amount");
    case COLUMN_PAYMENT_ID:
      return tr("Payment ID");
    case COLUMN_HEIGHT:
      return tr("Height");
    case COLUMN_FEE:
      return tr("Fee");
    default:
      break;
    }

    break;
  case Qt::TextAlignmentRole: {
    if (_section == COLUMN_TIME) {
      return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
    }

    return static_cast<int>(Qt::AlignRight | Qt::AlignVCenter);
  }
  case Qt::FontRole: {
    QFont font;
    font.setPixelSize(11);
    return font;
  }
  case ROLE_COLUMN:
    return _section;
  }

  return QVariant();
}

QVariant TransactionsModel::data(const QModelIndex& _index, int _role) const {
  if (!_index.isValid()) {
    return QVariant();
  }

  switch(_role) {
  case Qt::DisplayRole:
    return getDisplayRoleData(_index);
  case Qt::EditRole:
    return getEditRoleData(_index);
  case Qt::DecorationRole:
    return getDecorationRoleData(_index);
  case Qt::TextAlignmentRole:
    return headerData(_index.column(), Qt::Horizontal, _role);
  case Qt::ToolTipRole:
    return getToolTipRole(_index);
  default:
    return getUserRolesData(_index, _role);
  }

  return QVariant();
}

QModelIndex TransactionsModel::index(int _row, int _column, const QModelIndex& _parent) const {
  if (!hasIndex(_row, _column, _parent)) {
    return QModelIndex();
  }

  return createIndex(_row, _column, _row);
}

QModelIndex TransactionsModel::parent(const QModelIndex& _index) const {
  return QModelIndex();
}

bool TransactionsModel::setData(const QModelIndex& _index, const QVariant& _value, int _role) {
  if (_role == ROLE_SHOW_TRANSFERS) {
    m_showTransfers.setBit(_index.row(), _value.toBool());
    Q_EMIT dataChanged(_index.sibling(_index.row(), COLUMN_TRANSFERS), _index, QVector<int>() << _role);
    return true;
  }

  return false;
}

void TransactionsModel::walletOpened() {
  Q_ASSERT(m_transactions.isEmpty());
  Q_ASSERT(m_showTransfers.isEmpty());
  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  QHash<quintptr, FullTransactionInfo> transactionInfos;
  if (!walletAdapter->getAllTransactions(transactionInfos)) {
    return;
  }

  if (!transactionInfos.isEmpty()) {
    beginInsertRows(QModelIndex(), 0, transactionInfos.size() - 1);
    m_transactions = std::move(transactionInfos);
    m_showTransfers.resize(m_transactions.size());
    m_showTransfers.fill(false, 0, m_transactions.size());
    for (quintptr i = 0; i < m_transactions.size(); ++i) {
      if (m_transactions[i].isFusionTransaction) {
        m_fusionTransactions.insert(i);
      }
    }

    m_lastVisibleTransactionIndex = m_transactions.size() - 1;
    endInsertRows();
  }
}

void TransactionsModel::walletOpenError(int _initStatus) {
  // Do nothing
}

void TransactionsModel::walletClosed() {
  ensureSyncTimerStopped();
  beginResetModel();
  m_lastVisibleTransactionIndex = CryptoNote::WALLET_INVALID_TRANSACTION_ID;
  m_isSynchronized = false;
  m_transactions.clear();
  m_fusionTransactions.clear();
  m_showTransfers.clear();
  endResetModel();
}

void TransactionsModel::passwordChanged() {
  // Do nothing
}

void TransactionsModel::synchronizationProgressUpdated(quint32 _current, quint32 _total) {
  ensureSyncTimerStarted();
  m_isSynchronized = false;
}

void TransactionsModel::synchronizationCompleted() {
  ensureSyncTimerStopped();
  if (m_isSynchronized) {
    return;
  }

  m_isSynchronized = true;
  showAllCachedTransactions();
}

void TransactionsModel::balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) {
  // Do nothing
}

void TransactionsModel::externalTransactionCreated(quintptr _transactionIndex, const FullTransactionInfo& _transaction) {
  if (_transactionIndex < rowCount()) {
    return;
  }

  m_transactions.insert(_transactionIndex, std::move(_transaction));
  if (_transaction.isFusionTransaction) {
    m_fusionTransactions.insert(_transactionIndex);
  }

  if (m_isSynchronized) {
    showAllCachedTransactions();
  }
}

void TransactionsModel::transactionUpdated(quintptr _transactionIndex, const FullTransactionInfo& _transaction) {
  if (_transactionIndex < m_transactions.size()) {
    updateTransaction(_transactionIndex, _transaction);
  }
}

void TransactionsModel::cryptoNoteAdapterInitCompleted(int _status) {
  if (_status == 0) {
    INodeAdapter* nodeAdapter = m_cryptoNoteAdapter->getNodeAdapter();
    IWalletAdapter* walletAdapter = nodeAdapter->getWalletAdapter();
    walletAdapter->addObserver(this);
  }
}

void TransactionsModel::cryptoNoteAdapterDeinitCompleted() {
  // Do nothing
}

void TransactionsModel::fusionTransactionsVisibilityChanged(bool _isVisible) {
  if (rowCount() > 0) {
    for (quintptr transactionIndex : m_fusionTransactions) {
      Q_EMIT dataChanged(index(transactionIndex, 0), index(transactionIndex, 0), QVector<int>() << ROLE_IS_FUSION_TRANSACTION);
    }
  }
}

QByteArray TransactionsModel::toCsv() const {
  QByteArray res;
  res.append("\"State\",\"Date\",\"Amount\",\"Fee\",\"Hash\",\"Height\",\"Transfers\",\"Payment ID\"\n");
  for (qint32 row = 0; row < rowCount(); ++row) {
    QModelIndex ind = index(row, 0);
    res.append("\"").append(ind.sibling(row, COLUMN_STATE).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.data(ROLE_TIME).toDateTime().toString("yyyy-MM-dd HH:mm:ss").toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_AMOUNT).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.sibling(row, COLUMN_FEE).data().toString().toUtf8()).append("\",");
    res.append("\"").append(ind.data(ROLE_HASH).toByteArray().toHex()).append("\",");
    res.append("\"").append(ind.data(ROLE_HEIGHT).toString().toUtf8()).append("\",");
    res.append("\"").append(ind.data(ROLE_TRANSFER_COUNT).toString().toUtf8()).append("\",");
    res.append("\"").append(ind.data(ROLE_PAYMENT_ID).toString().toUtf8()).append("\"\n");
  }

  return res;
}

int TransactionsModel::findProxyColumn(QAbstractItemModel* _proxyModel, int _originColumn) {
  for (int i = 0; i < _proxyModel->columnCount(); ++i) {
    if (_proxyModel->headerData(i, Qt::Horizontal, ROLE_COLUMN).toInt() == _originColumn) {
      return i;
    }
  }

  return -1;
}

void TransactionsModel::ensureSyncTimerStarted() {
  if (m_syncTimerId == -1) {
    m_syncTimerId = startTimer(SYNC_TIMER_INTERVAL);
  }
}

void TransactionsModel::ensureSyncTimerStopped() {
  if (m_syncTimerId != -1) {
    killTimer(m_syncTimerId);
    m_syncTimerId = -1;
  }
}

void TransactionsModel::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_syncTimerId) {
    if (m_lastVisibleTransactionIndex != m_transactions.size() - 1) {
      showAllCachedTransactions();
    }

    return;
  }

  QAbstractItemModel::timerEvent(_event);
}

void TransactionsModel::nodeStateChanged(const QModelIndex& _topLeft, const QModelIndex& _bottomRight, const QVector<int>& _roles) {
  if (_roles.contains(NodeStateModel::ROLE_KNOWN_BLOCK_COUNT) && rowCount() > 0) {
    Q_EMIT dataChanged(index(0, COLUMN_AMOUNT), index(rowCount() - 1, COLUMN_AMOUNT), QVector<int>() << Qt::DecorationRole);
    Q_EMIT dataChanged(index(0, COLUMN_STATE), index(rowCount() - 1, COLUMN_STATE), QVector<int>() << Qt::EditRole);
  }
}

QVariant TransactionsModel::getDisplayRoleData(const QModelIndex &_index) const {
  switch(_index.column()) {
  case COLUMN_STATE: {
    CryptoNote::WalletTransactionState transactionState =
      static_cast<CryptoNote::WalletTransactionState>(_index.data(ROLE_STATE).value<quint8>());
    if (transactionState == CryptoNote::WalletTransactionState::FAILED) {
      return tr("Failed");
    } else if (transactionState == CryptoNote::WalletTransactionState::CANCELLED) {
      return tr("Cancelled");
    }

    quint32 transactionConfirmationCount = _index.data(ROLE_NUMBER_OF_CONFIRMATIONS).value<quint32>();
    QString transactionConfirmationState = transactionConfirmationCount < 1 ? tr("unconfirmed") : tr("confirmed");
    return QString("%1/%2").arg(transactionConfirmationCount).arg(transactionConfirmationState);
  }

  case COLUMN_TIME: {
    CryptoNote::WalletTransactionState transactionState =
      static_cast<CryptoNote::WalletTransactionState>(_index.data(ROLE_STATE).value<quint8>());
    QDateTime date = _index.data(ROLE_TIME).toDateTime();
    return (date.isNull() || !date.isValid() ? "-" : QLocale(QLocale::English).toString(date, "dd MMM yyyy, HH:mm:ss"));
  }

  case COLUMN_HASH:
    return _index.data(ROLE_HASH).toByteArray().toHex();
  case COLUMN_TRANSFERS:
    return _index.data(ROLE_TRANSFER_COUNT);
  case COLUMN_AMOUNT:
    return m_cryptoNoteAdapter->formatAmount(_index.data(ROLE_AMOUNT).value<qint64>());
  case COLUMN_PAYMENT_ID:
    return _index.data(ROLE_PAYMENT_ID);
  case COLUMN_HEIGHT:
    return QString::number(_index.data(ROLE_HEIGHT).value<quint32>());
  case COLUMN_FEE:
    return m_cryptoNoteAdapter->formatAmount(_index.data(ROLE_FEE).value<qint64>());
  case COLUMN_SHOW_TRANSFERS:
    return _index.data(ROLE_SHOW_TRANSFERS);
  default:
    break;
  }

  return QVariant();
}

QVariant TransactionsModel::getEditRoleData(const QModelIndex& _index) const {
  return getDisplayRoleData(_index);
}

QVariant TransactionsModel::getDecorationRoleData(const QModelIndex &_index) const {
  if (_index.column() == COLUMN_STATE || _index.column() == COLUMN_AMOUNT) {
    TransactionType transactionType = static_cast<TransactionType>(_index.data(ROLE_TYPE).value<quint8>());
    quint32 transactionConfirmationCount = _index.data(ROLE_NUMBER_OF_CONFIRMATIONS).value<quint32>();
    QString iconPrefix = getIconPrefix(transactionType);
    Q_ASSERT(!iconPrefix.isEmpty());
    CryptoNote::WalletTransactionState transactionState =
      static_cast<CryptoNote::WalletTransactionState>(_index.data(ROLE_STATE).value<quint8>());
    QString file;
    if (transactionState == CryptoNote::WalletTransactionState::FAILED) {
      file = ":icons/out_failed";
    } else if (transactionType == TransactionType::FUSION) {
      file = QString(":icons/%1confirmed").arg(iconPrefix);
    } else {
      if (transactionConfirmationCount == 0) {
        file = QString(":icons/%1unconfirmed").arg(iconPrefix);
      } else if (transactionConfirmationCount < 2) {
        file = QString(":icons/%1clock1").arg(iconPrefix);
      } else if (transactionConfirmationCount < 4) {
        file = QString(":icons/%1clock2").arg(iconPrefix);
      } else if (transactionConfirmationCount < 6) {
        file = QString(":icons/%1clock3").arg(iconPrefix);
      } else if (transactionConfirmationCount < 8) {
        file = QString(":icons/%1clock4").arg(iconPrefix);
      } else if (transactionConfirmationCount < 10) {
        file = QString(":icons/%1clock5").arg(iconPrefix);
      } else {
        file = QString(":icons/%1confirmed").arg(iconPrefix);
      }
    }

    QPixmap pixmap;
    if (!QPixmapCache::find(file, pixmap)) {
      pixmap.load(file);
      QPixmapCache::insert(file, pixmap);
    }

    return pixmap;
  }

  return QVariant();
}

QVariant TransactionsModel::getToolTipRole(const QModelIndex& _index) const {
  if (_index.column() == COLUMN_AMOUNT && _index.data(ROLE_IS_FUSION_TRANSACTION).toBool()) {
    return tr("Fusion transaction helps you to optimize your wallet.");
  }

  return QVariant();
}

QVariant TransactionsModel::getUserRolesData(const QModelIndex &_index, int _role) const {
  const FullTransactionInfo& transaction = m_transactions[_index.row()];

  switch (_role) {
    case ROLE_TIME:
      return (transaction.walletTransaction.timestamp > 0 ? QDateTime::fromTime_t(transaction.walletTransaction.timestamp) : QDateTime());
    case ROLE_RAW_TIME:
      return static_cast<quint64>(transaction.walletTransaction.timestamp);
    case ROLE_TYPE: {
      return static_cast<quint8>(getTransactionType(transaction));
    }
    case ROLE_HASH:
      return QByteArray(reinterpret_cast<const char*>(&transaction.walletTransaction.hash), sizeof(transaction.walletTransaction.hash));
    case ROLE_TRANSFER_COUNT:
      return transaction.transfers.size();
    case ROLE_AMOUNT:
      return static_cast<qint64>(transaction.walletTransaction.totalAmount);
    case ROLE_PAYMENT_ID:
      return m_cryptoNoteAdapter->extractPaymentIdFromExtra(transaction.walletTransaction.extra);
    case ROLE_TRANSACTION_ID:
      return _index.row();
    case ROLE_HEIGHT:
      return static_cast<quint32>(transaction.walletTransaction.blockHeight == CryptoNote::WALLET_UNCONFIRMED_TRANSACTION_HEIGHT ?
        0 : transaction.walletTransaction.blockHeight);
    case ROLE_FEE:
      return static_cast<qint64>(transaction.walletTransaction.fee);
    case ROLE_NUMBER_OF_CONFIRMATIONS: {
      quint32 knownBlockCount = m_nodeStateModel->index(0, NodeStateModel::COLUMN_KNOWN_BLOCK_COUNT).data(NodeStateModel::ROLE_KNOWN_BLOCK_COUNT).value<quint32>();
      return (transaction.walletTransaction.blockHeight == CryptoNote::WALLET_UNCONFIRMED_TRANSACTION_HEIGHT ? 0 :
         knownBlockCount - transaction.walletTransaction.blockHeight);
    }

    case ROLE_COLUMN:
      return headerData(_index.column(), Qt::Horizontal, _role);
    case ROLE_ROW:
      return _index.row();
    case ROLE_STATE:
      return static_cast<quint8>(transaction.walletTransaction.state);
    case ROLE_IS_FUSION_TRANSACTION:
      return transaction.isFusionTransaction;
    case ROLE_SHOW_TRANSFERS:
      return m_showTransfers.at(_index.row());
    case ROLE_TRANSFERS:
      return QVariant::fromValue<QList<CryptoNote::WalletTransfer>>(transaction.transfers);
    default:
      break;
    }

    return QVariant();
}

void TransactionsModel::showAllCachedTransactions() {
  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  if (!walletAdapter->isOpen() || m_transactions.isEmpty()) {
    return;
  }

  quintptr firstIndex = (m_lastVisibleTransactionIndex == CryptoNote::WALLET_INVALID_TRANSACTION_ID ? 0 : m_lastVisibleTransactionIndex + 1);
  quintptr lastIndex = m_transactions.size() - 1;

  if (firstIndex > lastIndex)
	  return;

  beginInsertRows(QModelIndex(), firstIndex, lastIndex);
  m_showTransfers.resize(lastIndex + 1);
  m_showTransfers.fill(false, firstIndex, lastIndex);
  m_lastVisibleTransactionIndex = m_transactions.size() - 1;
  endInsertRows();
  WalletLogger::debug(tr("[Transactions] Transactions inserted: last=%1").arg(m_transactions.size() - 1));
}

void TransactionsModel::updateTransaction(quintptr _transactionIndex, const FullTransactionInfo& _transaction) {
  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  if (!walletAdapter->isOpen()) {
    return;
  }

  const FullTransactionInfo oldTransactionInfo = m_transactions[_transactionIndex];
  m_transactions[_transactionIndex] = _transaction;

  if (oldTransactionInfo.walletTransaction.blockHeight != _transaction.walletTransaction.blockHeight) {
    Q_EMIT dataChanged(index(_transactionIndex, COLUMN_STATE), index(_transactionIndex, COLUMN_STATE));
    Q_EMIT dataChanged(index(_transactionIndex, COLUMN_TIME), index(_transactionIndex, COLUMN_TIME));
    Q_EMIT dataChanged(index(_transactionIndex, COLUMN_HEIGHT), index(_transactionIndex, COLUMN_HEIGHT));
  }

  if (oldTransactionInfo.walletTransaction.state != _transaction.walletTransaction.state) {
    Q_EMIT dataChanged(index(_transactionIndex, COLUMN_STATE), index(_transactionIndex, COLUMN_STATE));
  }

  if (oldTransactionInfo.transfers.size() != _transaction.transfers.size()) {
    Q_EMIT dataChanged(index(_transactionIndex, COLUMN_TRANSFERS), index(_transactionIndex, COLUMN_SHOW_TRANSFERS),
      QVector<int>() << ROLE_TRANSFERS);
  }

  if (oldTransactionInfo.isFusionTransaction != _transaction.isFusionTransaction) {
    Q_EMIT dataChanged(index(_transactionIndex, 0), index(_transactionIndex, 0), QVector<int>() << ROLE_IS_FUSION_TRANSACTION);
  }
}

}
