// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2022 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QCoreApplication>
#include <QFile>
#include <QTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSettings>
#include <QStandardPaths>
#include <QTextCodec>
#include <QUrl>
#include <Common/Util.h>
#include "CommandLineParser.h"
#include "CurrencyAdapter.h"
#include "CryptoNoteWalletConfig.h"
#include "Settings.h"

namespace WalletGui {

Q_DECL_CONSTEXPR char OPTION_WALLET_FILE[] = "walletFile";
Q_DECL_CONSTEXPR char OPTION_ENCRYPTED[] = "encrypted";
Q_DECL_CONSTEXPR char OPTION_LANGUAGE[] = "Language";
Q_DECL_CONSTEXPR char OPTION_CONNECTION[] = "connectionMode";
Q_DECL_CONSTEXPR char OPTION_CONNECTIONS[] = "connectionsCount";
Q_DECL_CONSTEXPR char OPTION_RPCNODES[] = "remoteNodes";
Q_DECL_CONSTEXPR char OPTION_DAEMON_PORT[] = "daemonPort";
Q_DECL_CONSTEXPR char OPTION_REMOTE_NODE[] = "remoteNode";
const char OPTION_WALLET_THEME[] = "theme";

const char OPTION_WALLET_OPTIMIZATION[] = "optimization";
const char OPTION_WALLET_OPTIMIZATION_ENABLED[] = "enabled";
const char OPTION_WALLET_OPTIMIZATION_TIME_SET_MANUALLY[] = "useCustomTime";
const char OPTION_WALLET_OPTIMIZATION_START_TIME[] = "startTime";
const char OPTION_WALLET_OPTIMIZATION_STOP_TIME[] = "stopTime";
const char OPTION_WALLET_OPTIMIZATION_INTERVAL[] = "interval";
const char OPTION_WALLET_OPTIMIZATION_THRESHOLD[] = "target";
const char OPTION_WALLET_OPTIMIZATION_MIXIN[] = "mixin";
const quint64 DEFAULT_OPTIMIZATION_PERIOD = 1000 * 60 * 30; // 30 minutes
const quint64 DEFAULT_OPTIMIZATION_THRESHOLD = 10000000000000;
const quint64 DEFAULT_OPTIMIZATION_MIXIN = 3;
const char OPTION_SKIP_WALLET_OPTIMIZATION_TRANSACTIONS[] = "skipFusionTransactions";
const char LOCALHOST[] = "127.0.0.1";
const char OPTION_WALLET_RPC[] = "WalletRpc";
const char OPTION_WALLET_RPC_ENABLED[] = "enabled";
const char OPTION_WALLET_RPC_BIND_IP[] = "walletRpcBindIp";
const char OPTION_WALLET_RPC_BIND_PORT[] = "walletRpcBindPort";
const char OPTION_WALLET_RPC_USER[] = "walletRpcUser";
const char OPTION_WALLET_RPC_PASSWORD[] = "walletRpcPassword";

const QVector<NodeSetting> DEFAULT_NODES_LIST = {
  {"node.karbo.io", 32348, "/", false},
  {"node.karbo.org", 32348, "/", false},
  {"node.karbowanec.com", 32348, "/", false},
  {"free.rublin.org", 32348, "/", false}
};

Settings& Settings::instance() {
  static Settings inst;
  return inst;
}

Settings::Settings() : QObject(), m_cmdLineParser(nullptr) {
}

Settings::~Settings() {
}

void Settings::setCommandLineParser(CommandLineParser* _cmdLineParser) {
  Q_CHECK_PTR(_cmdLineParser);
  m_cmdLineParser = _cmdLineParser;
}

void Settings::load() {
  QFile cfgFile(getDataDir().absoluteFilePath(QCoreApplication::applicationName() + ".cfg"));
  if (cfgFile.open(QIODevice::ReadOnly)) {
    m_settings = QJsonDocument::fromJson(cfgFile.readAll()).object();
    cfgFile.close();
    if (!m_settings.contains("walletFile")) {
      m_addressBookFile = getDataDir().absoluteFilePath(QCoreApplication::applicationName() + ".addressbook");
    } else {
      m_addressBookFile = m_settings.value("walletFile").toString();
      m_addressBookFile.replace(m_addressBookFile.lastIndexOf(".wallet"), 7, ".addressbook");
    }

  } else {
    m_addressBookFile = getDataDir().absoluteFilePath(QCoreApplication::applicationName() + ".addressbook");
  }

  QVector<NodeSetting> nodesList = getRpcNodesList();

  if (!m_settings.contains(OPTION_RPCNODES) || nodesList.isEmpty()) {
    setRpcNodesList(DEFAULT_NODES_LIST);
  } else {
    setRpcNodesList(nodesList);
  }

  if (!m_settings.contains("recentWallets")) {
     QStringList recentWallets;
     if (m_settings.contains("walletFile")) {
        recentWallets.prepend(m_settings.value("walletFile").toString());
     } else {
        recentWallets.prepend(getDataDir().absoluteFilePath(QCoreApplication::applicationName() + ".wallet"));
     }
     m_settings.insert("recentWallets", QJsonArray::fromStringList(recentWallets));
  }
}

bool Settings::isTestnet() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->hasTestnetOption();
}

bool Settings::withoutCheckpoints() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->hasWithoutCheckpointsOption();
}

bool Settings::alowReorg() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->hasAllowReorgOption();
}

bool Settings::hasAllowLocalIpOption() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->hasAllowLocalIpOption();
}

bool Settings::hasHideMyPortOption() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->hasHideMyPortOption();
}

QString Settings::getP2pBindIp() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getP2pBindIp();
}

quint16 Settings::getP2pBindPort() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getP2pBindPort();
}

QString Settings::getRpcBindIp() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getRpcBindIp();
}

quint16 Settings::getRpcBindPort() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getRpcBindPort();
}

bool Settings::hasRunRpc() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->hasRpcOption();
}

bool Settings::hasRestrictedRpc() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->hasRestrictedRpcOption();
}

quint16 Settings::getP2pExternalPort() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getP2pExternalPort();
}

quint16 Settings::getConnectionsCount() const {
  if (m_settings.contains(OPTION_CONNECTIONS)) {
    return m_settings.value(OPTION_CONNECTIONS).toVariant().toInt();
  }

  return CryptoNote::P2P_DEFAULT_CONNECTIONS_COUNT;
}

QStringList Settings::getPeers() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getPeers();
}

QStringList Settings::getPriorityNodes() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getPiorityNodes();
}

QStringList Settings::getExclusiveNodes() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getExclusiveNodes();
}

QStringList Settings::getSeedNodes() const {
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getSeedNodes();
}

QDir Settings::getDataDir() const {
  Q_CHECK_PTR(m_cmdLineParser);
  return QDir(m_cmdLineParser->getDataDir());
}

quint32 Settings::getRollBack() const {
  Q_CHECK_PTR(m_cmdLineParser);
  return m_cmdLineParser->rollBack();
}

QString Settings::getWalletFile() const {
  return m_settings.contains("walletFile") ? m_settings.value("walletFile").toString() :
    ""; //getDataDir().absoluteFilePath(QCoreApplication::applicationName() + ".wallet");
}

QString Settings::getWalletName() const {
  QFileInfo fi(getWalletFile());
  return fi.fileName();
}

QStringList Settings::getRecentWalletsList() const {
   QStringList recent_wallets;
   if (m_settings.contains("recentWallets")) {
     recent_wallets << m_settings.value("recentWallets").toVariant().toStringList();
   }
   return recent_wallets;
}

QString Settings::getAddressBookFile() const {
  return m_addressBookFile;
}

bool Settings::isEncrypted() const {
  return m_settings.contains("encrypted") ? m_settings.value("encrypted").toBool() : false;
}

bool Settings::isTrackingMode() const {
  return m_settings.contains("tracking") ? m_settings.value("tracking").toBool() : false;
}

QString Settings::getVersion() const {
  return WALLET_VERSION;
}

QString Settings::getCurrentTheme() const {
  return m_settings.contains(OPTION_WALLET_THEME) ? m_settings.value(OPTION_WALLET_THEME).toString() : "light";
}

QString Settings::getLanguage() const {
  QString currentLang; // default should be empty, in order system's will be used
  if (m_settings.contains(OPTION_LANGUAGE)) {
    currentLang = m_settings.value(OPTION_LANGUAGE).toString();
  }

  return currentLang;
}

QString Settings::getConnection() const {
  QString connection = "auto"; // default
  if (m_settings.contains(OPTION_CONNECTION)) {
    connection = m_settings.value(OPTION_CONNECTION).toString();
  }

  return connection;
}

QVector<NodeSetting> Settings::getRpcNodesList() const {
  QVector<NodeSetting> res;
  if (m_settings.contains(OPTION_RPCNODES)) {
    const QJsonArray nodeSettingArray(m_settings.value(OPTION_RPCNODES).toArray());
    for (const QJsonValue nodeSettingValue : nodeSettingArray) {
      const QJsonObject nodeSettingObj = nodeSettingValue.toObject();
      NodeSetting nodeSetting;
      if (nodeSettingObj.contains("host") &&
          nodeSettingObj.contains("port") &&
          nodeSettingObj.contains("path") &&
          nodeSettingObj.contains("ssl")) {
          nodeSetting.host = nodeSettingObj.value("host").toString();
          nodeSetting.port = nodeSettingObj.value("port").toInt();
          nodeSetting.path = nodeSettingObj.value("path").toString();
          nodeSetting.ssl  = nodeSettingObj.value("ssl").toBool();
      } else {
         // convert old format
         QUrl remoteNodeUrl = QUrl::fromUserInput(nodeSettingValue.toString());
         nodeSetting.host = remoteNodeUrl.host();
         nodeSetting.port = remoteNodeUrl.port();
         nodeSetting.path = "/";
         nodeSetting.ssl  = false;
      }
      res.append(nodeSetting);
    }
  }

  return res;
}

quint16 Settings::getCurrentLocalDaemonPort() const {
  if (m_settings.contains(OPTION_DAEMON_PORT)) {
    return m_settings.value(OPTION_DAEMON_PORT).toVariant().toInt();
  }

  return CryptoNote::RPC_DEFAULT_PORT;
}

NodeSetting Settings::getCurrentRemoteNode() const {
  NodeSetting remotenode;
  if (m_settings.contains(OPTION_REMOTE_NODE)) {
    const QJsonObject nodeSettingObj = m_settings.value(OPTION_REMOTE_NODE).toObject();
    remotenode.host = nodeSettingObj.value("host").toString();
    remotenode.port = nodeSettingObj.value("port").toInt();
    remotenode.path = nodeSettingObj.value("path").toString();
    remotenode.ssl = nodeSettingObj.value("ssl").toBool();
  }
  return remotenode;
}

quint16 Settings::getMiningThreads() const {
  if (m_settings.contains("miningThreads")) {
    return m_settings.value("miningThreads").toVariant().toInt();
  } else {
    return 0;
  }
}

bool Settings::isMiningOnLaunchEnabled() const {
  return m_settings.contains("autostartMininig") ? m_settings.value("autostartMininig").toBool() : false;
}

bool Settings::isStartOnLoginEnabled() const {
  bool res = false;
#ifdef Q_OS_MAC
  QDir autorunDir = QDir::home();
  if (!autorunDir.cd("Library") || !autorunDir.cd("LaunchAgents")) {
    return false;
  }

  QString autorunFilePath = autorunDir.absoluteFilePath(QCoreApplication::applicationName() + ".plist");
  if (!QFile::exists(autorunFilePath)) {
    return false;
  }

  QSettings autorunSettings(autorunFilePath, QSettings::NativeFormat);
  res = autorunSettings.value("RunAtLoad", false).toBool();
#elif defined(Q_OS_LINUX)
  QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
  if (configPath.isEmpty()) {
    return false;
  }

  QDir autorunDir(configPath);
  if (!autorunDir.cd("autostart")) {
    return false;
  }

  QString autorunFilePath = autorunDir.absoluteFilePath(QCoreApplication::applicationName() + ".desktop");
  res = QFile::exists(autorunFilePath);
#elif defined(Q_OS_WIN)
  QSettings autorunSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
  QString keyName = QString("%1Wallet").arg(CurrencyAdapter::instance().getCurrencyDisplayName());
  res = autorunSettings.contains(keyName) &&
    !QDir::fromNativeSeparators(autorunSettings.value(keyName).toString()).compare(QCoreApplication::applicationFilePath());
#endif
  return res;
}

#ifdef Q_OS_WIN
bool Settings::isMinimizeToTrayEnabled() const {
  return m_settings.contains("minimizeToTray") ? m_settings.value("minimizeToTray").toBool() : false;
}

bool Settings::isCloseToTrayEnabled() const {
  return m_settings.contains("closeToTray") ? m_settings.value("closeToTray").toBool() : false;
}
#endif

bool Settings::isOptimizationEnabled() const {
  if (!m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
    return false;
  }

  QJsonObject optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
  return optimizationObject.contains(OPTION_WALLET_OPTIMIZATION_ENABLED) ? optimizationObject.value(OPTION_WALLET_OPTIMIZATION_ENABLED).toBool() : false;
}

bool Settings::isOptimizationTimeSetManually() const {
  if (!m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
    return false;
  }

  QJsonObject optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
  if (!optimizationObject.contains(OPTION_WALLET_OPTIMIZATION_TIME_SET_MANUALLY)) {
    return false;
  }

  return optimizationObject.value(OPTION_WALLET_OPTIMIZATION_TIME_SET_MANUALLY).toBool();
}

QTime Settings::getOptimizationStartTime() const {
  if (!m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
    return QTime(0, 0);
  }

  QJsonObject optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
  if (!optimizationObject.contains(OPTION_WALLET_OPTIMIZATION_START_TIME)) {
    return QTime(0, 0);
  }

  QString timeString = optimizationObject.value(OPTION_WALLET_OPTIMIZATION_START_TIME).toString();
  return QTime::fromString(timeString, Qt::ISODate);
}

QTime Settings::getOptimizationStopTime() const {
  if (!m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
    return QTime(0, 0);
  }

  QJsonObject optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
  if (!optimizationObject.contains(OPTION_WALLET_OPTIMIZATION_STOP_TIME)) {
    return QTime(0, 0);
  }

  QString timeString = optimizationObject.value(OPTION_WALLET_OPTIMIZATION_STOP_TIME).toString();
  return QTime::fromString(timeString, Qt::ISODate);
}

quint64 Settings::getOptimizationInterval() const {
  if (!m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
    return DEFAULT_OPTIMIZATION_PERIOD;
  }

  QJsonObject optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
  if (!optimizationObject.contains(OPTION_WALLET_OPTIMIZATION_INTERVAL)) {
    return DEFAULT_OPTIMIZATION_PERIOD;
  }

  return optimizationObject.value(OPTION_WALLET_OPTIMIZATION_INTERVAL).toString().toULongLong();
}

quint64 Settings::getOptimizationThreshold() const {
  if (!m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
    return DEFAULT_OPTIMIZATION_THRESHOLD;
  }

  QJsonObject optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
  if (!optimizationObject.contains(OPTION_WALLET_OPTIMIZATION_THRESHOLD)) {
    return DEFAULT_OPTIMIZATION_THRESHOLD;
  }

  return optimizationObject.value(OPTION_WALLET_OPTIMIZATION_THRESHOLD).toString().toULongLong();
}

quint64 Settings::getOptimizationMixin() const {
  if (!m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
    return DEFAULT_OPTIMIZATION_MIXIN;
  }

  QJsonObject optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
  if (!optimizationObject.contains(OPTION_WALLET_OPTIMIZATION_MIXIN)) {
    return DEFAULT_OPTIMIZATION_MIXIN;
  }

  return optimizationObject.value(OPTION_WALLET_OPTIMIZATION_MIXIN).toString().toULongLong();
}

bool Settings::skipFusionTransactions() const {
  if (!m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
    return false;
  }

  QJsonObject optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
  return optimizationObject.contains(OPTION_SKIP_WALLET_OPTIMIZATION_TRANSACTIONS) ? optimizationObject.value(OPTION_SKIP_WALLET_OPTIMIZATION_TRANSACTIONS).toBool() : false;
}

bool Settings::hideEverythingOnLocked() const {
  return m_settings.contains("hideEverythingOnLocked") ? m_settings.value("hideEverythingOnLocked").toBool() : false;
}

bool Settings::runWalletRpc() const {
  if (!m_settings.contains(OPTION_WALLET_RPC)) {
    return false;
  }

  QJsonObject walletRpcObject = m_settings.value(OPTION_WALLET_RPC).toObject();
  return walletRpcObject.contains(OPTION_WALLET_RPC_ENABLED) ? walletRpcObject.value(OPTION_WALLET_RPC_ENABLED).toBool() : false;
}

QString Settings::getWalletRpcBindIp() const {
  if (m_settings.contains(OPTION_WALLET_RPC)) {
     QJsonObject walletRpcObject = m_settings.value(OPTION_WALLET_RPC).toObject();
     if (walletRpcObject.contains(OPTION_WALLET_RPC_BIND_IP)) {
       return walletRpcObject.value(OPTION_WALLET_RPC_BIND_IP).toString();
     }
  }

  return LOCALHOST;
}

quint16 Settings::getWalletRpcBindPort() const {
  if (m_settings.contains(OPTION_WALLET_RPC)) {
    QJsonObject walletRpcObject = m_settings.value(OPTION_WALLET_RPC).toObject();
    if (walletRpcObject.contains(OPTION_WALLET_RPC_BIND_PORT)) {
      return walletRpcObject.value(OPTION_WALLET_RPC_BIND_PORT).toVariant().toInt();
    }
  }

  return CryptoNote::WALLET_RPC_DEFAULT_PORT;
}

QString Settings::getWalletRpcUser() const {
  QString walletRpcUser = "";

  if (m_settings.contains(OPTION_WALLET_RPC)) {
    QJsonObject walletRpcObject = m_settings.value(OPTION_WALLET_RPC).toObject();
    if (walletRpcObject.contains(OPTION_WALLET_RPC_USER)) {
       walletRpcUser = walletRpcObject.value(OPTION_WALLET_RPC_USER).toString();
    }
  }

  return walletRpcUser;
}

QString Settings::getWalletRpcPassword() const {
  QString walletRpcPassword = "";

  if (m_settings.contains(OPTION_WALLET_RPC)) {
    QJsonObject walletRpcObject = m_settings.value(OPTION_WALLET_RPC).toObject();
    if (walletRpcObject.contains(OPTION_WALLET_RPC_PASSWORD)) {
       walletRpcPassword = walletRpcObject.value(OPTION_WALLET_RPC_PASSWORD).toString();
    }
  }

  return walletRpcPassword;
}


void Settings::setWalletFile(const QString& _file) {
  if (_file.endsWith(".wallet") || _file.endsWith(".keys")) {
    m_settings.insert("walletFile", _file);
  } else if (_file.endsWith(".trackingwallet")) {
    m_settings.insert("walletFile", _file);
  } else {
    m_settings.insert("walletFile", _file + ".wallet");
  }

  if (!m_settings.contains("recentWallets")) {
    QStringList recentWallets;
    if (_file.endsWith(".wallet") || _file.endsWith(".keys")) {
        recentWallets.prepend(_file);
    } else if (_file.endsWith(".trackingwallet")) {
        recentWallets.prepend(_file);
    } else {
       recentWallets.prepend(_file + ".wallet");
    }
    m_settings.insert("recentWallets", QJsonArray::fromStringList(recentWallets));
  } else {
    QStringList recentWallets = m_settings.value("recentWallets").toVariant().toStringList();

    foreach (const QString &recentFile, recentWallets) {
      if (recentFile.contains(_file))
        recentWallets.removeOne(recentFile);
    }
    if (_file.endsWith(".wallet") || _file.endsWith(".keys")) {
        recentWallets.prepend(_file);
    } else if (_file.endsWith(".trackingwallet")) {
        recentWallets.prepend(_file);
    } else {
       recentWallets.prepend(_file + ".wallet");
    }
    while (recentWallets.size() > 10)
           recentWallets.removeLast();
    m_settings.insert("recentWallets", QJsonArray::fromStringList(recentWallets));
  }

  saveSettings();
  m_addressBookFile = m_settings.value("walletFile").toString();
  m_addressBookFile.replace(m_addressBookFile.lastIndexOf(".wallet"), 7, ".addressbook");
}

void Settings::setEncrypted(bool _encrypted) {
  if (isEncrypted() != _encrypted) {
    m_settings.insert("encrypted", _encrypted);
    saveSettings();
  }
}

void Settings::setTrackingMode(bool _tracking) {
  if (isTrackingMode() != _tracking) {
    m_settings.insert("tracking", _tracking);
    saveSettings();
  }
}

void Settings::setMiningOnLaunchEnabled(bool _automining) {
  if (isMiningOnLaunchEnabled() != _automining) {
    m_settings.insert("autostartMininig", _automining);
    saveSettings();
  }
}

void Settings::setCurrentTheme(const QString& _theme) {
}

void Settings::setLanguage(const QString& _language) {
    m_settings.insert(OPTION_LANGUAGE, _language);
    saveSettings();
}

void Settings::setStartOnLoginEnabled(bool _enable) {
#ifdef Q_OS_MAC
  QDir autorunDir = QDir::home();
  if (!autorunDir.cd("Library") || !autorunDir.cd("LaunchAgents")) {
    return;
  }

  QString autorunFilePath = autorunDir.absoluteFilePath(QCoreApplication::applicationName() + ".plist");
  QSettings autorunSettings(autorunFilePath, QSettings::NativeFormat);
  autorunSettings.setValue("Label", "org." + QCoreApplication::applicationName());
  autorunSettings.setValue("Program", QCoreApplication::applicationFilePath());
  autorunSettings.setValue("RunAtLoad", _enable);
  autorunSettings.setValue("ProcessType", "InterActive");
#elif defined(Q_OS_LINUX)
  QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
  if (configPath.isEmpty()) {
    return;
  }

  QDir autorunDir(configPath);
  if(!autorunDir.exists("autostart")) {
    autorunDir.mkdir("autostart");
  }

  if (!autorunDir.cd("autostart")) {
    return;
  }

  QString autorunFilePath = autorunDir.absoluteFilePath(QCoreApplication::applicationName() + ".desktop");
  QFile autorunFile(autorunFilePath);
  if (!autorunFile.open(QFile::WriteOnly | QFile::Truncate)) {
    return;
  }

  if (_enable) {
    autorunFile.write("[Desktop Entry]\n");
    autorunFile.write("Type=Application\n");
    autorunFile.write(QString("Name=%1 Wallet\n").arg(CurrencyAdapter::instance().getCurrencyDisplayName()).toLocal8Bit());
    autorunFile.write(QString("Exec=%1\n").arg(QCoreApplication::applicationFilePath()).toLocal8Bit());
    autorunFile.write("Terminal=false\n");
    autorunFile.write("Hidden=false\n");
    autorunFile.close();
  } else {
    QFile::remove(autorunFilePath);
  }
#elif defined(Q_OS_WIN)
  QSettings autorunSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
  QString keyName = QString("%1Wallet").arg(CurrencyAdapter::instance().getCurrencyDisplayName());
  if (_enable) {
    autorunSettings.setValue(keyName, QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
  } else {
    autorunSettings.remove(keyName);
  }
#endif
}

void Settings::setConnection(const QString& _connection) {
    m_settings.insert(OPTION_CONNECTION, _connection);
    saveSettings();
}

void Settings::setConnectionsCount(const quint16& _count) {
  m_settings.insert(OPTION_CONNECTIONS, _count);
  saveSettings();
}

void Settings::setCurrentLocalDaemonPort(const quint16& _daemonPort) {
    m_settings.insert(OPTION_DAEMON_PORT, _daemonPort);
    saveSettings();
}

void Settings::setCurrentRemoteNode(const NodeSetting &remoteNode) {
  if (!remoteNode.host.isEmpty()) {
    QJsonObject nodeSettingObj;
    nodeSettingObj.insert("host", QJsonValue(remoteNode.host));
    nodeSettingObj.insert("port", QJsonValue(remoteNode.port));
    nodeSettingObj.insert("path", QJsonValue(remoteNode.path));
    nodeSettingObj.insert("ssl", QJsonValue(remoteNode.ssl));
    m_settings.insert(OPTION_REMOTE_NODE, nodeSettingObj);
  }
  saveSettings();
}

void Settings::setRpcNodesList(const QVector<NodeSetting> &RpcNodesList) {
  if (!RpcNodesList.isEmpty()) {
    QJsonArray nodesList;
    for (const NodeSetting &nodeSetting : RpcNodesList) {
      QJsonObject nodeSettingObj;
      nodeSettingObj.insert("host", QJsonValue(nodeSetting.host));
      nodeSettingObj.insert("port", QJsonValue(nodeSetting.port));
      nodeSettingObj.insert("path", QJsonValue(nodeSetting.path));
      nodeSettingObj.insert("ssl", QJsonValue(nodeSetting.ssl));
      nodesList.append(nodeSettingObj);
    }
    m_settings.insert(OPTION_RPCNODES, nodesList);
  }
  saveSettings();
}

void Settings::setMiningThreads(const quint16& _threads) {
  if (_threads != 0) {
    m_settings.insert("miningThreads", _threads);
  }
  saveSettings();
}

#ifdef Q_OS_WIN
void Settings::setMinimizeToTrayEnabled(bool _enable) {
  if (isMinimizeToTrayEnabled() != _enable) {
    m_settings.insert("minimizeToTray", _enable);
    saveSettings();
  }
}

void Settings::setCloseToTrayEnabled(bool _enable) {
  if (isCloseToTrayEnabled() != _enable) {
    m_settings.insert("closeToTray", _enable);
    saveSettings();
  }
}
#endif

void Settings::setOptimizationEnabled(bool _enable) {
  if (_enable == isOptimizationEnabled()) {
    return;
  }

  {
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_ENABLED, _enable);
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }
}

void Settings::setOptimizationTimeSetManually(bool _enable) {
  if (_enable == isOptimizationTimeSetManually()) {
    return;
  }

  {
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_TIME_SET_MANUALLY, _enable);
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }
}

void Settings::setOptimizationStartTime(const QTime& _startTime) {
  if (_startTime == getOptimizationStartTime()) {
    return;
  }

  {
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_START_TIME, _startTime.toString(Qt::ISODate));
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }
}

void Settings::setOptimizationStopTime(const QTime& _stopTime) {
  if (_stopTime == getOptimizationStopTime()) {
    return;
  }

  {
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_STOP_TIME, _stopTime.toString(Qt::ISODate));
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }
}

void Settings::setOptimizationInterval(quint64 _interval) {
  if (_interval == getOptimizationInterval()) {
    return;
  }

  {
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_INTERVAL, QString::number(_interval));
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }
}

void Settings::setOptimizationThreshold(quint64 _threshold) {
  if (_threshold == getOptimizationThreshold()) {
    return;
  }

  {
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_THRESHOLD, QString::number(_threshold));
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }
}

void Settings::setOptimizationMixin(quint64 _mixin) {
  if (_mixin == getOptimizationMixin()) {
    return;
  }

  {
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_MIXIN, QString::number(_mixin));
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }
}

void Settings::setSkipFusionTransactions(bool _skip) {
  if (_skip == skipFusionTransactions()) {
    return;
  }

  {
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_SKIP_WALLET_OPTIMIZATION_TRANSACTIONS, _skip);
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }
}

void Settings::setRunWalletRpc(bool _enable) {
  if (_enable == runWalletRpc()) {
    return;
  }

  {
    QJsonObject walletRpcObject;
    if (m_settings.contains(OPTION_WALLET_RPC)) {
      walletRpcObject = m_settings.value(OPTION_WALLET_RPC).toObject();
    }

    walletRpcObject.insert(OPTION_WALLET_RPC_ENABLED, _enable);
    m_settings.insert(OPTION_WALLET_RPC, walletRpcObject);
    saveSettings();
  }
}

void Settings::setWalletRpcBindIp(const QString& _ip) {
  if (_ip == getWalletRpcBindIp()) {
    return;
  }

  {
    QJsonObject walletRpcObject;
    if (m_settings.contains(OPTION_WALLET_RPC)) {
      walletRpcObject = m_settings.value(OPTION_WALLET_RPC).toObject();
    }

    walletRpcObject.insert(OPTION_WALLET_RPC_BIND_IP, _ip);
    m_settings.insert(OPTION_WALLET_RPC, walletRpcObject);
    saveSettings();
  }
}

void Settings::setWalletRpcBindPort(const quint16& _port) {
  if (_port == getWalletRpcBindPort()) {
    return;
  }

  {
    QJsonObject walletRpcObject;
    if (m_settings.contains(OPTION_WALLET_RPC)) {
      walletRpcObject = m_settings.value(OPTION_WALLET_RPC).toObject();
    }

    walletRpcObject.insert(OPTION_WALLET_RPC_BIND_PORT, _port);
    m_settings.insert(OPTION_WALLET_RPC, walletRpcObject);
    saveSettings();
  }
}

void Settings::setWalletRpcUser(const QString& _user) {
  if (_user == getWalletRpcUser()) {
    return;
  }

  {
    QJsonObject walletRpcObject;
    if (m_settings.contains(OPTION_WALLET_RPC)) {
      walletRpcObject = m_settings.value(OPTION_WALLET_RPC).toObject();
    }

    walletRpcObject.insert(OPTION_WALLET_RPC_USER, _user);
    m_settings.insert(OPTION_WALLET_RPC, walletRpcObject);
    saveSettings();
  }
}

void Settings::setWalletRpcPassword(const QString& _pwd) {
  if (_pwd == getWalletRpcPassword()) {
    return;
  }

  {
    QJsonObject walletRpcObject;
    if (m_settings.contains(OPTION_WALLET_RPC)) {
      walletRpcObject = m_settings.value(OPTION_WALLET_RPC).toObject();
    }

    walletRpcObject.insert(OPTION_WALLET_RPC_PASSWORD, _pwd);
    m_settings.insert(OPTION_WALLET_RPC, walletRpcObject);
    saveSettings();
  }
}

void Settings::setHideEverythingOnLocked(bool _hide) {
  if (hideEverythingOnLocked() != _hide) {
    m_settings.insert("hideEverythingOnLocked", _hide);
    saveSettings();
  }
}

void Settings::saveSettings() const {
  QFile cfgFile(getDataDir().absoluteFilePath(QCoreApplication::applicationName() + ".cfg"));
  if (cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    QJsonDocument cfg_doc(m_settings);
    cfgFile.write(cfg_doc.toJson());
    cfgFile.close();
  }
}

}
