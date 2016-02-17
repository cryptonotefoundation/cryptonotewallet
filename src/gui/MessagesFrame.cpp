// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "MessagesFrame.h"
#include "MainWindow.h"
#include "MessageDetailsDialog.h"
#include "MessagesModel.h"
#include "SortedMessagesModel.h"
#include "VisibleMessagesModel.h"

#include "ui_messagesframe.h"

namespace WalletGui {

MessagesFrame::MessagesFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::MessagesFrame),
  m_visibleMessagesModel(new VisibleMessagesModel) {
  m_ui->setupUi(this);
  m_ui->m_messagesView->setModel(m_visibleMessagesModel.data());
  m_ui->m_messagesView->header()->resizeSection(MessagesModel::COLUMN_DATE, 140);
  m_ui->m_replyButton->setEnabled(false);

  connect(m_ui->m_messagesView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MessagesFrame::currentMessageChanged);
}

MessagesFrame::~MessagesFrame() {
}

void MessagesFrame::currentMessageChanged(const QModelIndex& _currentIndex) {
  m_ui->m_replyButton->setEnabled(_currentIndex.isValid() && !_currentIndex.data(MessagesModel::ROLE_HEADER_REPLY_TO).toString().isEmpty());
}

void MessagesFrame::messageDoubleClicked(const QModelIndex& _index) {
  if (!_index.isValid()) {
    return;
  }

  MessageDetailsDialog dlg(_index, &MainWindow::instance());
  if (dlg.exec() == QDialog::Accepted) {
    Q_EMIT replyToSignal(dlg.getCurrentMessageIndex());
  }
}

void MessagesFrame::replyClicked() {
  Q_EMIT replyToSignal(m_ui->m_messagesView->selectionModel()->currentIndex());
}

}
