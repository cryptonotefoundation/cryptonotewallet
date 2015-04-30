// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QCommandLineParser>
#include <QObject>

namespace WalletGui {

class CommandLineParser : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(CommandLineParser)

public:
  CommandLineParser();
  ~CommandLineParser();

  bool process();

  bool hasHelpOption() const;
  bool hasVersionOption() const;
  QString errorText() const;
  QString helpText() const;
  QString getDataDir() const;
private:
  QCommandLineParser m_parser;
  QCommandLineOption m_help_option;
  QCommandLineOption m_version_option;
  QCommandLineOption m_data_dir_option;
};

}
