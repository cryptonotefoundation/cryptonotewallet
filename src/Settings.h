// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2015 XDN developers
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

  QDir getDataDir() const;
  QString getWalletFile() const;
  QString getAddressBookFile() const;
  QString getVersion() const;
  QStringList getMiningPoolList() const;
  bool isEncrypted() const;
  bool isStartOnLoginEnabled() const;
#ifdef Q_OS_WIN
  bool isMinimizeToTrayEnabled() const;
  bool isCloseToTrayEnabled() const;
#endif

  void setWalletFile(const QString& _file);
  void setEncrypted(bool _encrypted);
  void setStartOnLoginEnabled(bool _enable);
  void setMiningPoolList(const QStringList& _miningPoolList);
#ifdef Q_OS_WIN
  void setMinimizeToTrayEnabled(bool _enable);
  void setCloseToTrayEnabled(bool _enable);
#endif

private:
  QJsonObject m_settings;
  QString m_addressBookFile;
  CommandLineParser* m_cmdLineParser;

  Settings();
  ~Settings();

  void saveSettings() const;
};

}
