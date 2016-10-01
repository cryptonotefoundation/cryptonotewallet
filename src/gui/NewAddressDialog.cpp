// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QRegExpValidator>
#include "NewAddressDialog.h"

#include "ui_newaddressdialog.h"

namespace WalletGui {

NewAddressDialog::NewAddressDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::NewAddressDialog) {
  m_ui->setupUi(this);

  QRegExp hexMatcher("^[0-9A-F]{64}$", Qt::CaseInsensitive);
  QValidator *validator = new QRegExpValidator(hexMatcher, this);
  m_ui->m_contactPaymentIdEdit->setValidator(validator);
}

NewAddressDialog::~NewAddressDialog() {
}

QString NewAddressDialog::getAddress() const {
  return m_ui->m_addressEdit->text();
}

QString NewAddressDialog::getPaymentID() const {
  return m_ui->m_contactPaymentIdEdit->text();
}

QString NewAddressDialog::getLabel() const {
  return m_ui->m_labelEdit->text();
}

void NewAddressDialog::setEditLabel(QString label) {
  m_ui->m_labelEdit->setText(label);
}

void NewAddressDialog::setEditAddress(QString address) {
  m_ui->m_addressEdit->setText(address);
}

void NewAddressDialog::setEditPaymentId(QString paymentid) {
  m_ui->m_contactPaymentIdEdit->setText(paymentid);
}

}
