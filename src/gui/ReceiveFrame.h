// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016 The Karbovanets developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QFrame>
#include "AccountFrame.h"

namespace Ui {
class ReceiveFrame;
}

namespace WalletGui {

class ReceiveFrame : public QFrame {
  Q_OBJECT
  Q_DISABLE_COPY(ReceiveFrame)

public:
  ReceiveFrame(QWidget* _parent);
  ~ReceiveFrame();

  Q_SLOT void closePaymentRequestForm();

private:
  QScopedPointer<Ui::ReceiveFrame> m_ui;

  const WalletGui::AccountFrame * accoframe;

  void updateWalletAddress(const QString& _address);
  void walletClosed();
  QString wallet_address;
  QString requestUri;

  Q_SLOT void copyAddress();
  Q_SLOT void saveQRcodeToFile();
  Q_SLOT void requestPaymentClicked();
  Q_SLOT void generatePaymentIdClicked();

  Q_SLOT void createRequestPaymentClicked();

};

}
