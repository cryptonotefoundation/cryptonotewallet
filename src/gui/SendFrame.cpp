// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2017 The befrank developers
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
#include <QRegExpValidator>
#include <QInputDialog>
#include <QMessageBox>
#include <QUrlQuery>
#include <QTime>
#include <QUrl>
#include "ui_sendframe.h"
#include "Settings.h"
#include "AddressProvider.h"
#include "OpenUriDialog.h"
#include "ConfirmSendDialog.h"

namespace WalletGui {

SendFrame::SendFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::SendFrame), m_addressProvider(new AddressProvider(this)) {
  m_ui->setupUi(this);
  clearAllClicked();
  mixinValueChanged(m_ui->m_mixinSlider->value());
  remote_node_fee = 0;

  connect(&WalletAdapter::instance(), &WalletAdapter::walletSendTransactionCompletedSignal, this, &SendFrame::sendTransactionCompleted,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletActualBalanceUpdatedSignal, this, &SendFrame::walletActualBalanceUpdated,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &SendFrame::reset);

  m_ui->m_tickerLabel->setText(CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  m_ui->m_feeSpin->setSuffix(" " + CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  m_ui->m_donateSpin->setSuffix(" " + CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  m_ui->m_feeSpin->setMinimum(CurrencyAdapter::instance().formatAmount(CurrencyAdapter::instance().getMinimumFee()).toDouble());
  m_ui->m_remote_label->hide();

  QRegExp hexMatcher("^[0-9A-F]{64}$", Qt::CaseInsensitive);
  QValidator *validator = new QRegExpValidator(hexMatcher, this);
  m_ui->m_paymentIdEdit->setValidator(validator);

  QString connection = Settings::instance().getConnection();
  if(connection.compare("remote") == 0) {
    QString remoteNodeUrl = Settings::instance().getCurrentRemoteNode() + "/feeaddress";
    m_addressProvider->getAddress(remoteNodeUrl);
    connect(m_addressProvider, &AddressProvider::addressFoundSignal, this, &SendFrame::onAddressFound, Qt::QueuedConnection);
  }

}

SendFrame::~SendFrame() {
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

  connect(newTransfer, &TransferFrame::amountValueChangedSignal, this, &SendFrame::amountValueChange, Qt::QueuedConnection);
  connect(newTransfer, &TransferFrame::insertPaymentIDSignal, this, &SendFrame::insertPaymentID, Qt::QueuedConnection);

}

void SendFrame::clearAllClicked() {
  Q_FOREACH (TransferFrame* transfer, m_transfers) {
    transfer->close();
  }
  m_transfers.clear();
  addRecipientClicked();
  amountValueChange();
  m_ui->m_paymentIdEdit->clear();
  m_ui->m_mixinSlider->setValue(5);
  m_ui->m_feeSpin->setValue(m_ui->m_feeSpin->minimum());
}

void SendFrame::reset() {
  amountValueChange();
}

void SendFrame::amountValueChange() {
    QVector<quint64> amounts;
    Q_FOREACH (TransferFrame * transfer, m_transfers) {
      quint64 amount = CurrencyAdapter::instance().parseAmount(transfer->getAmountString());
      amounts.push_back(amount);
      }
    total_amount = 0;
    for(QVector<quint64>::iterator it = amounts.begin(); it != amounts.end(); ++it) {
      total_amount += *it;
    }

    QVector<quint64> fees;
    fees.clear();
    Q_FOREACH (TransferFrame * transfer, m_transfers) {
      quint64 amount = CurrencyAdapter::instance().parseAmount(transfer->getAmountString());
      quint64 percentfee = amount * 0.25 / 100; // fee is 0.25%
      fees.push_back(percentfee);
      }
    remote_node_fee = 0;
    if( !remote_node_fee_address.isEmpty() ) {
        for(QVector<quint64>::iterator it = fees.begin(); it != fees.end(); ++it) {
            remote_node_fee += *it;
        }
        if (remote_node_fee < CurrencyAdapter::instance().getMinimumFee()) {
            remote_node_fee = CurrencyAdapter::instance().getMinimumFee();
        }
        if (remote_node_fee > 1000) {
            remote_node_fee = 1000;
        }
    }

    QVector<float> donations;
    donations.clear();
    Q_FOREACH (TransferFrame * transfer, m_transfers) {
      float amount = transfer->getAmountString().toFloat();
      float donationpercent = amount * 0.1 / 100; // donation is 0.1%
      donations.push_back(donationpercent);
      }
    float donation_amount = 0;
    for(QVector<float>::iterator it = donations.begin(); it != donations.end(); ++it) {
        donation_amount += *it;
    }
    float min = CurrencyAdapter::instance().formatAmount(CurrencyAdapter::instance().getMinimumFee()).toFloat();
    if (donation_amount < min) {
        donation_amount = min;
    }
    donation_amount = floor(donation_amount * pow(10., 4) + .5) / pow(10., 4);
    m_ui->m_donateSpin->setValue(QString::number(donation_amount).toDouble());

    if( !remote_node_fee_address.isEmpty() ) {
        quint64 actualBalance = WalletAdapter::instance().getActualBalance();
        if(actualBalance > remote_node_fee) {
            m_ui->m_balanceLabel->setText(CurrencyAdapter::instance().formatAmount(actualBalance - remote_node_fee));
        } else {
            m_ui->m_balanceLabel->setText(CurrencyAdapter::instance().formatAmount(actualBalance));
        }
    }
}

void SendFrame::insertPaymentID(QString _paymentid) {
    m_ui->m_paymentIdEdit->setText(_paymentid);
}

void SendFrame::onAddressFound(const QString& _address) {
    SendFrame::remote_node_fee_address = _address;
    m_ui->m_remote_label->show();
}

void SendFrame::openUriClicked() {
    OpenUriDialog dlg(&MainWindow::instance());
    if (dlg.exec() == QDialog::Accepted) {
        QString uri = dlg.getURI();
        if (uri.isEmpty()) {
          return;
        }
        SendFrame::parsePaymentRequest(uri);
        Q_EMIT uriOpenSignal();
    }
}

void SendFrame::parsePaymentRequest(QString _request) {
    if(_request.startsWith("befrank://", Qt::CaseInsensitive))
    {
       _request.replace(0, 13, "befrank:");
    }
    if(!_request.startsWith("befrank:", Qt::CaseInsensitive)) {
      QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Payment request should start with befrank:"), QtCriticalMsg));
      return;
    }

    if(_request.startsWith("befrank:", Qt::CaseInsensitive))
    {
      _request.remove(0, 11);
    }

    QString address = _request.split("?").at(0);

    if (!CurrencyAdapter::instance().validateAddress(address)) {
      QCoreApplication::postEvent(
        &MainWindow::instance(),
        new ShowMessageEvent(tr("Invalid recipient address"), QtCriticalMsg));
      return;
    }
    m_transfers.at(0)->TransferFrame::setAddress(address);

    _request.replace("?", "&");

    QUrlQuery uriQuery(_request);

    quint64 amount = CurrencyAdapter::instance().parseAmount(uriQuery.queryItemValue("amount"));
    if(amount != 0){
        m_transfers.at(0)->TransferFrame::setAmount(amount);
    }

    QString label = uriQuery.queryItemValue("label");
    if(!label.isEmpty()){
        m_transfers.at(0)->TransferFrame::setLabel(label);
    }

    QString payment_id = uriQuery.queryItemValue("payment_id");
    if(!payment_id.isEmpty()){
        SendFrame::insertPaymentID(payment_id);
    }

}

void SendFrame::sendClicked() {
 ConfirmSendDialog dlg(&MainWindow::instance());
    dlg.showPasymentDetails(total_amount);
    if(!m_ui->m_paymentIdEdit->text().isEmpty()){
      dlg.showPaymentId(m_ui->m_paymentIdEdit->text());
    } else {
      dlg.confirmNoPaymentId();
    }
    if (dlg.exec() == QDialog::Accepted) {

      QVector<CryptoNote::WalletLegacyTransfer> walletTransfers;
      Q_FOREACH (TransferFrame * transfer, m_transfers) {
        QString address = transfer->getAddress();
        if (!CurrencyAdapter::instance().validateAddress(address)) {
          QCoreApplication::postEvent(
            &MainWindow::instance(),
            new ShowMessageEvent(tr("Invalid recipient address"), QtCriticalMsg));
          return;
        }

        CryptoNote::WalletLegacyTransfer walletTransfer;
        walletTransfer.address = address.toStdString();
        uint64_t amount = CurrencyAdapter::instance().parseAmount(transfer->getAmountString());
        walletTransfer.amount = amount;
        walletTransfers.push_back(walletTransfer);
        QString label = transfer->getLabel();
        if (!label.isEmpty()) {
          AddressBookModel::instance().addAddress(label, address, m_ui->m_paymentIdEdit->text().toUtf8());
        }
      }

      // Dev donation
      if (m_ui->donateCheckBox->isChecked()) {
          CryptoNote::WalletLegacyTransfer walletTransfer;
          walletTransfer.address = "FSoYHF6m6Fudfdkyrj6pfARM7inBfMX4SMHtiZFg3u9d346NpJgYN3N1eiRNB1B3bc2x811J9EWy9HxfuWpjojv5FaY9not";
          walletTransfer.amount = CurrencyAdapter::instance().parseAmount(m_ui->m_donateSpin->cleanText());
          walletTransfers.push_back(walletTransfer);
      }

      // Remote node fee
      QString connection = Settings::instance().getConnection();
      if(connection.compare("remote") == 0) {
          if (!SendFrame::remote_node_fee_address.isEmpty()) {
            CryptoNote::WalletLegacyTransfer walletTransfer;
            walletTransfer.address = SendFrame::remote_node_fee_address.toStdString();
            walletTransfer.amount = remote_node_fee;
            walletTransfers.push_back(walletTransfer);
          }
      }

      // Miners fee
      quint64 fee = CurrencyAdapter::instance().parseAmount(m_ui->m_feeSpin->cleanText());
      if (fee < CurrencyAdapter::instance().getMinimumFee()) {
        QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Incorrect fee value"), QtCriticalMsg));
        return;
      }

      quint64 total_transaction_amount = 0;
      for (size_t i = 0; i < walletTransfers.size(); i++){
        total_transaction_amount += walletTransfers.at(i).amount;
      }
      if (total_transaction_amount > (WalletAdapter::instance().getActualBalance() - fee)) {
        QMessageBox::critical(this, tr("Insufficient balance"), tr("Available balance is insufficient to send this transaction. Have you excluded a fee?"), QMessageBox::Ok);
        return;
      }

      if (WalletAdapter::instance().isOpen()) {
          QByteArray paymentIdString = m_ui->m_paymentIdEdit->text().toUtf8();
          if (!isValidPaymentId(paymentIdString)) {
            QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Invalid payment ID"), QtCriticalMsg));
            return;
          }

          WalletAdapter::instance().sendTransaction(walletTransfers, fee, m_ui->m_paymentIdEdit->text(), m_ui->m_mixinSlider->value());
      }
  }
}

void SendFrame::mixinValueChanged(int _value) {
  m_ui->m_mixinEdit->setText(QString::number(_value));
}

void SendFrame::sendTransactionCompleted(CryptoNote::TransactionId _id, bool _error, const QString& _errorText) {
  Q_UNUSED(_id);
  if (_error) {
    QCoreApplication::postEvent(
      &MainWindow::instance(),
      new ShowMessageEvent(_errorText, QtCriticalMsg));
  } else {
    clearAllClicked();
  }
}

void SendFrame::walletActualBalanceUpdated(quint64 _balance) {
  if(!remote_node_fee_address.isEmpty() && _balance > remote_node_fee) {
    m_ui->m_balanceLabel->setText(CurrencyAdapter::instance().formatAmount(_balance - remote_node_fee));
  } else {
    m_ui->m_balanceLabel->setText(CurrencyAdapter::instance().formatAmount(_balance));
  }
}

bool SendFrame::isValidPaymentId(const QByteArray& _paymentIdString) {
  if (_paymentIdString.isEmpty()) {
    return true;
  }

  QByteArray paymentId = QByteArray::fromHex(_paymentIdString);
  return (paymentId.size() == sizeof(Crypto::Hash)) && (_paymentIdString.toUpper() == paymentId.toHex().toUpper());
}

void SendFrame::generatePaymentIdClicked() {
  SendFrame::insertPaymentID(CurrencyAdapter::instance().generatePaymentId());
}

}
