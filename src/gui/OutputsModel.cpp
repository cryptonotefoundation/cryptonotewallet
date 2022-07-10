// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2022 Karbo developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QMetaEnum>

#include "CryptoNoteCore/CryptoNoteTools.h"
#include "Common/StringTools.h"
#include "CurrencyAdapter.h"
#include "NodeAdapter.h"
#include "OutputsModel.h"
#include "WalletAdapter.h"

namespace WalletGui {

const int OUTPUTS_MODEL_COLUMN_COUNT =
  OutputsModel::staticMetaObject.enumerator(OutputsModel::staticMetaObject.indexOfEnumerator("Columns")).keyCount();

OutputsModel::OutputsModel() : QAbstractItemModel()
{
  connect(&WalletAdapter::instance(), &WalletAdapter::reloadWalletTransactionsSignal, this, &OutputsModel::reloadWalletTransactions,
          Qt::QueuedConnection);

  connect(&WalletAdapter::instance(), &WalletAdapter::walletTransactionCreatedSignal, this,
          static_cast<void(OutputsModel::*)(CryptoNote::TransactionId)>(&OutputsModel::appendTransaction), Qt::QueuedConnection);

  connect(&WalletAdapter::instance(), &WalletAdapter::walletTransactionUpdatedSignal, this,
          &OutputsModel::appendTransaction, Qt::QueuedConnection);

  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &OutputsModel::reset,
          Qt::QueuedConnection);
}

OutputsModel::~OutputsModel() {
}

OutputsModel& OutputsModel::instance() {
  static OutputsModel inst;
  return inst;
}

Qt::ItemFlags OutputsModel::flags(const QModelIndex& _index) const {
  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsSelectable;

  return flags;
}

int OutputsModel::columnCount(const QModelIndex& _parent) const {
  return OUTPUTS_MODEL_COLUMN_COUNT;
}

int OutputsModel::rowCount(const QModelIndex& _parent) const {
  return m_utputs.size();
}

QVariant OutputsModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  if(_orientation != Qt::Horizontal) {
    return QVariant();
  }

  switch(_role) {
  case Qt::DisplayRole:
    switch(_section) {
    case COLUMN_STATE:
      return tr("Status");
    case COLUMN_TYPE:
      return tr("Type");
    case COLUMN_OUTPUT_KEY:
      return tr("Public key (stealth address)");
    case COLUMN_TX_HASH:
      return tr("Transaction hash");
    case COLUMN_AMOUNT:
      return tr("Amount");
    case COLUMN_GLOBAL_OUTPUT_INDEX:
      return tr("Global index");
    case COLUMN_OUTPUT_IN_TRANSACTION:
      return tr("Index in transaction");
    case COLUMN_TX_PUBLIC_KEY:
      return tr("Transaction public key");
    case COLUMN_SPENDING_BLOCK_HEIGHT:
      return tr("Spent at height");
    case COLUMN_TIMESTAMP:
      return tr("Timestamp");
    case COLUMN_SPENDING_TRANSACTION_HASH:
      return tr("Spent in transaction");
    case COLUMN_KEY_IMAGE:
      return tr("Key image");
    case COLUMN_INPUT_IN_TRANSACTION:
      return tr("As input");
    default:
      break;
    }

  case ROLE_COLUMN:
    return _section;
  }

  return QVariant();
}

QVariant OutputsModel::data(const QModelIndex& _index, int _role) const {
  if(!_index.isValid()) {
    return QVariant();
  }

  CryptoNote::TransactionSpentOutputInformation _output = m_utputs.value(_index.row());

  switch(_role) {
  case Qt::DisplayRole:
  case Qt::EditRole:
    return getDisplayRole(_index);

  //case Qt::EditRole:
  case 15:
    return getEditRole(_index);

  case Qt::DecorationRole:
    return getDecorationRole(_index);

  case Qt::TextAlignmentRole:
    return getAlignmentRole(_index);

  case Qt::ToolTipRole:
    return getToolTipRole(_index);

  default:
    return getUserRole(_index, _role, _output);
  }

  return QVariant();
}

QModelIndex OutputsModel::index(int _row, int _column, const QModelIndex& _parent) const {
  if(_parent.isValid()) {
    return QModelIndex();
  }

  return createIndex(_row, _column, _row);
}

QModelIndex OutputsModel::parent(const QModelIndex& _index) const {
  return QModelIndex();
}

QVariant OutputsModel::getAlignmentRole(const QModelIndex& _index) const {
  return headerData(_index.column(), Qt::Horizontal, Qt::TextAlignmentRole);
}

QVariant OutputsModel::getDecorationRole(const QModelIndex& _index) const {
  if(_index.column() == COLUMN_STATE) {
    OutputState state = static_cast<OutputState>(_index.data(ROLE_STATE).value<quint8>());
    if (state == OutputState::SPENT) {
      return QPixmap(":icons/tx-output").scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    } else if (state == OutputState::UNSPENT) {
      return QPixmap(":icons/tx-input").scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
  }

  return QVariant();
}

QVariant OutputsModel::getDisplayRole(const QModelIndex& _index) const {
  OutputState state = static_cast<OutputState>(_index.data(ROLE_STATE).value<quint8>());
  bool is_spent = state == OutputState::SPENT;

  OutputType type = static_cast<OutputType>(_index.data(ROLE_TYPE).value<quint8>());

  switch(_index.column()) {

  case COLUMN_STATE: {
    if (is_spent) {
      return tr("Spent");
    } else {
      return tr("Unspent");
    }

    return QVariant();
  }

  case COLUMN_TYPE: {
    if (type == OutputType::Key)
      return tr("Key");
    else if (type == OutputType::Multisignature)
      return tr("Multisignature");
    else
      return tr("Invalid");
  }

  case COLUMN_OUTPUT_KEY:
    return _index.data(ROLE_OUTPUT_KEY).toByteArray().toHex().toUpper();

  case COLUMN_TX_HASH:
    return _index.data(ROLE_TX_HASH).toByteArray().toHex().toUpper();

  case COLUMN_AMOUNT:
    return CurrencyAdapter::instance().formatAmount(_index.data(ROLE_AMOUNT).value<qint64>());

  case COLUMN_GLOBAL_OUTPUT_INDEX: {
    quint32 index = _index.data(ROLE_GLOBAL_OUTPUT_INDEX).value<qint32>();
      if (index < 0 || index == std::numeric_limits<uint32_t>::max())
        return tr("Pending");
      else
        return index;
  }

  case COLUMN_OUTPUT_IN_TRANSACTION:
    return _index.data(ROLE_OUTPUT_IN_TRANSACTION).value<qint32>();

  case COLUMN_TX_PUBLIC_KEY: {
    if (type == OutputType::Key)
      return _index.data(ROLE_TX_PUBLIC_KEY).toByteArray().toHex().toUpper();
    else if (type == OutputType::Multisignature)
      return "-";
  }

  case COLUMN_SPENDING_BLOCK_HEIGHT: {
    if (is_spent) {
      quint32 height = _index.data(ROLE_SPENDING_BLOCK_HEIGHT).value<qint32>();
      if (height < 0 || height == std::numeric_limits<uint32_t>::max())
        return "Unconfirmed";
      else
        return height;
    } else {
      return "-";
    }
  }

  case COLUMN_TIMESTAMP: {
    if (is_spent) {
      QDateTime date = _index.data(ROLE_TIMESTAMP).toDateTime();
      return (date.isNull() || !date.isValid() ? "-" : date.toString("dd-MM-yy HH:mm"));
    } else {
      return "-";
    }
  }

  case COLUMN_SPENDING_TRANSACTION_HASH: {
    if (is_spent)
      return _index.data(ROLE_SPENDING_TRANSACTION_HASH).toByteArray().toHex().toUpper();
    else
      return "-";
  }

  case COLUMN_KEY_IMAGE: {
    if (is_spent)
      return _index.data(ROLE_KEY_IMAGE).toByteArray().toHex().toUpper();
    else
      return "-";
  }

  case COLUMN_INPUT_IN_TRANSACTION: {
    if (is_spent)
      return _index.data(ROLE_INPUT_IN_TRANSACTION).value<qint32>();
    else
      return "-";
  }

  default:
    break;
  }

  return QVariant();
}

QVariant OutputsModel::getEditRole(const QModelIndex& _index) const {
  OutputState state = static_cast<OutputState>(_index.data(ROLE_STATE).value<quint8>());
  bool is_spent = state == OutputState::SPENT;

  OutputType type = static_cast<OutputType>(_index.data(ROLE_TYPE).value<quint8>());

  switch(_index.column()) {

  case COLUMN_STATE: {
    if (is_spent) {
      return tr("Spent");
    } else {
      return tr("Unspent");
    }

    return QVariant();
  }

  case COLUMN_TYPE: {
    if (type == OutputType::Key)
      return tr("Key");
    else if (type == OutputType::Multisignature)
      return tr("Multisignature");
    else
      return tr("Invalid");
  }

  case COLUMN_OUTPUT_KEY:
    return _index.data(ROLE_OUTPUT_KEY).toByteArray().toHex().toUpper();

  case COLUMN_TX_HASH:
    return _index.data(ROLE_TX_HASH).toByteArray().toHex().toUpper();

  case COLUMN_AMOUNT:
    return _index.data(ROLE_AMOUNT).value<qint64>();

  case COLUMN_GLOBAL_OUTPUT_INDEX: {
    quint32 index = _index.data(ROLE_GLOBAL_OUTPUT_INDEX).value<qint32>();
      if (index < 0 || index == std::numeric_limits<uint32_t>::max())
        return tr("Pending");
      else
        return index;
  }

  case COLUMN_OUTPUT_IN_TRANSACTION:
    return _index.data(ROLE_OUTPUT_IN_TRANSACTION).value<qint32>();

  case COLUMN_TX_PUBLIC_KEY: {
    if (type == OutputType::Key)
      return _index.data(ROLE_TX_PUBLIC_KEY).toByteArray().toHex().toUpper();
    else if (type == OutputType::Multisignature)
      return "-";
  }

  case COLUMN_SPENDING_BLOCK_HEIGHT: {
    if (is_spent) {
      quint32 height = _index.data(ROLE_SPENDING_BLOCK_HEIGHT).value<qint32>();
      if (height < 0 || height == std::numeric_limits<uint32_t>::max())
        return "Unconfirmed";
      else
        return height;
    } else {
      return "-";
    }
  }

  case COLUMN_TIMESTAMP: {
    if (is_spent) {
      QDateTime date = _index.data(ROLE_TIMESTAMP).toDateTime();
      return (date.isNull() || !date.isValid() ? "-" : date.toString("dd-MM-yy HH:mm"));
    } else {
      return "-";
    }
  }

  case COLUMN_SPENDING_TRANSACTION_HASH: {
    if (is_spent)
      return _index.data(ROLE_SPENDING_TRANSACTION_HASH).toByteArray().toHex().toUpper();
    else
      return "-";
  }

  case COLUMN_KEY_IMAGE: {
    if (is_spent)
      return _index.data(ROLE_KEY_IMAGE).toByteArray().toHex().toUpper();
    else
      return "-";
  }

  case COLUMN_INPUT_IN_TRANSACTION: {
    if (is_spent)
      return _index.data(ROLE_INPUT_IN_TRANSACTION).value<qint32>();
    else
      return "-";
  }

  default:
    break;
  }

  return QVariant();
}

QVariant OutputsModel::getToolTipRole(const QModelIndex& _index) const {
  OutputState state = static_cast<OutputState>(_index.data(ROLE_STATE).value<quint8>());
  if (state == OutputState::SPENT) {
    return tr("Spent");
  } else if(state == OutputState::UNSPENT) {
    return tr("Unspent");
  }

  return QVariant();
}

QVariant OutputsModel::getUserRole(const QModelIndex& _index, int _role, CryptoNote::TransactionSpentOutputInformation _output) const {
  switch(_role) {

  case ROLE_STATE: {
    if (_output.spendingTransactionHash != CryptoNote::NULL_HASH)
      return static_cast<quint8>(OutputState::SPENT);
    else
      return static_cast<quint8>(OutputState::UNSPENT);
  }

  case ROLE_TYPE:
    return static_cast<quint8>(_output.type);

  case ROLE_AMOUNT:
    return static_cast<quint64>(_output.amount);

  case ROLE_GLOBAL_OUTPUT_INDEX:
    return static_cast<quint32>(_output.globalOutputIndex);

  case ROLE_OUTPUT_IN_TRANSACTION:
    return static_cast<quint32>(_output.outputInTransaction);

  case ROLE_TX_HASH:
    return QByteArray(reinterpret_cast<char*>(&_output.transactionHash), 32);

  case ROLE_TX_PUBLIC_KEY:
    return QByteArray(reinterpret_cast<char*>(&_output.transactionPublicKey), 32);

  case ROLE_OUTPUT_KEY:
    return QByteArray(reinterpret_cast<char*>(&_output.outputKey), 32);

  case ROLE_SPENDING_BLOCK_HEIGHT:
    return _output.spendingBlockHeight;

  case ROLE_TIMESTAMP:
    return (_output.timestamp > 0 ? QDateTime::fromTime_t(_output.timestamp) : QDateTime());

  case ROLE_SPENDING_TRANSACTION_HASH:
    return QByteArray(reinterpret_cast<char*>(&_output.spendingTransactionHash), 32);

  case ROLE_KEY_IMAGE:
    return QByteArray(reinterpret_cast<char*>(&_output.keyImage), 32);

  case ROLE_INPUT_IN_TRANSACTION:
    return static_cast<quint32>(_output.inputInTransaction);

  case ROLE_ROW:
    return _index.row();
  }

  return QVariant();
}

void OutputsModel::reloadWalletTransactions() {
  reset();
  std::vector<CryptoNote::TransactionOutputInformation> unspent = WalletAdapter::instance().getOutputs();
  std::vector<CryptoNote::TransactionSpentOutputInformation> spent = WalletAdapter::instance().getSpentOutputs();
  m_utputs = QVector<CryptoNote::TransactionSpentOutputInformation>::fromStdVector(spent);
  spent.clear();
  spent.shrink_to_fit();

  quint32 outputsCount = unspent.size() + m_utputs.size();

  for (const auto& o : unspent) {
    //CryptoNote::TransactionSpentOutputInformation s = *static_cast<const CryptoNote::TransactionSpentOutputInformation *>(&o); // crashes here
    CryptoNote::TransactionSpentOutputInformation s;
    s.type = o.type;
    s.amount = o.amount;
    s.globalOutputIndex = o.globalOutputIndex;
    s.outputInTransaction = o.outputInTransaction;
    s.transactionHash = o.transactionHash;
    s.transactionPublicKey = o.transactionPublicKey;
    s.outputKey = o.outputKey;
    s.requiredSignatures = o.requiredSignatures;

    s.spendingBlockHeight = std::numeric_limits<uint32_t>::max();
    s.spendingTransactionHash = CryptoNote::NULL_HASH;
    s.timestamp = 0;
    s.keyImage = {};
    s.inputInTransaction = std::numeric_limits<uint32_t>::max();

    m_utputs.append(s);
  }
  unspent.clear();

  // need to sort them
  qSort(m_utputs.begin(), m_utputs.end(), transactionSpentOutputInformationLessThan);

  beginInsertRows(QModelIndex(), 0, outputsCount - 1);
  endInsertRows();
}

void OutputsModel::appendTransaction(CryptoNote::TransactionId _id) {
  Q_UNUSED(_id);
  reloadWalletTransactions();
}

void OutputsModel::reset() {
  beginResetModel();
  m_utputs.clear();
  endResetModel();
}

}
