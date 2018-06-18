// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2015 XDN developers
// Copyright (c) 2016-2017 The Karbovanets developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QApplication>
#include <QClipboard>

#include "TransferFrame.h"
#include "AddressBookDialog.h"
#include "MainWindow.h"
#include "CurrencyAdapter.h"
#include "DnsLookup.h"

#include "ui_transferframe.h"

namespace WalletGui {

Q_DECL_CONSTEXPR quint32 ADDRESS_INPUT_INTERVAL = 1500;

TransferFrame::TransferFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::TransferFrame), m_aliasProvider(new DnsManager(this)), m_addressInputTimer(-1) {
  m_ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  m_ui->m_amountSpin->setSuffix(" " + CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  connect(m_aliasProvider, &DnsManager::aliasFoundSignal, this, &TransferFrame::onAliasFound);
}

TransferFrame::~TransferFrame() {
}

QString TransferFrame::getAddress() const {
  QString address = m_ui->m_addressEdit->text().trimmed();
    if (address.contains('<')) {
      int startPos = address.indexOf('<');
      int endPos = address.indexOf('>');
      address = address.mid(startPos + 1, endPos - startPos - 1);
    }
  return address;
}

QString TransferFrame::getLabel() const {
  return m_ui->m_labelEdit->text().trimmed();
}

qreal TransferFrame::getAmount() const {
  return m_ui->m_amountSpin->value();
}

QString TransferFrame::getAmountString() const {
  return m_ui->m_amountSpin->cleanText();
}

void TransferFrame::disableRemoveButton(bool _disable) {
  m_ui->m_removeButton->setDisabled(_disable);
}

void TransferFrame::addressBookClicked() {
  AddressBookDialog dlg(&MainWindow::instance());
  if(dlg.exec() == QDialog::Accepted) {
    m_ui->m_addressEdit->setText(dlg.getAddress());
    Q_EMIT insertPaymentIDSignal(dlg.getPaymentID());
  }
}

void TransferFrame::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_addressInputTimer) {
    m_aliasProvider->getAddresses(m_ui->m_addressEdit->text().trimmed());
    killTimer(m_addressInputTimer);
    m_addressInputTimer = -1;
    return;
  }

  QFrame::timerEvent(_event);
}

void TransferFrame::onAliasFound(const QString& _name, const QString& _address) {
  m_ui->m_addressEdit->setText(QString("%1 <%2>").arg(_name).arg(_address));
}

void TransferFrame::addressEdited(const QString& _text) {
  if(!_text.isEmpty() && _text.contains('.')) {
    if (m_addressInputTimer != -1) {
      killTimer(m_addressInputTimer);
    }
    m_addressInputTimer = startTimer(ADDRESS_INPUT_INTERVAL);
  }
}

void TransferFrame::pasteClicked() {
  m_ui->m_addressEdit->setText(QApplication::clipboard()->text());
}

void TransferFrame::amountValueChange() {
  Q_EMIT amountValueChangedSignal();
}

void TransferFrame::setAddress(QString _address) {
  m_ui->m_addressEdit->setText(_address);
}

void TransferFrame::setLabel(QString _label) {
  m_ui->m_labelEdit->setText(_label);
}

void TransferFrame::setAmount(quint64 _amount) {
  m_ui->m_amountSpin->setValue(CurrencyAdapter::instance().formatAmount(_amount).toDouble());
}

}
