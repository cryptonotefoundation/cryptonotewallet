// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QCoreApplication>

#include <common/util.h>

#include "CommandLineParser.h"
#include "CurrencyAdapter.h"

namespace WalletGui {

CommandLineParser::CommandLineParser() : QObject(), m_parser(), m_help_option(m_parser.addHelpOption()),
  m_version_option(m_parser.addVersionOption()),
  m_data_dir_option("data-dir", tr("Specify data directory"), tr("directory"), QString::fromStdString(tools::get_default_data_dir())) {
  m_parser.setApplicationDescription(QString(tr("%1 wallet")).arg(CurrencyAdapter::instance().getCurrencyDisplayName()));
  m_parser.addHelpOption();
  m_parser.addVersionOption();
  m_parser.addOption(m_data_dir_option);
}

CommandLineParser::~CommandLineParser() {
}

bool CommandLineParser::process() {
#ifdef Q_OS_WIN
  return m_parser.parse(QCoreApplication::arguments());
#else
  m_parser.process(*QCoreApplication::instance());
  return true;
#endif
}

bool CommandLineParser::hasHelpOption() const {
  return m_parser.isSet(m_help_option);
}

bool CommandLineParser::hasVersionOption() const {
  return m_parser.isSet(m_version_option);
}

QString CommandLineParser::errorText() const {
  return m_parser.errorText();
}

QString CommandLineParser::helpText() const {
  return m_parser.helpText();
}

QString CommandLineParser::getDataDir() const {
  return m_parser.value(m_data_dir_option);
}

}
