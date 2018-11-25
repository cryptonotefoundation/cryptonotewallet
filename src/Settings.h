// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016 Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDir>
#include <QJsonObject>
#include <QObject>

namespace WalletGui {

class CommandLineParser;

class Settings : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(Settings)

public:
  static Settings& instance();

  void setCommandLineParser(CommandLineParser* _cmd_line_parser);
  void load();

  bool hasAllowLocalIpOption() const;
  bool hasHideMyPortOption() const;
  bool isTestnet() const;
  QDir getDataDir() const;
  QString getP2pBindIp() const;
  quint16 getLocalRpcPort() const;
  quint16 getP2pBindPort() const;
  quint16 getP2pExternalPort() const;
  QStringList getExclusiveNodes() const;
  QStringList getPeers() const;
  QStringList getPriorityNodes() const;
  QStringList getSeedNodes() const;

  QString getWalletFile() const;
  QStringList getRecentWalletsList() const;
  QString getAddressBookFile() const;
  QString getVersion() const;
  QString getLanguage() const;
  QString getConnection() const;
  QStringList getRpcNodesList() const;
  quint16 getCurrentLocalDaemonPort() const;
  QString getCurrentRemoteNode() const;
  QString getCurrentPool() const;
  QString getCurrentTheme() const;

  bool isOptimizationEnabled() const;
  bool isOptimizationTimeSetManually() const;
  QTime getOptimizationStartTime() const;
  QTime getOptimizationStopTime() const;
  quint64 getOptimizationInterval() const;
  quint64 getOptimizationThreshold() const;
  quint64 getOptimizationMixin() const;

  bool isEncrypted() const;
  bool isStartOnLoginEnabled() const;
  bool isTrackingMode() const;

#ifdef Q_OS_WIN
  bool isMinimizeToTrayEnabled() const;
  bool isCloseToTrayEnabled() const;
#endif

  void setWalletFile(const QString& _file);
  void setEncrypted(bool _encrypted);
  void setTrackingMode(bool _tracking);
  void setCurrentTheme(const QString& _theme);
  void setLanguage(const QString& _language);
  void setStartOnLoginEnabled(bool _enable);
  void setConnection(const QString& _connection);
  void setCurrentLocalDaemonPort(const quint16& _daemonPort);
  void setCurrentRemoteNode(const QString& _remoteNode);
  void setRpcNodesList(const QStringList& _RpcNodesList);
#ifdef Q_OS_WIN
  void setMinimizeToTrayEnabled(bool _enable);
  void setCloseToTrayEnabled(bool _enable);
#endif

  void setOptimizationEnabled(bool _enable);
  void setOptimizationTimeSetManually(bool _enable);
  void setOptimizationStartTime(const QTime& _startTime);
  void setOptimizationStopTime(const QTime& _stopTime);
  void setOptimizationInterval(quint64 _interval);
  void setOptimizationThreshold(quint64 _threshold);
  void setOptimizationMixin(quint64 _mixin);

private:
  QJsonObject m_settings;
  QString m_addressBookFile;
  QString m_currentLang;
  QString m_connectionMode;
  quint16 m_daemonPort;
  CommandLineParser* m_cmdLineParser;

  Settings();
  ~Settings();

  void saveSettings() const;
};

}
