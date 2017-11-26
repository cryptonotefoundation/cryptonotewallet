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

#include <QCoreApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSettings>
#include <QStandardPaths>
#include <QTextCodec>
#include <QTime>
#include <QUrl>
#include <QUuid>

#include <Common/Util.h>
#include <CryptoNoteConfig.h>

#include "Settings.h"
#include "Application/CommandLineParser.h"
#include "Style/LightStyle.h"
#include "Style/DarkStyle.h"

namespace WalletGui {

namespace {

const char OPTION_MINING_POOLS[] = "miningPools";
const char OPTION_MINING_CPU_CORE_COUNT[] = "miningCpuCoreCount";
const char OPTION_MINING_ON_LOCKED_SCREEN[] = "miningOnLockedScreen";
const char OPTION_MINING_PARAMS[] = "miningParams";
const char OPTION_MINING_POOL_SWITCH_STRATEGY[] = "miningPoolSwitchStrategy";
const char OPTION_NODE_CONNECTION_METHOD[] = "connectionMethod";
const char OPTION_NODE_LOCAL_RPC_PORT[] = "localRpcPort";
const char OPTION_NODE_REMOTE_RPC_URL[] = "remoteRpcUrl";
const char OPTION_WALLET_WALLET_FILE[] = "walletFile";
const char OPTION_WALLET_ENCRYPTED[] = "encrypted";
const char OPTION_WALLET_THEME[] = "theme";
const char OPTION_WALLET_OPTIMIZATION[] = "optimization";
const char OPTION_WALLET_OPTIMIZATION_ENABLED[] = "enabled";
const char OPTION_WALLET_OPTIMIZATION_TIME_SET_MANUALLY[] = "useCustomTime";
const char OPTION_WALLET_OPTIMIZATION_START_TIME[] = "startTime";
const char OPTION_WALLET_OPTIMIZATION_STOP_TIME[] = "stopTime";
const char OPTION_WALLET_OPTIMIZATION_INTERVAL[] = "interval";
const char OPTION_WALLET_OPTIMIZATION_THRESHOLD[] = "target";
const char OPTION_WALLET_OPTIMIZATION_MIXIN[] = "mixin";
const char OPTION_WALLET_OPTIMIZATION_FUSION_TARNSACTIONS_IS_VISIBLE[] = "showOptimizationTransactions";
const char OPTION_RECENT_WALLETS[] = "recentWallets";
const char OPTION_MINIMIZE_TO_TRAY[] = "minimizeToTray";
const char OPTION_CLOSE_TO_TRAY[] = "closeToTray";
const char OPTION_PRIVACY_PARAMS[] = "privacyParams";
const char OPTION_PRIVACY_NEWS_ENABLED[] = "newsEnabled";

const char DEFAULT_WALLET_FILE_NAME[] = "intensecoin.wallet";
const quint64 DEFAULT_OPTIMIZATION_PERIOD = 1000 * 60 * 30; // 30 minutes
const quint64 DEFAULT_OPTIMIZATION_THRESHOLD = 10000000000000;
const quint64 DEFAULT_OPTIMIZATION_MIXIN = 6;

const quint64 VERSION_MAJOR = 1;
const quint64 VERSION_MINOR = 4;
const quint64 VERSION_PATCH = 1;

}

Settings& Settings::instance() {
  static Settings inst;
  return inst;
}


Settings::Settings() : m_p2pBindPort(0), m_cmdLineParser(nullptr) {
  m_defaultPoolList << "45.32.171.89:4444";

  Style* lightStyle = new LightStyle();
  Style* darkStyle = new DarkStyle();
  m_styles.insert(lightStyle->getStyleId(), lightStyle);
  m_styles.insert(darkStyle->getStyleId(), darkStyle);
}

Settings::~Settings() {
  qDeleteAll(m_styles);
}

bool Settings::hasDebugOption() const {
  QReadLocker lock(&m_lock);
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->hasDebugOption();
}

void Settings::setCommandLineParser(CommandLineParser* _cmdLineParser) {
  QWriteLocker lock(&m_lock);
  Q_ASSERT(_cmdLineParser != nullptr);
  m_cmdLineParser = _cmdLineParser;
}

void Settings::init() {
  QFile cfgFile(getDataDir().absoluteFilePath("intensecoinwallet.cfg"));
  if (cfgFile.open(QIODevice::ReadOnly)) {
    m_settings = QJsonDocument::fromJson(cfgFile.readAll()).object();
    cfgFile.close();
  }

  if (isOptimizationEnabled()) {
	  setOptimizationEnabled(false);
  }

  restoreDefaultPoolList();
}

void Settings::restoreDefaultPoolList() {
  if (!m_settings.contains(OPTION_MINING_POOLS)) {
    setMiningPoolList(QStringList() << m_defaultPoolList);
  } else {
    QStringList poolList = getMiningPoolList();
    for (const QString& pool : m_defaultPoolList) {
      if (!poolList.contains(pool)) {
        poolList << pool;
      }
    }

    setMiningPoolList(poolList);
  }
}

bool Settings::isMiningOnLockedScreenEnabled(bool _defaultValue) const {
  QReadLocker lock(&m_lock);
  if (!m_settings.contains(OPTION_MINING_PARAMS)) {
    return _defaultValue;
  }

  QJsonObject miningParams = m_settings.value(OPTION_MINING_PARAMS).toObject();
  return miningParams.contains(OPTION_MINING_ON_LOCKED_SCREEN) ? miningParams.value(OPTION_MINING_ON_LOCKED_SCREEN).toBool() : _defaultValue;
}

bool Settings::isSystemTrayAvailable() const {
#ifdef Q_OS_MAC
  return false;
#else

#ifdef Q_OS_LINUX
  if (QString(qVersion()) < "5.5.0") {
    return false;
  }
#endif

  return true;
#endif
}

bool Settings::isTestnet() const {
  QReadLocker lock(&m_lock);
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->hasTestnetOption();
}

quintptr Settings::getStyleCount() const {
  QReadLocker lock(&m_lock);
  return m_styles.count();
}

const Style& Settings::getStyle(quintptr _styleIndex) const {
  QReadLocker lock(&m_lock);
  return *m_styles.values()[_styleIndex];
}

const Style& Settings::getCurrentStyle() const {
  QReadLocker lock(&m_lock);
  QString theme = m_settings.contains(OPTION_WALLET_THEME) ? m_settings.value(OPTION_WALLET_THEME).toString() : "dark";
  return *m_styles[theme];
}

bool Settings::hasAllowLocalIpOption() const {
  QReadLocker lock(&m_lock);
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->hasAllowLocalIpOption();
}

bool Settings::hasHideMyPortOption() const {
  QReadLocker lock(&m_lock);
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->hasHideMyPortOption();
}

QString Settings::getP2pBindIp() const {
  QReadLocker lock(&m_lock);
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getP2pBindIp();
}

quint16 Settings::getP2pBindPort() const {
  QReadLocker lock(&m_lock);
  Q_ASSERT(m_cmdLineParser != nullptr);
  if (m_p2pBindPort != 0) {
    return m_p2pBindPort;
  }

  return m_cmdLineParser->getP2pBindPort();
}

quint16 Settings::getP2pExternalPort() const {
  QReadLocker lock(&m_lock);
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getP2pExternalPort();
}

QStringList Settings::getPeers() const {
  QReadLocker lock(&m_lock);
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getPeers();
}

QStringList Settings::getPriorityNodes() const {
  QReadLocker lock(&m_lock);
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getPiorityNodes();
}

QStringList Settings::getExclusiveNodes() const {
  QReadLocker lock(&m_lock);
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getExclusiveNodes();
}

QStringList Settings::getSeedNodes() const {
  QReadLocker lock(&m_lock);
  Q_ASSERT(m_cmdLineParser != nullptr);
  return m_cmdLineParser->getSeedNodes();
}

QDir Settings::getDataDir() const {
  QReadLocker lock(&m_lock);
  Q_ASSERT(m_cmdLineParser != nullptr);
  return QDir(m_cmdLineParser->getDataDir());
}

QString Settings::getLegacyAddressBookFile() const {
  QString addressBookFile = getWalletFile();
  addressBookFile.replace(addressBookFile.lastIndexOf(".wallet"), 7, ".addressbook");
  return addressBookFile;
}

ConnectionMethod Settings::getConnectionMethod() const {
  QReadLocker lock(&m_lock);
  return m_settings.contains(OPTION_NODE_CONNECTION_METHOD) ?
    static_cast<ConnectionMethod>(m_settings.value(OPTION_NODE_CONNECTION_METHOD).toInt()) : ConnectionMethod::AUTO;
}

bool Settings::isOptimizationEnabled() const {
  QReadLocker lock(&m_lock);
  if (!m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
    return false;
  }

  QJsonObject optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
  return optimizationObject.contains(OPTION_WALLET_OPTIMIZATION_ENABLED) ? optimizationObject.value(OPTION_WALLET_OPTIMIZATION_ENABLED).toBool() : false;
}

bool Settings::isFusionTransactionsVisible() const {
  QReadLocker lock(&m_lock);
  if (!m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
    return false;
  }

  QJsonObject optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
  if (!optimizationObject.contains(OPTION_WALLET_OPTIMIZATION_FUSION_TARNSACTIONS_IS_VISIBLE)) {
    return false;
  }

  return optimizationObject.value(OPTION_WALLET_OPTIMIZATION_FUSION_TARNSACTIONS_IS_VISIBLE).toBool();
}

bool Settings::isOptimizationTimeSetManually() const {
  QReadLocker lock(&m_lock);
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
  QReadLocker lock(&m_lock);
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
  QReadLocker lock(&m_lock);
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
  QReadLocker lock(&m_lock);
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
  QReadLocker lock(&m_lock);
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
  QReadLocker lock(&m_lock);
  if (!m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
    return DEFAULT_OPTIMIZATION_MIXIN;
  }

  QJsonObject optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
  if (!optimizationObject.contains(OPTION_WALLET_OPTIMIZATION_MIXIN)) {
    return DEFAULT_OPTIMIZATION_MIXIN;
  }

  return optimizationObject.value(OPTION_WALLET_OPTIMIZATION_MIXIN).toString().toULongLong();
}

bool Settings::isNewsEnabled() const {
  QReadLocker lock(&m_lock);
  if (!m_settings.contains(OPTION_PRIVACY_PARAMS)) {
    return false;
  }

  QJsonObject privacyParams = m_settings.value(OPTION_PRIVACY_PARAMS).toObject();
  return privacyParams.contains(OPTION_PRIVACY_NEWS_ENABLED) ? privacyParams.value(OPTION_PRIVACY_NEWS_ENABLED).toBool() : false;
}

quint16 Settings::getLocalRpcPort() const {
  QReadLocker lock(&m_lock);
  return m_settings.value(OPTION_NODE_LOCAL_RPC_PORT).toInt(CryptoNote::RPC_DEFAULT_PORT);
}

QUrl Settings::getRemoteRpcUrl() const {
  QReadLocker lock(&m_lock);
  QUrl res;
  if (m_settings.contains(OPTION_NODE_REMOTE_RPC_URL)) {
    res = QUrl::fromUserInput(m_settings.value(OPTION_NODE_REMOTE_RPC_URL).toString());
  }

  return res;
}

QString Settings::getMiningPoolSwitchStrategy(const QString& _defaultValue) const {
  QReadLocker lock(&m_lock);
  if (!m_settings.contains(OPTION_MINING_PARAMS)) {
    return _defaultValue;
  }

  QJsonObject miningParams = m_settings.value(OPTION_MINING_PARAMS).toObject();
  return miningParams.contains(OPTION_MINING_POOL_SWITCH_STRATEGY) ?
    miningParams.value(OPTION_MINING_POOL_SWITCH_STRATEGY).toString() : _defaultValue;
}

quint32 Settings::getMiningCpuCoreCount(quint32 _defaultValue) const {
  QReadLocker lock(&m_lock);
  if (!m_settings.contains(OPTION_MINING_PARAMS)) {
    return _defaultValue;
  }

  QJsonObject miningParams = m_settings.value(OPTION_MINING_PARAMS).toObject();
  return miningParams.contains(OPTION_MINING_CPU_CORE_COUNT) ?
    miningParams.value(OPTION_MINING_CPU_CORE_COUNT).toVariant().toUInt() : _defaultValue;
}

QString Settings::getWalletFile() const {
  QReadLocker lock(&m_lock);
  return m_settings.contains(OPTION_WALLET_WALLET_FILE) ? m_settings.value(OPTION_WALLET_WALLET_FILE).toString() :
    getDataDir().absoluteFilePath(DEFAULT_WALLET_FILE_NAME);
}

bool Settings::isEncrypted() const {
  QReadLocker lock(&m_lock);
  return m_settings.contains(OPTION_WALLET_ENCRYPTED) ? m_settings.value(OPTION_WALLET_ENCRYPTED).toBool() : false;
}

QString Settings::getVersion() const {
  QReadLocker lock(&m_lock);
  return QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH);
}

QString Settings::getCurrentTheme() const {
  QReadLocker lock(&m_lock);
  return m_settings.contains(OPTION_WALLET_THEME) ? m_settings.value(OPTION_WALLET_THEME).toString() : "light";
}

QStringList Settings::getRecentWalletList() const {
  QReadLocker lock(&m_lock);
  QStringList res;
  if (m_settings.contains(OPTION_RECENT_WALLETS)) {
    QJsonArray recentWalletArray = m_settings.value(OPTION_RECENT_WALLETS).toArray();
    for (const auto& item : recentWalletArray) {
      res << item.toString();
    }
  }

  return res;
}

QStringList Settings::getMiningPoolList() const {
  QReadLocker lock(&m_lock);
  QStringList res;
  if (m_settings.contains(OPTION_MINING_POOLS)) {
    QJsonArray miningPoolArray = m_settings.value(OPTION_MINING_POOLS).toArray();
    for (const auto& item : miningPoolArray) {
      res << item.toString();
    }
  }

  return res;
}

bool Settings::isStartOnLoginEnabled() const {
  QReadLocker lock(&m_lock);
  bool res = false;
#ifdef Q_OS_MAC
  QDir autorunDir = QDir::home();
  if (!autorunDir.cd("Library") || !autorunDir.cd("LaunchAgents")) {
    return false;
  }

  QString autorunFilePath = autorunDir.absoluteFilePath("intensecoinwallet.plist");
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

  QString autorunFilePath = autorunDir.absoluteFilePath("intensecoinwallet.desktop");
  res = QFile::exists(autorunFilePath);
#elif defined(Q_OS_WIN)
  QSettings autorunSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
  res = autorunSettings.contains("IntensecoinWallet") &&
    !QDir::fromNativeSeparators(autorunSettings.value("IntensecoinWallet").toString().split(' ')[0]).compare(QCoreApplication::applicationFilePath());
#endif
  return res;
}

bool Settings::isRunMinimizedEnabled() const {
  QReadLocker lock(&m_lock);
  return m_cmdLineParser->hasMinimizedOption();
}

bool Settings::isMinimizeToTrayEnabled() const {
  QReadLocker lock(&m_lock);
  if (!isSystemTrayAvailable()) {
    return false;
  }

  return m_settings.contains(OPTION_MINIMIZE_TO_TRAY) ? m_settings.value(OPTION_MINIMIZE_TO_TRAY).toBool() : false;
}

bool Settings::isCloseToTrayEnabled() const {
  QReadLocker lock(&m_lock);
  if (!isSystemTrayAvailable()) {
    return false;
  }

  return m_settings.contains(OPTION_CLOSE_TO_TRAY) ? m_settings.value(OPTION_CLOSE_TO_TRAY).toBool() : false;
}

void Settings::setConnectionMethod(ConnectionMethod _connectionMethod) {
  if (getConnectionMethod() != _connectionMethod) {
    {
      QWriteLocker lock(&m_lock);
      m_settings.insert(OPTION_NODE_CONNECTION_METHOD, static_cast<int>(_connectionMethod));
      saveSettings();
    }

    notifyObservers();
  }
}

void Settings::setLocalRpcPort(quint16 _port) {
  if (getLocalRpcPort() != _port) {
    {
      QWriteLocker lock(&m_lock);
      m_settings.insert(OPTION_NODE_LOCAL_RPC_PORT, _port);
      saveSettings();
    }

    notifyObservers();
  }
}

void Settings::setRemoteRpcUrl(const QUrl& _url) {
  QUrl oldUrl = getRemoteRpcUrl();
  if (oldUrl.host() != _url.host() || oldUrl.port() != _url.port()) {
    {
      QWriteLocker lock(&m_lock);
      m_settings.insert(OPTION_NODE_REMOTE_RPC_URL, QString("%1:%2").arg(_url.host()).arg(_url.port()));
      saveSettings();
    }

    notifyObservers();
  }
}

void Settings::setP2pBindPort(quint16 _p2pBindPort) {
  {
    QWriteLocker lock(&m_lock);
    m_p2pBindPort = _p2pBindPort;
  }

  notifyObservers();
}

void Settings::setWalletFile(const QString& _file) {
  {
    QWriteLocker lock(&m_lock);
    if (_file.endsWith(".wallet") || _file.endsWith(".keys")) {
      m_settings.insert(OPTION_WALLET_WALLET_FILE, _file);
    } else {
      m_settings.insert(OPTION_WALLET_WALLET_FILE, _file + ".wallet");
    }

    saveSettings();
  }

  notifyObservers();
}

void Settings::setCurrentTheme(const QString& _theme) {
  if (getCurrentTheme() != _theme) {
    {
      QWriteLocker lock(&m_lock);
      m_settings.insert(OPTION_WALLET_THEME, _theme);
      saveSettings();
    }

    notifyObservers();
  }
}

void Settings::setRecentWalletList(const QStringList &_recentWalletList) {
  {
    QWriteLocker lock(&m_lock);
    m_settings.insert(OPTION_RECENT_WALLETS, QJsonArray::fromStringList(_recentWalletList));
    saveSettings();
  }

  notifyObservers();
}

void Settings::setMiningCpuCoreCount(quint32 _cpuCoreCount) {
  QWriteLocker lock(&m_lock);
  QJsonObject miningParamsObject = m_settings.value(OPTION_MINING_PARAMS).toObject();
  miningParamsObject.insert(OPTION_MINING_CPU_CORE_COUNT, static_cast<int>(_cpuCoreCount));
  m_settings.insert(OPTION_MINING_PARAMS, miningParamsObject);
  saveSettings();
}

void Settings::setMiningOnLockedScreenEnabled(bool _enable) {
  {
    QWriteLocker lock(&m_lock);
    QJsonObject miningParamsObject = m_settings.value(OPTION_MINING_PARAMS).toObject();
    miningParamsObject.insert(OPTION_MINING_ON_LOCKED_SCREEN, _enable);
    m_settings.insert(OPTION_MINING_PARAMS, miningParamsObject);
    saveSettings();
  }

  notifyObservers();
}


void Settings::setMiningPoolList(const QStringList &_miningPoolList) {
  {
    QWriteLocker lock(&m_lock);
    m_settings.insert(OPTION_MINING_POOLS, QJsonArray::fromStringList(_miningPoolList));
    saveSettings();
  }

  notifyObservers();
}

void Settings::setMiningPoolSwitchStrategy(const QString& _miningPoolSwitchStrategy) {
  {
    QWriteLocker lock(&m_lock);
    QJsonObject miningParamsObject = m_settings.value(OPTION_MINING_PARAMS).toObject();
    miningParamsObject.insert(OPTION_MINING_POOL_SWITCH_STRATEGY, _miningPoolSwitchStrategy);
    m_settings.insert(OPTION_MINING_PARAMS, miningParamsObject);
    saveSettings();
  }

  notifyObservers();
}

void Settings::setStartOnLoginEnabled(bool _enable) {
  {
    QWriteLocker lock(&m_lock);
#ifdef Q_OS_MAC
    QDir autorunDir = QDir::home();
    if (!autorunDir.cd("Library") || !autorunDir.cd("LaunchAgents")) {
      return;
    }

    QString autorunFilePath = autorunDir.absoluteFilePath("intensecoinwallet.plist");
    QSettings autorunSettings(autorunFilePath, QSettings::NativeFormat);
    autorunSettings.remove("Program");
    autorunSettings.setValue("Label", "org.intensecoin.intensecoinwallet");
    autorunSettings.setValue("ProgramArguments", QVariantList() << QCoreApplication::applicationFilePath() << "--minimized");
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

    QString autorunFilePath = autorunDir.absoluteFilePath("intensecoinwallet.desktop");
    QFile autorunFile(autorunFilePath);
    if (!autorunFile.open(QFile::WriteOnly | QFile::Truncate)) {
      return;
    }

    if (_enable) {
      autorunFile.write("[Desktop Entry]\n");
      autorunFile.write("Type=Application\n");
      autorunFile.write("Name=Intensecoin Wallet\n");
      autorunFile.write(QString("Exec=%1 --minimized\n").arg(QCoreApplication::applicationFilePath()).toLocal8Bit());
      autorunFile.write("Terminal=false\n");
      autorunFile.write("Hidden=false\n");
      autorunFile.close();
    } else {
      QFile::remove(autorunFilePath);
    }
#elif defined(Q_OS_WIN)
    QSettings autorunSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (_enable) {
      QString appPath = QString("%1 --minimized").arg(QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
      autorunSettings.setValue("IntensecoinWallet", appPath);
    } else {
      autorunSettings.remove("IntensecoinWallet");
    }
#endif
  }

  notifyObservers();
}

void Settings::setOptimizationEnabled(bool _enable) {
  if (_enable == isOptimizationEnabled()) {
    return;
  }

  {
    QWriteLocker lock(&m_lock);
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_ENABLED, _enable);
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }

  notifyObservers();
}

void Settings::setFusionTransactionsVisible(bool _visible) {
  if (_visible == isFusionTransactionsVisible()) {
    return;
  }

  {
    QWriteLocker lock(&m_lock);
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_FUSION_TARNSACTIONS_IS_VISIBLE, _visible);
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }

  notifyObservers();
}

void Settings::setOptimizationTimeSetManually(bool _enable) {
  if (_enable == isOptimizationTimeSetManually()) {
    return;
  }

  {
    QWriteLocker lock(&m_lock);
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_TIME_SET_MANUALLY, _enable);
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }

  notifyObservers();
}

void Settings::setOptimizationStartTime(const QTime& _startTime) {
  if (_startTime == getOptimizationStartTime()) {
    return;
  }

  {
    QWriteLocker lock(&m_lock);
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_START_TIME, _startTime.toString(Qt::ISODate));
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }

  notifyObservers();
}

void Settings::setOptimizationStopTime(const QTime& _stopTime) {
  if (_stopTime == getOptimizationStopTime()) {
    return;
  }

  {
    QWriteLocker lock(&m_lock);
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_STOP_TIME, _stopTime.toString(Qt::ISODate));
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }

  notifyObservers();
}

void Settings::setOptimizationInterval(quint64 _interval) {
  if (_interval == getOptimizationInterval()) {
    return;
  }

  {
    QWriteLocker lock(&m_lock);
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_INTERVAL, QString::number(_interval));
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }

  notifyObservers();
}

void Settings::setOptimizationThreshold(quint64 _threshold) {
  if (_threshold == getOptimizationThreshold()) {
    return;
  }

  {
    QWriteLocker lock(&m_lock);
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_THRESHOLD, QString::number(_threshold));
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }

  notifyObservers();
}

void Settings::setOptimizationMixin(quint64 _mixin) {
  if (_mixin == getOptimizationMixin()) {
    return;
  }

  {
    QWriteLocker lock(&m_lock);
    QJsonObject optimizationObject;
    if (m_settings.contains(OPTION_WALLET_OPTIMIZATION)) {
      optimizationObject = m_settings.value(OPTION_WALLET_OPTIMIZATION).toObject();
    }

    optimizationObject.insert(OPTION_WALLET_OPTIMIZATION_MIXIN, QString::number(_mixin));
    m_settings.insert(OPTION_WALLET_OPTIMIZATION, optimizationObject);
    saveSettings();
  }

  notifyObservers();
}

void Settings::setNewsEnabled(bool _enable) {
  if (_enable == isNewsEnabled()) {
    return;
  }

  {
    QWriteLocker lock(&m_lock);
    QJsonObject privacyObject;
    if (m_settings.contains(OPTION_PRIVACY_PARAMS)) {
      privacyObject = m_settings.value(OPTION_PRIVACY_PARAMS).toObject();
    }

    privacyObject.insert(OPTION_PRIVACY_NEWS_ENABLED, _enable);
    m_settings.insert(OPTION_PRIVACY_PARAMS, privacyObject);
    saveSettings();
  }

  notifyObservers();
}

void Settings::setMinimizeToTrayEnabled(bool _enable) {
  if (!isSystemTrayAvailable()) {
    return;
  }

  if (isMinimizeToTrayEnabled() != _enable) {
    {
      QWriteLocker lock(&m_lock);
      m_settings.insert(OPTION_MINIMIZE_TO_TRAY, _enable);
      saveSettings();
    }

    notifyObservers();
  }
}

void Settings::setCloseToTrayEnabled(bool _enable) {
  if (!isSystemTrayAvailable()) {
    return;
  }

  if (isCloseToTrayEnabled() != _enable) {
    {
      QWriteLocker lock(&m_lock);
      m_settings.insert(OPTION_CLOSE_TO_TRAY, _enable);
      saveSettings();
    }

    notifyObservers();
  }
}

void Settings::addObserver(ISettingsObserver* _settingsObserver) {
  Q_ASSERT(_settingsObserver != nullptr);
  Q_ASSERT(!m_observers.contains(_settingsObserver));
  m_observers.insert(_settingsObserver);
}

void Settings::removeObserver(ISettingsObserver* _settingsObserver) {
  Q_ASSERT(_settingsObserver != nullptr);
  Q_ASSERT(m_observers.contains(_settingsObserver));
  m_observers.remove(_settingsObserver);
}

#ifdef Q_OS_WIN
void Settings::setUrlHandler() {
  QWriteLocker lock(&m_lock);
  QSettings protocolSettings("HKEY_CURRENT_USER\\Software\\Classes\\intensecoin", QSettings::NativeFormat);
  protocolSettings.setValue(".", "URL:intensecoin");
  protocolSettings.setValue("URL Protocol", "");
  QSettings iconSettings("HKEY_CURRENT_USER\\Software\\Classes\\intensecoin\\DefaultIcon", QSettings::NativeFormat);
  iconSettings.setValue(".", QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
  QSettings openSettings("HKEY_CURRENT_USER\\Software\\Classes\\intensecoin\\shell\\open\\command", QSettings::NativeFormat);
  QString commandString("\"%1\" \"%2\"");
  openSettings.setValue(".", commandString.arg(QDir::toNativeSeparators(QCoreApplication::applicationFilePath())).arg("%1"));
}
#endif

void Settings::saveSettings() const {
  QFile cfgFile(QDir(m_cmdLineParser->getDataDir()).absoluteFilePath("intensecoinwallet.cfg"));
  if (cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    QJsonDocument cfg_doc(m_settings);
    cfgFile.write(cfg_doc.toJson());
    cfgFile.close();
  }
}

void Settings::notifyObservers() {
  for (ISettingsObserver* observer : m_observers) {
    observer->settingsUpdated();
  }
}

}
