// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2015 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QFrame>

#include <IWallet.h>

namespace Ui {
class SendMessageFrame;
}

namespace WalletGui {

class SendMessageFrame : public QFrame {
  Q_OBJECT

public:
  SendMessageFrame(QWidget* _parent);
  ~SendMessageFrame();

  void setAddress(const QString& _address);

private:
  QScopedPointer<Ui::SendMessageFrame> m_ui;

  void sendMessageCompleted(CryptoNote::TransactionId _transactionId, bool _error, const QString& _errorText);
  void reset();

  Q_SLOT void addressBookClicked();
  Q_SLOT void mixinValueChanged(int _value);
  Q_SLOT void pasteClicked();
  Q_SLOT void sendClicked();
};

}
