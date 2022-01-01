// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2022 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "Logging/LoggerManager.h"

namespace WalletGui {

class LoggerAdapter {

public:
  static LoggerAdapter& instance();
  void init();
  void log(std::string message);
  Logging::LoggerManager& getLoggerManager();

private:
  Logging::LoggerManager m_logManager;

  LoggerAdapter();
  ~LoggerAdapter();
};

}
