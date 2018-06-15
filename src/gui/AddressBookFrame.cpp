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
  m_ui->m_addressBookView->setSortingEnabled(true);
  m_ui->m_addressBookView->sortByColumn(0, Qt::AscendingOrder);

  connect(m_ui->m_addressBookView->selectionModel(), &QItemSelectionModel::currentChanged, this, &AddressBookFrame::currentAddressChanged);

  m_ui->m_addressBookView->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_ui->m_addressBookView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

  contextMenu = new QMenu();
  contextMenu->addAction(QString(tr("&Pay to")), this, SLOT(payToClicked()));
  contextMenu->addAction(QString(tr("Copy &label")), this, SLOT(copyLabelClicked()));
  contextMenu->addAction(QString(tr("Copy &address")), this, SLOT(copyClicked()));
  contextMenu->addAction(QString(tr("Copy Payment &ID")), this, SLOT(copyPaymentIdClicked()));
  contextMenu->addAction(QString(tr("&Edit")), this, SLOT(editClicked()));
  contextMenu->addAction(QString(tr("&Delete")), this, SLOT(deleteClicked()));
}

AddressBookFrame::~AddressBookFrame() {
}

void AddressBookFrame::onCustomContextMenu(const QPoint &point) {
  index = m_ui->m_addressBookView->indexAt(point);
  contextMenu->exec(m_ui->m_addressBookView->mapToGlobal(point));
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

    QModelIndex contactIndex = AddressBookModel::instance().indexFromContact(label,0);
    QString contactLabel = contactIndex.data(AddressBookModel::ROLE_LABEL).toString();
    if(label == contactLabel) {
      QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Contact with such label already exists."), QtCriticalMsg));
      //label = QString(label + "%1").arg(label.toInt()+1);
      NewAddressDialog dlg(&MainWindow::instance());
      dlg.setEditLabel(label);
      dlg.setEditAddress(address);
      dlg.setEditPaymentId(paymentid);
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

        QModelIndex contactIndex = AddressBookModel::instance().indexFromContact(label,0);
        QString contactLabel = contactIndex.data(AddressBookModel::ROLE_LABEL).toString();
        if(label == contactLabel) {
          QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Contact with such label already exists."), QtCriticalMsg));
          return;
        }
        AddressBookModel::instance().addAddress(label, address, paymentid);
      }
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

     QModelIndex contactIndex = AddressBookModel::instance().indexFromContact(label,0);
     QString contactLabel = contactIndex.data(AddressBookModel::ROLE_LABEL).toString();
     if(label == contactLabel) {
       QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Contact with such label already exists."), QtCriticalMsg));
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

void AddressBookFrame::copyLabelClicked() {
  QApplication::clipboard()->setText(m_ui->m_addressBookView->currentIndex().data(AddressBookModel::ROLE_LABEL).toString());
}

void AddressBookFrame::deleteClicked() {
  int row = m_ui->m_addressBookView->currentIndex().row();
  AddressBookModel::instance().removeAddress(row);
  m_ui->m_copyPaymentIdButton->setEnabled(false);
  currentAddressChanged(m_ui->m_addressBookView->currentIndex());
}

void AddressBookFrame::payToClicked() {
  Q_EMIT payToSignal(m_ui->m_addressBookView->currentIndex());
}

void AddressBookFrame::addressDoubleClicked(const QModelIndex& _index) {
  if (!_index.isValid()) {
    return;
  }

  Q_EMIT payToSignal(_index);
}

void AddressBookFrame::currentAddressChanged(const QModelIndex& _index) {
  m_ui->m_copyAddressButton->setEnabled(_index.isValid());
  m_ui->m_deleteAddressButton->setEnabled(_index.isValid());
  m_ui->m_editAddressButton->setEnabled(_index.isValid());
  m_ui->m_copyPaymentIdButton->setEnabled(!_index.data(AddressBookModel::ROLE_PAYMENTID).toString().isEmpty());
}

bool AddressBookFrame::isValidPaymentId(const QByteArray& _paymentIdString) {
  if (_paymentIdString.isEmpty()) {
    return true;
  }

  QByteArray paymentId = QByteArray::fromHex(_paymentIdString);
  return (paymentId.size() == sizeof(Crypto::Hash)) && (_paymentIdString.toUpper() == paymentId.toHex().toUpper());
}

}
