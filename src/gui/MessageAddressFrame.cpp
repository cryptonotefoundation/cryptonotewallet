// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QApplication>
#include <QClipboard>

#include "MessageAddressFrame.h"
#include "AddressBookDialog.h"
#include "AliasProvider.h"
#include "MainWindow.h"
#include "ui_messageaddressframe.h"

namespace WalletGui {

namespace {
  Q_DECL_CONSTEXPR quint32 MESSAGE_ADDRESS_INPUT_INTERVAL = 1500;
}

MessageAddressFrame::MessageAddressFrame(QWidget* _parent) : QFrame(_parent),
  m_ui(new Ui::MessageAddressFrame), m_aliasProvider(new AliasProvider(this)), m_addressInputTimerId(-1) {
  m_ui->setupUi(this);

  connect(m_aliasProvider, &AliasProvider::aliasFoundSignal, this, &MessageAddressFrame::onAliasFound);
}

MessageAddressFrame::~MessageAddressFrame() {

}

QString MessageAddressFrame::getAddress() const {
  return m_ui->m_addressEdit->text();
}

void MessageAddressFrame::disableRemoveButton(bool _disable) {
  m_ui->m_removeButton->setDisabled(_disable);
}

void MessageAddressFrame::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_addressInputTimerId) {
    m_aliasProvider->getAddresses(m_ui->m_addressEdit->text().trimmed());
    killTimer(m_addressInputTimerId);
    m_addressInputTimerId = -1;
    return;
  }

  QFrame::timerEvent(_event);
}

void MessageAddressFrame::onAliasFound(const QString& _name, const QString& _address) {
  m_ui->m_addressEdit->setText(QString("%1 <%2>").arg(_name).arg(_address));
}

void MessageAddressFrame::addressBookClicked() {
  AddressBookDialog dlg(&MainWindow::instance());
  if(dlg.exec() == QDialog::Accepted) {
    m_ui->m_addressEdit->setText(dlg.getAddress());
  }
}

void MessageAddressFrame::addressEdited(const QString& _text) {
  if (m_addressInputTimerId != -1) {
    killTimer(m_addressInputTimerId);
  }

  m_addressInputTimerId = startTimer(MESSAGE_ADDRESS_INPUT_INTERVAL);
}

void MessageAddressFrame::pasteClicked() {
  m_ui->m_addressEdit->setText(QApplication::clipboard()->text());
}

}
