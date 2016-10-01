// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QApplication>
#include <QClipboard>

#include "CurrencyAdapter.h"
#include "AddressBookModel.h"
#include "AddressBookFrame.h"
#include "MainWindow.h"
#include "NewAddressDialog.h"
#include "WalletEvents.h"

#include "ui_addressbookframe.h"

namespace WalletGui {

AddressBookFrame::AddressBookFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::AddressBookFrame) {
  m_ui->setupUi(this);
  m_ui->m_addressBookView->setModel(&AddressBookModel::instance());
  m_ui->m_addressBookView->header()->setStretchLastSection(false);
  m_ui->m_addressBookView->header()->setSectionResizeMode(1, QHeaderView::Stretch);

  connect(m_ui->m_addressBookView->selectionModel(), &QItemSelectionModel::currentChanged, this, &AddressBookFrame::currentAddressChanged);
}

AddressBookFrame::~AddressBookFrame() {
}

void AddressBookFrame::addClicked() {
  NewAddressDialog dlg(&MainWindow::instance());
  if (dlg.exec() == QDialog::Accepted) {
    QString label = dlg.getLabel();
    QString address = dlg.getAddress();
    QByteArray paymentid = dlg.getPaymentID().toUtf8();
    if (!CurrencyAdapter::instance().validateAddress(address)) {
      QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Invalid address"), QtCriticalMsg));
      return;
    }

    if (!isValidPaymentId(paymentid)) {
      QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Invalid payment ID"), QtCriticalMsg));
      return;
    }
    AddressBookModel::instance().addAddress(label, address, paymentid);
  }
}

void AddressBookFrame::editClicked() {
   NewAddressDialog dlg(&MainWindow::instance());
   dlg.setWindowTitle(QString(tr("Edit contact")));
   dlg.setEditLabel(m_ui->m_addressBookView->currentIndex().data(AddressBookModel::ROLE_LABEL).toString());
   dlg.setEditAddress(m_ui->m_addressBookView->currentIndex().data(AddressBookModel::ROLE_ADDRESS).toString());
   dlg.setEditPaymentId(m_ui->m_addressBookView->currentIndex().data(AddressBookModel::ROLE_PAYMENTID).toString());
   if (dlg.exec() == QDialog::Accepted) {
     QString label = dlg.getLabel();
     QString address = dlg.getAddress();
     QByteArray paymentid = dlg.getPaymentID().toUtf8();
     if (!CurrencyAdapter::instance().validateAddress(address)) {
       QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Invalid address"), QtCriticalMsg));
       return;
     }

     if (!isValidPaymentId(paymentid)) {
       QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Invalid payment ID"), QtCriticalMsg));
       return;
     }
     AddressBookModel::instance().addAddress(label, address, paymentid);
   deleteClicked();
   }
}

void AddressBookFrame::copyClicked() {
  QApplication::clipboard()->setText(m_ui->m_addressBookView->currentIndex().data(AddressBookModel::ROLE_ADDRESS).toString());
}

void AddressBookFrame::copyPaymentIdClicked() {
  QApplication::clipboard()->setText(m_ui->m_addressBookView->currentIndex().data(AddressBookModel::ROLE_PAYMENTID).toString());
}

void AddressBookFrame::deleteClicked() {
  int row = m_ui->m_addressBookView->currentIndex().row();
  AddressBookModel::instance().removeAddress(row);
}

void AddressBookFrame::currentAddressChanged(const QModelIndex& _index) {
  m_ui->m_copyAddressButton->setEnabled(_index.isValid());
  m_ui->m_deleteAddressButton->setEnabled(_index.isValid());
  m_ui->m_editAddressButton->setEnabled(_index.isValid());
  m_ui->m_copyPaymentIdButton->setDisabled(_index.isValid());
  if(!_index.data(AddressBookModel::ROLE_PAYMENTID).toString().isEmpty()){
    m_ui->m_copyPaymentIdButton->setEnabled(_index.isValid());
  }
}

bool AddressBookFrame::isValidPaymentId(const QByteArray& _paymentIdString) {
  if (_paymentIdString.isEmpty()) {
    return true;
  }

  QByteArray paymentId = QByteArray::fromHex(_paymentIdString);
  return (paymentId.size() == sizeof(Crypto::Hash)) && (_paymentIdString.toUpper() == paymentId.toHex().toUpper());
}

}
