// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QCommandLineParser>
#include <QObject>

namespace WalletGui {

class CommandLineParser : public QObject {
  Q_OBJECT

public:
  CommandLineParser(QObject* _parent);
  ~CommandLineParser();

  bool process(const QStringList& _argv);

  bool hasHelpOption() const;
  bool hasVersionOption() const;
  bool hasTestnetOption() const;
  bool hasMinimizedOption() const;
  bool hasAllowLocalIpOption() const;
  bool hasHideMyPortOption() const;
  QString getErrorText() const;
  QString getHelpText() const;
  QString getP2pBindIp() const;
  quint16 getP2pBindPort() const;
  quint16 getP2pExternalPort() const;
  QStringList getPeers() const;
  QStringList getPiorityNodes() const;
  QStringList getExclusiveNodes() const;
  QStringList getSeedNodes() const;
  QString getDataDir() const;

private:
  QCommandLineParser m_parser;
  QCommandLineOption m_helpOption;
  QCommandLineOption m_versionOption;
  QCommandLineOption m_testnetOption;
  QCommandLineOption m_p2pBindIpOption;
  QCommandLineOption m_p2pBindPortOption;
  QCommandLineOption m_p2pExternalOption;
  QCommandLineOption m_allowLocalIpOption;
  QCommandLineOption m_addPeerOption;
  QCommandLineOption m_addPriorityNodeOption;
  QCommandLineOption m_addExclusiveNodeOption;
  QCommandLineOption m_seedNodeOption;
  QCommandLineOption m_hideMyPortOption;
  QCommandLineOption m_dataDirOption;
  QCommandLineOption m_minimized;
};

}
