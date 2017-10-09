// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Intensecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Intensecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Intensecoin.  If not, see <http://www.gnu.org/licenses/>.

#include <QMetaMethod>
#include <QUrl>
#include <QUrlQuery>

#include <Wallet/WalletErrors.h>

#include "SendFrame.h"
#include "Settings/Settings.h"
#include "Gui/Common/QuestionDialog.h"
#include "ICryptoNoteAdapter.h"
#include "IDonationManager.h"
#include "INodeAdapter.h"
#include "IWallet.h"
#include "Models/AddressBookModel.h"
#include "Models/WalletStateModel.h"
#include "SendGlassFrame.h"
#include "Style/Style.h"
#include "TransferFrame.h"

#include "ui_SendFrame.h"

namespace WalletGui {

namespace {

const char SEND_FRAME_STYLE_SHEET[] =
  "WalletGui--SendFrame {"
    "background-color: #ffffff;"
    "border: none;"
  "}"

  "WalletGui--SendFrame #m_sendScrollarea {"
    "background-color: %backgroundColorGray%;"
    "border: none;"
    "border-bottom: 1px solid %borderColor%;"
  "}"

  "WalletGui--SendFrame #m_sendScrollarea QScrollBar:vertical {"
    "margin-top: 0px;"
    "border-top: none;"
  "}"

  "WalletGui--SendFrame #scrollAreaWidgetContents {"
    "background-color: #ffffff;"
    "border: none;"
  "}"

  "WalletGui--SendFrame #m_sendFeeFrame {"
    "border: none;"
    "border-top: 1px solid %borderColor%;"
    "border-bottom: 1px solid %borderColor%;"
  "}";

const quint64 MAXIMUM_UNSYNCED_BLOCKS_WHEN_SEND_AVAILABLE = 5;
const quint64 DEFAULT_MIXIN_VALUE = 6;
const quint64 MAX_MIXIN_VALUE = 1000;
const quint64 CRITICAL_MIXIN_BOUND = 3;
const quint64 NORMAL_MIXIN_BOUND = 6;
const char PAYMENT_URL_AMOUNT_TAG[] = "amount";
const char PAYMENT_URL_PAYMENT_ID_TAG[] = "payment_id";
const char PAYMENT_URL_MESSAGE_TAG[] = "message";
const char PAYMENT_URL_LABEL_TAG[] = "label";
const char MIXIN_CRITICAL_COLOR[] = "#ea161f";
const char MIXIN_NORMAL_COLOR[] = "#fdce00";
const char MIXIN_GOOD_COLOR[] = "#6ca025";
const char MIXIN_SLIDER_STYLE_SHEET_TEMPLATE[] =
  "QSlider::sub-page:horizontal {"
      "background-color: %COLOR%;"
      "border: 1px solid %COLOR%;"
  "}";

bool isValidPaymentId(const QString& _paymentIdString) {
  if (!_paymentIdString.trimmed().isEmpty() &&
    (_paymentIdString.trimmed().size() != sizeof(Crypto::Hash) * 2 ||
    QByteArray::fromHex(_paymentIdString.toLatin1()).size() != sizeof(Crypto::Hash) ||
    QByteArray::fromHex(_paymentIdString.toLatin1()).toHex().toLower() != _paymentIdString.trimmed().toLower())) {
    return false;
  }

  return true;
}

}

SendFrame::SendFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::SendFrame),
  m_cryptoNoteAdapter(nullptr), m_donationManager(nullptr), m_applicationEventHandler(nullptr), m_mainWindow(nullptr),
  m_glassFrame(new SendGlassFrame(nullptr)), m_walletStateModel(nullptr) {
  m_ui->setupUi(this);
  m_glassFrame->setObjectName("m_sendGlassFrame");
  m_ui->m_mixinSpin->setMaximum(MAX_MIXIN_VALUE);
  mixinValueChanged(m_ui->m_mixinSlider->value());
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(SEND_FRAME_STYLE_SHEET));
}

SendFrame::~SendFrame() {
  m_transfers.clear();
  m_glassFrame->deleteLater();
}

void SendFrame::addRecipient(const QString& _address) {
  if (m_transfers.size() == 1 && m_transfers[0]->getAddress().isEmpty()) {
    m_transfers[0]->setAddress(_address);
  } else {
    addRecipientClicked();
    m_ui->m_sendScrollarea->widget()->updateGeometry();
    m_ui->m_sendScrollarea->updateGeometry();
    m_transfers.last()->setAddress(_address);
  }
}

void SendFrame::setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) {
  m_cryptoNoteAdapter = _cryptoNoteAdapter;
  m_cryptoNoteAdapter->addObserver(this);
  m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
  m_ui->m_feeSpin->setSuffix(" " + m_cryptoNoteAdapter->getCurrencyTicker().toUpper());

  for (auto& transfer : m_transfers) {
    transfer->setCryptoNoteAdapter(_cryptoNoteAdapter);
  }

  amountStringChanged(QString());
}

void SendFrame::setApplicationEventHandler(IApplicationEventHandler* _applicationEventHandler) {
  m_applicationEventHandler = _applicationEventHandler;
  m_applicationEventHandler->addObserver(this);
}

void SendFrame::setAddressBookManager(IAddressBookManager* _addressBookManager) {
  m_addressBookManager = _addressBookManager;

  for (auto& transfer : m_transfers) {
    transfer->setAddressBookManager(m_addressBookManager);
  }
}

void SendFrame::setDonationManager(IDonationManager* _donationManager) {
  m_donationManager = _donationManager;
}

void SendFrame::setMainWindow(QWidget* _mainWindow) {
  m_mainWindow = _mainWindow;
  for (auto& transfer : m_transfers) {
    transfer->setMainWindow(_mainWindow);
  }

  QList<QPushButton*> buttonList = m_mainWindow->findChildren<QPushButton*>("m_transactionsButton");
  Q_ASSERT(!buttonList.isEmpty());
  connect(this, &SendFrame::showTransactionsFrameSignal, buttonList.first(), &QPushButton::click);
}

void SendFrame::setWalletStateModel(QAbstractItemModel* _model) {
  m_walletStateModel = _model;
  for (auto& transfer : m_transfers) {
    transfer->setWalletStateModel(_model);
  }
}

void SendFrame::setAddressBookModel(QAbstractItemModel* _model) {
  m_addressBookModel = _model;
  for (auto& transfer : m_transfers) {
    transfer->setAddressBookModel(_model);
  }
}

void SendFrame::walletOpened() {
  addRecipientClicked();
  QUrl url = m_applicationEventHandler->getLastReceivedUrl();
  if (url.isValid()) {
    urlReceived(url);
  }
}

void SendFrame::walletOpenError(int _initStatus) {
  // Do nothing
}

void SendFrame::walletClosed() {
  clearAll();
}

void SendFrame::passwordChanged() {
  // Do nothing
}

void SendFrame::synchronizationProgressUpdated(quint32 _current, quint32 _total) {
  if (_total - _current > MAXIMUM_UNSYNCED_BLOCKS_WHEN_SEND_AVAILABLE) {
    m_glassFrame->install(this);
    m_glassFrame->updateSynchronizationState(_current, _total);
  }
}

void SendFrame::synchronizationCompleted() {
  m_glassFrame->remove();
}

void SendFrame::balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) {
  // Do nothing
}

void SendFrame::externalTransactionCreated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void SendFrame::transactionUpdated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void SendFrame::urlReceived(const QUrl& _url) {
  if (!_url.isValid()) {
    return;
  }

  QUrlQuery urlQuery(_url);
  QString address = _url.path();
  qreal amount = QLocale(QLocale::English).toDouble(m_cryptoNoteAdapter->formatUnsignedAmount(urlQuery.queryItemValue(PAYMENT_URL_AMOUNT_TAG).toULongLong()));
  QString paymentId = urlQuery.queryItemValue(PAYMENT_URL_PAYMENT_ID_TAG);
  QString message = urlQuery.queryItemValue(PAYMENT_URL_MESSAGE_TAG);
  Q_UNUSED(message)
  QString label = urlQuery.queryItemValue(PAYMENT_URL_LABEL_TAG);
  clearAll();
  addRecipientClicked();
  m_transfers[0]->setAddress(address);
  m_transfers[0]->setAmount(amount);
  m_transfers[0]->setLabel(label);
  m_ui->m_paymentIdEdit->setText(paymentId);
}

void SendFrame::screenLocked() {
  // Do nothing
}

void SendFrame::screenUnlocked() {
  // Do nothing
}

void SendFrame::cryptoNoteAdapterInitCompleted(int _status) {
  if (_status == 0) {
    m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
    for (auto& transfer : m_transfers) {
      transfer->setCryptoNoteAdapter(m_cryptoNoteAdapter);
    }
  }
}

void SendFrame::cryptoNoteAdapterDeinitCompleted() {
  // Do nothing
}


void SendFrame::addRecipientClicked() {
  TransferFrame* newTransfer = new TransferFrame(m_ui->m_sendScrollarea->widget());
  newTransfer->setCryptoNoteAdapter(m_cryptoNoteAdapter);
  newTransfer->setAddressBookManager(m_addressBookManager);
  newTransfer->setMainWindow(m_mainWindow);
  newTransfer->setWalletStateModel(m_walletStateModel);
  newTransfer->setAddressBookModel(m_addressBookModel);
  m_ui->m_transfersLayout->insertWidget(m_transfers.size(), newTransfer);
  m_transfers.append(newTransfer);
  m_transfers.first()->hideBorder();
  if (m_transfers.size() == 1) {
    newTransfer->disableRemoveButton(true);
  } else{
    m_transfers[0]->disableRemoveButton(false);
  }

  connect(newTransfer, &TransferFrame::amountStringChangedSignal, this, &SendFrame::amountStringChanged);
  connect(newTransfer, &TransferFrame::addressChangedSignal, this, &SendFrame::addressChanged);
  connect(newTransfer, &TransferFrame::destroyed, [this, newTransfer](QObject* _obj) {
    if (m_transfers.isEmpty()) {
      return;
    }

    m_transfers.removeOne(static_cast<TransferFrame*>(_obj));

    if (m_transfers.size() == 1) {
      m_transfers[0]->disableRemoveButton(true);
    }

    m_transfers.first()->hideBorder();
    m_ui->m_transfersLayout->removeWidget(newTransfer);
    m_ui->m_sendScrollarea->widget()->updateGeometry();
    m_ui->m_sendScrollarea->updateGeometry();
    amountStringChanged(QString());
    m_ui->m_sendButton->setEnabled(readyToSend());
  });

  m_ui->m_sendScrollarea->widget()->adjustSize();
  m_ui->m_sendScrollarea->widget()->updateGeometry();
  m_ui->m_sendScrollarea->updateGeometry();
  m_ui->m_sendButton->setEnabled(false);
}

void SendFrame::clearAll() {
  for (TransferFrame* transfer : m_transfers) {
    m_ui->m_transfersLayout->removeWidget(transfer);
    transfer->close();
    transfer->deleteLater();
  }

  QApplication::processEvents();
  m_transfers.clear();
  m_ui->m_feeSpin->setValue(m_ui->m_feeSpin->minimum());
  m_ui->m_paymentIdEdit->clear();
  m_ui->m_mixinSlider->setValue(DEFAULT_MIXIN_VALUE);
  m_ui->m_sendScrollarea->widget()->adjustSize();
  m_ui->m_sendScrollarea->widget()->updateGeometry();
  m_ui->m_sendScrollarea->updateGeometry();
}


void SendFrame::sendClicked() {
  CryptoNote::TransactionParameters transactionParameters;
  QString selfAddress = m_walletStateModel->index(0, WalletStateModel::COLUMN_ADDRESS).data().toString();
  transactionParameters.sourceAddresses.push_back(selfAddress.toStdString());
  transactionParameters.changeDestination = selfAddress.toStdString();

  const quint64 actualBalance = m_walletStateModel->index(0, 0).data(WalletStateModel::ROLE_ACTUAL_BALANCE).value<quint64>();
  quint64 transferSum = 0;
  const quint64 fee = m_cryptoNoteAdapter->parseAmount(m_ui->m_feeSpin->cleanText());
  for (TransferFrame* transfer : m_transfers) {
    QString address = transfer->getAddress();
    if (!m_cryptoNoteAdapter->isValidAddress(address)) {
      transfer->setAddressError();
      m_ui->m_sendScrollarea->ensureWidgetVisible(transfer);
      return;
    }

    CryptoNote::WalletOrder walletTransfer;
    walletTransfer.address = address.toStdString();
    qint64 amount = m_cryptoNoteAdapter->parseAmount(transfer->getAmountString());
    if (amount <= 0) {
      transfer->setAmountFormatError();
      m_ui->m_sendScrollarea->ensureWidgetVisible(transfer);
      return;
    }

    transferSum += amount;
    if (transferSum + fee > actualBalance) {
      transfer->setInsufficientFundsError();
      m_ui->m_sendScrollarea->ensureWidgetVisible(transfer);
      return;
    }

    walletTransfer.amount = amount;
    transactionParameters.destinations.push_back(walletTransfer);
    QString label = transfer->getLabel();
    if (!label.isEmpty() && m_addressBookManager->findAddressByAddress(address) == INVALID_ADDRESS_INDEX &&
      m_addressBookManager->findAddressByLabel(label) == INVALID_ADDRESS_INDEX) {
      m_addressBookManager->addAddress(label, address, false);
    }
  }

  if (fee < m_cryptoNoteAdapter->getMinimalFee()) {
    return;
  }

  transactionParameters.fee = fee;

  QuestionDialog dlg(tr("Attention"), tr("Transactions with high anonymity level can take several minutes and even\nmore to be generated. "
    "Such transactions will likely be too large to be\ncurrently accepted by the network.\nPlease confirm you wish to generate this transaction."),
    m_mainWindow);
  dlg.setMinimumHeight(130);
  int mixin = m_ui->m_mixinSpin->value();
  if (mixin > 6 && dlg.exec() == QDialog::Rejected) {
    return;
  }

  transactionParameters.mixIn = mixin;
  transactionParameters.extra = m_cryptoNoteAdapter->convertPaymentToExtra(m_ui->m_paymentIdEdit->text());

  bool donationChangeIsEnabled = m_donationManager->isDonationChangeEnabled();
  QString donationAddress = m_donationManager->getDonationChangeAddress();
  if (donationChangeIsEnabled && !donationAddress.isEmpty()) {
    quint32 donationAmount = m_donationManager->getDonationChangeAmount();
    qreal donationCoeff = donationAmount / 100. / 10.;
    transactionParameters.donation.address = donationAddress.toStdString();
    transactionParameters.donation.threshold = transferSum * donationCoeff;
  }

  IWalletAdapter::SendTransactionStatus status = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->sendTransaction(transactionParameters);
  processTranactionSendStatus(status);
}

void SendFrame::mixinValueChanged(int _value) {
  if (m_ui->m_mixinSlider->value() != _value) {
    m_ui->m_mixinSlider->setValue(_value);
  }

  if (m_ui->m_mixinSpin->value() != _value) {
    m_ui->m_mixinSpin->setValue(_value);
  }

  setMixinError(false);
  updateSliderStyleSheet();
}

void SendFrame::validatePaymentId(const QString& _paymentId) {
  bool validPaymentId = isValidPaymentId(_paymentId);
  if (!validPaymentId) {
    setPaymentIdError(true);
    m_ui->m_sendButton->setEnabled(false);
    return;
  }

  setPaymentIdError(false);
  m_ui->m_sendButton->setEnabled(readyToSend());
}

void SendFrame::processTranactionSendStatus(IWalletAdapter::SendTransactionStatus _status) {
  switch (_status) {
  case 0:
    clearAll();
    addRecipientClicked();
    Q_EMIT showTransactionsFrameSignal();
    return;

  case IWalletAdapter::SEND_BIG_TRANSACTION_SIZE:
    for (TransferFrame* transfer : m_transfers) {
      transfer->setBigTransactionError();
    }

    break;

  case IWalletAdapter::SEND_WRONG_AMOUNT:
    for (TransferFrame* transfer : m_transfers) {
      transfer->setAmountFormatError();
    }

    break;

  case IWalletAdapter::SEND_BIG_MIXIN:
    setMixinError(true);
    break;

  default:
    for (TransferFrame* transfer : m_transfers) {
      transfer->setAmountFormatError();
    }

    break;
  }
}

void SendFrame::setPaymentIdError(bool _error) {
  m_ui->m_paymentIdEdit->setProperty("errorState", _error);
  m_ui->m_paymentIdTextLabel->setProperty("errorState", _error);
  if (_error) {
    m_ui->m_paymentIdTextLabel->setText(tr("INVALID FORMAT"));
  } else{
    m_ui->m_paymentIdTextLabel->setText(tr("PAYMENT ID"));
  }

  m_ui->m_paymentIdEdit->style()->unpolish(m_ui->m_paymentIdEdit);
  m_ui->m_paymentIdEdit->style()->polish(m_ui->m_paymentIdEdit);

  m_ui->m_paymentIdTextLabel->style()->unpolish(m_ui->m_paymentIdTextLabel);
  m_ui->m_paymentIdTextLabel->style()->polish(m_ui->m_paymentIdTextLabel);
  m_ui->m_paymentIdTextLabel->update();
}

void SendFrame::setMixinError(bool _error) {
  m_ui->m_mixinTextLabel->setProperty("errorState", _error);
  if (_error) {
    m_ui->m_mixinTextLabel->setText(tr("MIXIN IS TOO BIG"));
  } else{
    m_ui->m_mixinTextLabel->setText(tr("ANONYMITY LEVEL"));
  }

  m_ui->m_mixinTextLabel->style()->unpolish(m_ui->m_mixinTextLabel);
  m_ui->m_mixinTextLabel->style()->polish(m_ui->m_mixinTextLabel);
  m_ui->m_mixinTextLabel->update();
}

void SendFrame::updateSliderStyleSheet() {
  int mixinValue = m_ui->m_mixinSpin->value();
  QString color = MIXIN_GOOD_COLOR;
  if (mixinValue < CRITICAL_MIXIN_BOUND) {
    color = MIXIN_CRITICAL_COLOR;
  } else if (mixinValue < NORMAL_MIXIN_BOUND) {
    color = MIXIN_NORMAL_COLOR;
  }

  QString mixinSliderStyleSheet = QString(MIXIN_SLIDER_STYLE_SHEET_TEMPLATE).replace("%COLOR%", color);
  m_ui->m_mixinSlider->setStyleSheet(mixinSliderStyleSheet);
}

void SendFrame::amountStringChanged(const QString& _amountString) {
  quint64 totalSendAmount = 0;
  for (TransferFrame* transfer : m_transfers) {
    quint64 amount = m_cryptoNoteAdapter->parseUnsignedAmount(transfer->getAmountString());
    totalSendAmount += amount;
  }

  m_ui->m_totalAmountLabel->setText(QString("%1 %2").arg(m_cryptoNoteAdapter->formatUnsignedAmount(totalSendAmount)).
  arg(m_cryptoNoteAdapter->getCurrencyTicker().toUpper()));
}

void SendFrame::addressChanged(const QString& _address) {
  m_ui->m_sendButton->setEnabled(readyToSend());
}

bool SendFrame::readyToSend() const {
  bool readyToSend = true;
  for (const auto& transfer : m_transfers) {
    readyToSend = readyToSend && transfer->readyToSend();
  }

  readyToSend = readyToSend && isValidPaymentId(m_ui->m_paymentIdEdit->text());
  return readyToSend;
}

}
