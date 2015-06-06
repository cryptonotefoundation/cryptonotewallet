// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2015 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QFrame>

namespace Ui {
class TransferFrame;
}

namespace WalletGui {

class TransferFrame : public QFrame {
  Q_OBJECT
  Q_DISABLE_COPY(TransferFrame)

public:
  TransferFrame(QWidget* _parent);
  ~TransferFrame();

  QString getAddress() const;
  QString getLabel() const;
  qreal getAmount() const;
  QString getAmountString() const;
  QString getComment() const;

  void disableRemoveButton(bool _disable);
  void setAddress(const QString& _address);

private:
  QScopedPointer<Ui::TransferFrame> m_ui;

  Q_SLOT void addressBookClicked();
  Q_SLOT void pasteClicked();
};

}
