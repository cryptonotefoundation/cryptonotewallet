// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QObject>

namespace WalletGui {

class SignalHandler : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(SignalHandler)

public:
  static SignalHandler& instance();

  void init();

private:
  SignalHandler();
  ~SignalHandler();

  static void sigHandler(int _signal);

Q_SIGNALS:
  void quitSignal();
};

}
