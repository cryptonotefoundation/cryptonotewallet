// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QFrame>

namespace Ui {
class MessagesFrame;
}

namespace WalletGui {

class VisibleMessagesModel;

class MessagesFrame : public QFrame {
  Q_OBJECT

public:
  MessagesFrame(QWidget* _parent);
  ~MessagesFrame();

private:
  QScopedPointer<Ui::MessagesFrame> m_ui;
  QScopedPointer<VisibleMessagesModel> m_visibleMessagesModel;

  void currentMessageChanged(const QModelIndex& _currentIndex);

  Q_SLOT void messageDoubleClicked(const QModelIndex& _index);
  Q_SLOT void replyClicked();

Q_SIGNALS:
  void replyToSignal(const QModelIndex& _index);
};

}
