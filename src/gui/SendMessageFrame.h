// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QFrame>

#include <IWalletLegacy.h>

namespace Ui {
class SendMessageFrame;
}

namespace WalletGui {

class AliasProvider;

class SendMessageFrame : public QFrame {
  Q_OBJECT

public:
  SendMessageFrame(QWidget* _parent);
  ~SendMessageFrame();

  void setAddress(const QString& _address);

protected:
  void timerEvent(QTimerEvent* _event) Q_DECL_OVERRIDE;

private:
  QScopedPointer<Ui::SendMessageFrame> m_ui;
  AliasProvider* m_aliasProvider;
  int m_addressInputTimer;

  void onAliasFound(const QString& _name, const QString& _address);
  void sendMessageCompleted(CryptoNote::TransactionId _transactionId, bool _error, const QString& _errorText);
  void reset();

  QString getAddress() const;

  Q_SLOT void addressBookClicked();
  Q_SLOT void addressEdited(const QString& _text);
  Q_SLOT void mixinValueChanged(int _value);
  Q_SLOT void pasteClicked();
  Q_SLOT void sendClicked();
};

}
