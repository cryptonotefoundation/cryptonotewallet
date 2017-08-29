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

#include <QEventLoop>
#include <QThread>
#include <QTimerEvent>
#include <QUrl>

#include "CryptoNoteAdapter.h"
#include "WalletLogger/WalletLogger.h"
#include "IBlockChainExplorerAdapter.h"
#include "InProcessNodeAdapter.h"
#include "ProxyRpcNodeAdapter.h"

#include "CryptoNoteCore/TransactionExtra.h"

namespace WalletGui {

namespace {

const int AUTO_CONNECTION_INTERVAL = 1000;
const char OLD_CORE_LOG_FILE_NAME[] = "intensecoinwallet.log";

}

CryptoNoteAdapter::CryptoNoteAdapter(const QDir& _dataDir, bool _testnet, bool _debug, QObject* _parent) : QObject(_parent),
  m_dataDir(_dataDir), m_testnet(_testnet), m_debug(_debug), m_connectionMethod(ConnectionMethod::AUTO),
  m_localDaemodPort(CryptoNote::RPC_DEFAULT_PORT), m_remoteDaemonUrl(), m_coreLogger(), m_walletLogger(),
  m_currency(CryptoNote::CurrencyBuilder(m_coreLogger).currency()),
  m_nodeAdapter(nullptr), m_autoConnectionTimerId(-1) {
}

CryptoNoteAdapter::~CryptoNoteAdapter() {
}

int CryptoNoteAdapter::init(ConnectionMethod _connectionMethod, quint16 _localDaemonPort,
  const QUrl& _remoteDaemonUrl) {
  Q_ASSERT(m_nodeAdapter == nullptr);
  m_connectionMethod = _connectionMethod;
  m_localDaemodPort = _localDaemonPort;
  m_remoteDaemonUrl = _remoteDaemonUrl;
  QEventLoop initLoop;
  bool initCompleted = false;
  QMetaObject::Connection connection = connect(this, &CryptoNoteAdapter::initCompletedSignal, [&initLoop, &initCompleted](int _initStatus) {
    initCompleted = true;
    initLoop.exit(_initStatus);
  });

  initCoreLogger(m_debug);
  initWalletLogger(m_debug);
  initNode();
  int initStatus = initLoop.exec();
  if (!initCompleted) {
    initStatus = -1;
  }

  disconnect(connection);

  WalletLogger::debug(tr("[CryptoNote wrapper] Init status: %1").arg(initStatus));
  return initStatus;
}

void CryptoNoteAdapter::deinit() {
  WalletLogger::debug(tr("[CryptoNote wrapper] Deinitializing..."));
  Q_ASSERT(m_nodeAdapter != nullptr);
  if (m_nodeAdapter->getBlockChainExplorerAdapter() != nullptr) {
    m_nodeAdapter->getBlockChainExplorerAdapter()->deinit();
  }

  m_nodeAdapter->deinit();
  dynamic_cast<QObject*>(m_nodeAdapter)->deleteLater();
  m_nodeAdapter = nullptr;
  Q_EMIT deinitCompletedSignal();
}

INodeAdapter* CryptoNoteAdapter::getNodeAdapter() const {
  Q_ASSERT(m_nodeAdapter != nullptr);
  return m_nodeAdapter;
}

bool CryptoNoteAdapter::isValidAddress(const QString& _address) const {
  CryptoNote::AccountPublicAddress internalAddress;
  return m_currency.parseAccountAddressString(_address.toStdString(), internalAddress);
}

bool CryptoNoteAdapter::isValidPaymentId(const QString& _paymentId) const {
  if (_paymentId.length() != 2 * sizeof(Crypto::Hash)) {
    return false;
  }

  QByteArray byteArray = QByteArray::fromHex(_paymentId.toUtf8());
  return byteArray.size() == sizeof(Crypto::Hash);
}

QString CryptoNoteAdapter::getCurrencyTicker() const {
  return "ITNS";
}

quint64 CryptoNoteAdapter::getMinimalFee() const {
  return m_currency.minimumFee();
}

quint64 CryptoNoteAdapter::getTargetTime() const {
  return m_currency.difficultyTarget();
}

QString CryptoNoteAdapter::formatAmount(qint64 _amount) const {
  QString result = QString::number(qAbs(_amount));
  quintptr numberOfDecimalPlaces = m_currency.numberOfDecimalPlaces();
  if (result.length() < numberOfDecimalPlaces + 1) {
    result = result.rightJustified(numberOfDecimalPlaces + 1, '0');
  }

  quint32 dotPos = result.length() - numberOfDecimalPlaces;
  for (quint32 pos = result.length() - 1; pos > dotPos + 1; --pos) {
    if (result[pos] == '0') {
      result.remove(pos, 1);
    } else {
      break;
    }
  }

  result.insert(dotPos, ".");
  for (qint32 pos = dotPos - 3; pos > 0; pos -= 3) {
    if (result[pos - 1].isDigit()) {
      result.insert(pos, ',');
    }
  }

  return _amount < 0 ? result.prepend("-") : result;
}

QString CryptoNoteAdapter::formatUnsignedAmount(quint64 _amount) const {
  QString result = QString::number(_amount);
  quintptr numberOfDecimalPlaces = m_currency.numberOfDecimalPlaces();
  if (result.length() < numberOfDecimalPlaces + 1) {
    result = result.rightJustified(numberOfDecimalPlaces + 1, '0');
  }

  quint32 dotPos = result.length() - numberOfDecimalPlaces;
  for (quint32 pos = result.length() - 1; pos > dotPos + 1; --pos) {
    if (result[pos] == '0') {
      result.remove(pos, 1);
    } else {
      break;
    }
  }

  result.insert(dotPos, ".");
  for (qint32 pos = dotPos - 3; pos > 0; pos -= 3) {
    if (result[pos - 1].isDigit()) {
      result.insert(pos, ',');
    }
  }

  return result;
}

QString CryptoNoteAdapter::formatAmountToShort(qint64 _amount) const {
  QString amountString = formatAmount(_amount);
  int dotPos = amountString.indexOf('.');
  if (amountString.length() - dotPos <= 5) {
    return amountString;
  }

  QString result = amountString.left(dotPos + 1);
  for (int i = dotPos + 1; i < dotPos + 5; ++i) {
    result.append(amountString[i]);
  }

  return result;
}

qint64 CryptoNoteAdapter::parseAmount(const QString& _amountString) const {
  QString amountString = _amountString.trimmed().remove(',');
  bool negative = amountString.startsWith('-');
  if (negative) {
    amountString.remove(0, 1);
  }

  uint64_t amount;
  if (!m_currency.parseAmount(amountString.toStdString(), amount)) {
    amount = 0;
  }

  return negative ? -static_cast<qint64>(amount) : static_cast<qint64>(amount);
}

quint64 CryptoNoteAdapter::parseUnsignedAmount(const QString& _amountString) const {
  QString amountString = _amountString.trimmed().remove(',');
  uint64_t amount;
  if (!m_currency.parseAmount(amountString.toStdString(), amount)) {
    amount = 0;
  }

  return amount;
}

std::string CryptoNoteAdapter::convertPaymentToExtra(const QString& _paymentIdString) const {
  if (_paymentIdString.isEmpty()) {
    return "";
  }

  std::string paymentIdString = _paymentIdString.toStdString();

  Crypto::Hash paymentId;
  if (!CryptoNote::parsePaymentId(paymentIdString, paymentId)) {
    return "";
  }

  std::vector<uint8_t> extra;
  CryptoNote::BinaryArray extraNonce;
  CryptoNote::setPaymentIdToTransactionExtraNonce(extraNonce, paymentId);
  if (!CryptoNote::addExtraNonceToTransactionExtra(extra, extraNonce)) {
    return "";
  }

  return std::string(extra.begin(), extra.end());
}

QString CryptoNoteAdapter::extractPaymentIdFromExtra(const std::string& _extra) const {
  std::vector<CryptoNote::TransactionExtraField> extraFields;
  std::vector<uint8_t> extraVector;
  std::copy(_extra.begin(), _extra.end(), std::back_inserter(extraVector));

  if (!CryptoNote::parseTransactionExtra(extraVector, extraFields)) {
    return QString();
  }

  CryptoNote::TransactionExtraNonce extraNonce;
  if (CryptoNote::findTransactionExtraFieldByType(extraFields, extraNonce)) {
    Crypto::Hash paymentIdHash;
    if (CryptoNote::getPaymentIdFromTransactionExtraNonce(extraNonce.nonce, paymentIdHash)) {
      return QByteArray(reinterpret_cast<char*>(&paymentIdHash), sizeof(paymentIdHash)).toHex();
    }
  }

  return QByteArray();
}

void CryptoNoteAdapter::addObserver(ICryptoNoteAdapterObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  connect(this, SIGNAL(initCompletedSignal(int)), observer, SLOT(cryptoNoteAdapterInitCompleted(int)));
  connect(this, SIGNAL(deinitCompletedSignal()), observer, SLOT(cryptoNoteAdapterDeinitCompleted()));
}

void CryptoNoteAdapter::removeObserver(ICryptoNoteAdapterObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  disconnect(this, SIGNAL(initCompletedSignal(int)), observer, SLOT(cryptoNoteAdapterInitCompleted(int)));
  disconnect(this, SIGNAL(deinitCompletedSignal()), observer, SLOT(cryptoNoteAdapterDeinitCompleted()));
}

void CryptoNoteAdapter::initCompleted(int _status) {
  if (_status == INodeAdapter::INIT_SUCCESS && m_nodeAdapter->getBlockChainExplorerAdapter() != nullptr) {
    m_nodeAdapter->getBlockChainExplorerAdapter()->init();
  }

  if (m_nodeAdapter->getNodeType() == NodeType::IN_PROCESS || m_autoConnectionTimerId == -1) {
    m_nodeAdapter->removeObserver(this);
    Q_EMIT initCompletedSignal(_status);
  }
}

void CryptoNoteAdapter::deinitCompleted() {
  // Do nothing
}

void CryptoNoteAdapter::peerCountUpdated(quintptr _count) {
  if (m_nodeAdapter->getNodeType() == NodeType::RPC && m_autoConnectionTimerId != -1) {
    killTimer(m_autoConnectionTimerId);
    m_autoConnectionTimerId = -1;
    m_nodeAdapter->removeObserver(this);
    Q_EMIT initCompletedSignal(INodeAdapter::INIT_SUCCESS);
  }
}

void CryptoNoteAdapter::localBlockchainUpdated(CryptoNote::BlockHeaderInfo _lastLocalBlockInfo) {
  if (m_nodeAdapter->getNodeType() == NodeType::RPC && m_autoConnectionTimerId != -1) {
    killTimer(m_autoConnectionTimerId);
    m_autoConnectionTimerId = -1;
    m_nodeAdapter->removeObserver(this);
    Q_EMIT initCompletedSignal(INodeAdapter::INIT_SUCCESS);
  }
}

void CryptoNoteAdapter::lastKnownBlockHeightUpdated(quint32 _height) {
  if (m_nodeAdapter->getNodeType() == NodeType::RPC && m_autoConnectionTimerId != -1) {
    killTimer(m_autoConnectionTimerId);
    m_autoConnectionTimerId = -1;
    m_nodeAdapter->removeObserver(this);
    Q_EMIT initCompletedSignal(INodeAdapter::INIT_SUCCESS);
  }
}

void CryptoNoteAdapter::connectionStatusUpdated(bool _connected) {
  if (m_nodeAdapter->getNodeType() == NodeType::RPC && m_autoConnectionTimerId != -1 && !_connected) {
    onLocalDaemonNotFound();
  }
}

void CryptoNoteAdapter::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_autoConnectionTimerId) {
    onLocalDaemonNotFound();
    return;
  }

  QObject::timerEvent(_event);
}

void CryptoNoteAdapter::initCoreLogger(bool _debug) {
  if (QFile::exists(m_dataDir.absoluteFilePath(OLD_CORE_LOG_FILE_NAME))) {
    QFile::rename(m_dataDir.absoluteFilePath(OLD_CORE_LOG_FILE_NAME), m_dataDir.absoluteFilePath(CORE_LOG_FILE_NAME));
  }

  configureLogger(m_coreLogger, m_dataDir.absoluteFilePath(CORE_LOG_FILE_NAME), _debug);
}

void CryptoNoteAdapter::initWalletLogger(bool _debug) {
  configureLogger(m_walletLogger, m_dataDir.absoluteFilePath(WALLET_LOG_FILE_NAME), _debug);
}

void CryptoNoteAdapter::initNode() {
  Q_ASSERT(m_nodeAdapter == nullptr);
  switch(m_connectionMethod) {
  case ConnectionMethod::AUTO:
    initAutoConnection();
    break;
  case ConnectionMethod::EMBEDDED:
    initInProcessNode();
    break;
  case ConnectionMethod::LOCAL:
    initLocalRpcNode();
    break;
  case ConnectionMethod::REMOTE:
    initRemoteRpcNode();
    break;
  }
}

void CryptoNoteAdapter::initAutoConnection() {
  WalletLogger::debug(tr("[CryptoNote wrapper] Searching local daemon: 127.0.0.1:%1").arg(CryptoNote::RPC_DEFAULT_PORT));
  m_nodeAdapter = new ProxyRpcNodeAdapter(m_currency, m_coreLogger, m_walletLogger, "127.0.0.1", CryptoNote::RPC_DEFAULT_PORT, this);
  m_nodeAdapter->addObserver(this);
  m_autoConnectionTimerId = startTimer(AUTO_CONNECTION_INTERVAL);
  m_nodeAdapter->init();
}

void CryptoNoteAdapter::initInProcessNode() {
  m_nodeAdapter = new InProcessNodeAdapter(m_currency, m_coreLogger, m_walletLogger, this);
  m_nodeAdapter->addObserver(this);
  m_nodeAdapter->init();
}

void CryptoNoteAdapter::initLocalRpcNode() {
  m_nodeAdapter = new ProxyRpcNodeAdapter(m_currency, m_coreLogger, m_walletLogger, "127.0.0.1", m_localDaemodPort, this);
  m_nodeAdapter->addObserver(this);
  m_nodeAdapter->init();
}

void CryptoNoteAdapter::initRemoteRpcNode() {
  m_nodeAdapter = new ProxyRpcNodeAdapter(m_currency, m_coreLogger, m_walletLogger, m_remoteDaemonUrl.host(), m_remoteDaemonUrl.port(), this);
  m_nodeAdapter->addObserver(this);
  m_nodeAdapter->init();
}

void CryptoNoteAdapter::onLocalDaemonNotFound() {
  WalletLogger::debug(tr("[CryptoNote wrapper] Daemon on 127.0.0.1:%1 not found").arg(CryptoNote::RPC_DEFAULT_PORT));
  killTimer(m_autoConnectionTimerId);
  m_autoConnectionTimerId = -1;
  QObject* nodeAdapter = dynamic_cast<QObject*>(m_nodeAdapter);
  m_nodeAdapter->deinit();
  nodeAdapter->deleteLater();
  m_nodeAdapter = nullptr;
  initInProcessNode();
}

void CryptoNoteAdapter::configureLogger(Logging::LoggerManager& _logger, const QString& _logFilePath, bool _debug) {
  Logging::Level level = _debug ? Logging::DEBUGGING : Logging::INFO;
  Common::JsonValue loggerConfiguration(Common::JsonValue::OBJECT);
  loggerConfiguration.insert("globalLevel", static_cast<int64_t>(level));
  Common::JsonValue& cfgLoggers = loggerConfiguration.insert("loggers", Common::JsonValue::ARRAY);
  Common::JsonValue& fileLogger = cfgLoggers.pushBack(Common::JsonValue::OBJECT);
  fileLogger.insert("type", "file");
  fileLogger.insert("filename", _logFilePath.toStdString());
  fileLogger.insert("level", static_cast<int64_t>(level));
  _logger.configure(loggerConfiguration);
}

}
