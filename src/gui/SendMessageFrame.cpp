// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QClipboard>

#include "SendMessageFrame.h"
#include "AddressBookDialog.h"
#include "AliasProvider.h"
#include "CurrencyAdapter.h"
#include "MainWindow.h"
#include "Message.h"
#include "MessagesModel.h"
#include "WalletAdapter.h"
#include "WalletEvents.h"

#include "ui_sendmessageframe.h"

namespace WalletGui {

Q_DECL_CONSTEXPR quint64 MESSAGE_AMOUNT = 100000;
Q_DECL_CONSTEXPR quint64 MINIMAL_MESSAGE_FEE = 100000000;
Q_DECL_CONSTEXPR int DEFAULT_MESSAGE_MIXIN = 2;
Q_DECL_CONSTEXPR quint32 MESSAGE_ADDRESS_INPUT_INTERVAL = 1500;

SendMessageFrame::SendMessageFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::SendMessageFrame),
  m_aliasProvider(new AliasProvider(this)), m_addressInputTimer(-1) {
  m_ui->setupUi(this);
  m_ui->m_mixinSlider->setValue(DEFAULT_MESSAGE_MIXIN);
  m_ui->m_feeSpin->setMinimum(CurrencyAdapter::instance().formatAmount(MESSAGE_AMOUNT + MINIMAL_MESSAGE_FEE).toDouble());
  connect(&WalletAdapter::instance(), &WalletAdapter::walletSendMessageCompletedSignal, this, &SendMessageFrame::sendMessageCompleted,
    Qt::QueuedConnection);
  connect(m_aliasProvider, &AliasProvider::aliasFoundSignal, this, &SendMessageFrame::onAliasFound);
}

SendMessageFrame::~SendMessageFrame() {
}

void SendMessageFrame::setAddress(const QString& _address) {
  m_ui->m_addressEdit->setText(_address);
  m_ui->m_addReplyToCheck->setChecked(true);
}

void SendMessageFrame::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_addressInputTimer) {
    m_aliasProvider->getAddresses(m_ui->m_addressEdit->text().trimmed());
    killTimer(m_addressInputTimer);
    m_addressInputTimer = -1;
    return;
  }

  QFrame::timerEvent(_event);
}

void SendMessageFrame::onAliasFound(const QString& _name, const QString& _address) {
  m_ui->m_addressEdit->setText(QString("%1 <%2>").arg(_name).arg(_address));
}

void SendMessageFrame::sendMessageCompleted(CryptoNote::TransactionId _transactionId, bool _error, const QString& _errorText) {
  Q_UNUSED(_transactionId);
  if (_error) {
    QCoreApplication::postEvent(
      &MainWindow::instance(),
      new ShowMessageEvent(_errorText, QtCriticalMsg));
  } else {
    reset();
  }
}

void SendMessageFrame::reset() {
  m_ui->m_mixinSlider->setValue(DEFAULT_MESSAGE_MIXIN);
  m_ui->m_feeSpin->setValue(m_ui->m_feeSpin->minimum());
  m_ui->m_addressEdit->clear();
  m_ui->m_messageTextEdit->clear();
}

QString SendMessageFrame::getAddress() const {
  QString address = m_ui->m_addressEdit->text().trimmed();
  if (address.contains('<')) {
    int startPos = address.indexOf('<');
    int endPos = address.indexOf('>');
    address = address.mid(startPos + 1, endPos - startPos - 1);
  }

  return address;
}

void SendMessageFrame::addressBookClicked() {
  AddressBookDialog dlg(&MainWindow::instance());
  if(dlg.exec() == QDialog::Accepted) {
    m_ui->m_addressEdit->setText(dlg.getAddress());
  }
}

void SendMessageFrame::addressEdited(const QString& _text) {
  if (m_addressInputTimer != -1) {
    killTimer(m_addressInputTimer);
  }

  m_addressInputTimer = startTimer(MESSAGE_ADDRESS_INPUT_INTERVAL);
}

void SendMessageFrame::mixinValueChanged(int _value) {
  m_ui->m_mixinEdit->setText(QString::number(_value));
}

void SendMessageFrame::pasteClicked() {
  m_ui->m_addressEdit->setText(QApplication::clipboard()->text());
}

void SendMessageFrame::sendClicked() {
  if (!WalletAdapter::instance().isOpen()) {
    return;
  }

  QString address = getAddress();
  if (!CurrencyAdapter::instance().validateAddress(address)) {
    QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Invalid recipient address"), QtCriticalMsg));
    return;
  }

  QString messageString = m_ui->m_messageTextEdit->toPlainText();
  if (m_ui->m_addReplyToCheck->isChecked()) {
    MessageHeader header;
    header.append(qMakePair(QString(MessagesModel::HEADER_REPLY_TO_KEY), WalletAdapter::instance().getAddress()));
    messageString = Message::makeTextMessage(messageString, header);
  }

  CryptoNote::WalletLegacyTransfer transfer;
  transfer.address = address.toStdString();
  quint64 fee = CurrencyAdapter::instance().parseAmount(m_ui->m_feeSpin->cleanText());
  transfer.amount = MESSAGE_AMOUNT;
  CryptoNote::TransactionMessage message;
  message.address = address.toStdString();
  message.message = messageString.toStdString();
  if (fee < MESSAGE_AMOUNT + MINIMAL_MESSAGE_FEE) {
    QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Incorrect fee value"), QtCriticalMsg));
    return;
  }

  if (WalletAdapter::instance().isOpen()) {
    WalletAdapter::instance().sendMessage(QVector<CryptoNote::WalletLegacyTransfer>() << transfer, fee - MESSAGE_AMOUNT, m_ui->m_mixinSlider->value(),
      QVector<CryptoNote::TransactionMessage>() << message);
  }
}

}
