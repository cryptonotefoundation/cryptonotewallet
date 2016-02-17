// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "AddressBookModel.h"
#include "CurrencyAdapter.h"
#include "MainWindow.h"
#include "NodeAdapter.h"
#include "SendFrame.h"
#include "TransferFrame.h"
#include "WalletAdapter.h"
#include "WalletEvents.h"

#include "ui_sendframe.h"

namespace WalletGui {

Q_DECL_CONSTEXPR int DEFAULT_MIXIN = 2;

SendFrame::SendFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::SendFrame) {
  m_ui->setupUi(this);
  clearAllClicked();
  m_ui->m_mixinSlider->setValue(DEFAULT_MIXIN);

  connect(&WalletAdapter::instance(), &WalletAdapter::walletSendTransactionCompletedSignal, this, &SendFrame::sendTransactionCompleted,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletActualBalanceUpdatedSignal, this, &SendFrame::walletActualBalanceUpdated,
    Qt::QueuedConnection);

  m_ui->m_tickerLabel->setText(CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  m_ui->m_feeSpin->setSuffix(" " + CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  m_ui->m_feeSpin->setMinimum(CurrencyAdapter::instance().formatAmount(CurrencyAdapter::instance().getMinimumFee()).toDouble());
}

SendFrame::~SendFrame() {
}

void SendFrame::setAddress(const QString& _address) {
  Q_FOREACH (TransferFrame* transfer, m_transfers) {
    if (transfer->getAddress().isEmpty()) {
      transfer->setAddress(_address);
      return;
    }
  }

  addRecipientClicked();
  m_transfers.last()->setAddress(_address);
}

void SendFrame::addRecipientClicked() {
  TransferFrame* newTransfer = new TransferFrame(m_ui->m_transfersScrollarea);
  m_ui->m_send_frame_layout->insertWidget(m_transfers.size(), newTransfer);
  m_transfers.append(newTransfer);
  if (m_transfers.size() == 1) {
    newTransfer->disableRemoveButton(true);
  } else {
    m_transfers[0]->disableRemoveButton(false);
  }

  connect(newTransfer, &TransferFrame::destroyed, [this](QObject* _obj) {
      m_transfers.removeOne(static_cast<TransferFrame*>(_obj));
      if (m_transfers.size() == 1) {
        m_transfers[0]->disableRemoveButton(true);
      }
    });
}

void SendFrame::clearAllClicked() {
  Q_FOREACH (TransferFrame* transfer, m_transfers) {
    transfer->close();
  }

  m_transfers.clear();
  addRecipientClicked();
  m_ui->m_paymentIdEdit->clear();
  m_ui->m_mixinSlider->setValue(DEFAULT_MIXIN);
  m_ui->m_feeSpin->setValue(m_ui->m_feeSpin->minimum());
}

void SendFrame::sendClicked() {
  QVector<CryptoNote::WalletLegacyTransfer> walletTransfers;
  QVector<CryptoNote::TransactionMessage> walletMessages;
  Q_FOREACH (TransferFrame * transfer, m_transfers) {
    QString address = transfer->getAddress();
    if (!CurrencyAdapter::instance().validateAddress(address)) {
      QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Invalid recipient address"), QtCriticalMsg));
      return;
    }

    CryptoNote::WalletLegacyTransfer walletTransfer;
    walletTransfer.address = address.toStdString();
    uint64_t amount = CurrencyAdapter::instance().parseAmount(transfer->getAmountString());
    walletTransfer.amount = amount;
    walletTransfers.push_back(walletTransfer);
    QString label = transfer->getLabel();
    if (!label.isEmpty()) {
      AddressBookModel::instance().addAddress(label, address);
    }

    QString comment = transfer->getComment();
    if (!comment.isEmpty()) {
      walletMessages.append(CryptoNote::TransactionMessage{comment.toStdString(), address.toStdString()});
    }
  }

  quint64 fee = CurrencyAdapter::instance().parseAmount(m_ui->m_feeSpin->cleanText());
  if (fee < CurrencyAdapter::instance().getMinimumFee()) {
    QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Incorrect fee value"), QtCriticalMsg));
    return;
  }

  if (WalletAdapter::instance().isOpen()) {
    WalletAdapter::instance().sendTransaction(walletTransfers, fee, m_ui->m_paymentIdEdit->text(), m_ui->m_mixinSlider->value(),
      walletMessages);
  }
}

void SendFrame::mixinValueChanged(int _value) {
  m_ui->m_mixinEdit->setText(QString::number(_value));
}

void SendFrame::sendTransactionCompleted(CryptoNote::TransactionId _id, bool _error, const QString& _errorText) {
  Q_UNUSED(_id);
  if (_error) {
    QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(_errorText, QtCriticalMsg));
  } else {
    clearAllClicked();
  }
}

void SendFrame::walletActualBalanceUpdated(quint64 _balance) {
  m_ui->m_balanceLabel->setText(CurrencyAdapter::instance().formatAmount(_balance));
}
  
}
