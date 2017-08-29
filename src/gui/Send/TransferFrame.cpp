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

#include <QClipboard>
#include <QCompleter>
#include <QWheelEvent>

#include "TransferFrame.h"
#include "Settings/Settings.h"
#include "Gui/Common/AddressBookDialog.h"
#include "ICryptoNoteAdapter.h"
#include "Models/AddressBookModel.h"
#include "Style/Style.h"

#include "ui_TransferFrame.h"

namespace WalletGui {

namespace {

const char TRANSFER_FRAME_STYLE_SHEET_TEMPLATE[] =
  "WalletGui--TransferFrame {"
    "background-color: %backgroundColorGray%;"
    "border: none;"
    "border-top: 1px solid %borderColor%;"
  "}"

  "WalletGui--TransferFrame[hideTopBorder=\"true\"] {"
    "border-top: none"
  "}"

  "WalletGui--TransferFrame #m_addressBookButton {"
    "margin-top: 1px;"
  "}"

  "WalletGui--TransferFrame #m_sendAmountSpin {"
    "min-width: 230px;"
    "max-width: 230px;"
  "}";

}

TransferFrame::TransferFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::TransferFrame),
  m_cryptoNoteAdapter(nullptr), m_mainWindow(nullptr), m_addressBookModel(nullptr), m_addressCompleter(new QCompleter(this)) {
  m_ui->setupUi(this);
  m_ui->m_sendAmountSpin->installEventFilter(this);
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(TRANSFER_FRAME_STYLE_SHEET_TEMPLATE));
}

TransferFrame::~TransferFrame() {
}

bool TransferFrame::readyToSend() const {
  QString address = getAddress();
  return !address.isEmpty() && m_cryptoNoteAdapter->isValidAddress(address);
}

QString TransferFrame::getAddress() const {
  return m_ui->m_sendAddressEdit->text().trimmed();
}

QString TransferFrame::getAmountString() const {
  return m_ui->m_sendAmountSpin->cleanText();
}

QString TransferFrame::getLabel() const {
  return m_ui->m_sendLabelEdit->text().trimmed();
}

void TransferFrame::setAddress(const QString& _address) {
  m_ui->m_sendAddressEdit->setText(_address);
}

void TransferFrame::setAmount(qreal _amount) {
  m_ui->m_sendAmountSpin->setValue(_amount);
}

void TransferFrame::setLabel(const QString& _label) {
  m_ui->m_sendLabelEdit->setText(_label);
}

void TransferFrame::disableRemoveButton(bool _disable) {
  m_ui->m_removeButton->setHidden(_disable);
}

void TransferFrame::hideBorder() {
  setProperty("hideTopBorder", true);
  style()->unpolish(this);
  style()->polish(this);
}

void TransferFrame::setAddressError(bool _error) {
  m_ui->m_payToTextLabel->setProperty("errorState", _error);
  m_ui->m_sendAddressEdit->setProperty("errorState", _error);
  if (_error) {
    m_ui->m_payToTextLabel->setText(tr("INVALID ADDRESS"));
  } else {
    m_ui->m_payToTextLabel->setText(tr("PAY TO"));
  }

  m_ui->m_sendAddressEdit->style()->unpolish(m_ui->m_sendAddressEdit);
  m_ui->m_sendAddressEdit->style()->polish(m_ui->m_sendAddressEdit);
  m_ui->m_sendAddressEdit->update();

  m_ui->m_payToTextLabel->style()->unpolish(m_ui->m_payToTextLabel);
  m_ui->m_payToTextLabel->style()->polish(m_ui->m_payToTextLabel);
  m_ui->m_payToTextLabel->update();
}

void TransferFrame::setDuplicationError(bool _error) {
  m_ui->m_sendLabelEdit->setProperty("errorState", _error);
  m_ui->m_labelTextLabel->setProperty("errorState", _error);
  m_ui->m_labelTextLabel->setText(_error ? tr("ADDRESS OR LABEL ALREADY EXISTS IN THE ADDRESS BOOK AND WILL NOT BE ADDED") : tr("LABEL"));

  m_ui->m_sendLabelEdit->style()->unpolish(m_ui->m_sendLabelEdit);
  m_ui->m_sendLabelEdit->style()->polish(m_ui->m_sendLabelEdit);
  m_ui->m_sendLabelEdit->update();

  m_ui->m_labelTextLabel->style()->unpolish(m_ui->m_labelTextLabel);
  m_ui->m_labelTextLabel->style()->polish(m_ui->m_labelTextLabel);
  m_ui->m_labelTextLabel->update();
}

void TransferFrame::setAmountFormatError(bool _error) {
  m_ui->m_amountTextLabel->setProperty("errorState", _error);
  m_ui->m_sendAmountSpin->setProperty("errorState", _error);
  if (_error) {
    m_ui->m_amountTextLabel->setText(tr("WRONG AMOUNT"));
    connect(m_ui->m_sendAmountSpin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,
      &TransferFrame::validateAmount, Qt::UniqueConnection);
  } else {
    m_ui->m_amountTextLabel->setText(tr("AMOUNT"));
  }

  m_ui->m_sendAmountSpin->style()->unpolish(m_ui->m_sendAmountSpin);
  m_ui->m_sendAmountSpin->style()->polish(m_ui->m_sendAmountSpin);
  m_ui->m_sendAmountSpin->update();

  m_ui->m_amountTextLabel->style()->unpolish(m_ui->m_amountTextLabel);
  m_ui->m_amountTextLabel->style()->polish(m_ui->m_amountTextLabel);
  m_ui->m_amountTextLabel->update();
}

void TransferFrame::setInsufficientFundsError(bool _error) {
  m_ui->m_amountTextLabel->setProperty("errorState", _error);
  m_ui->m_sendAmountSpin->setProperty("errorState", _error);
  if (_error) {
    m_ui->m_amountTextLabel->setText(tr("INSUFFICIENT FUNDS"));
    connect(m_ui->m_sendAmountSpin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,
      &TransferFrame::validateAmount, Qt::UniqueConnection);
  } else {
    m_ui->m_amountTextLabel->setText(tr("AMOUNT"));
  }

  m_ui->m_sendAmountSpin->style()->unpolish(m_ui->m_sendAmountSpin);
  m_ui->m_sendAmountSpin->style()->polish(m_ui->m_sendAmountSpin);
  m_ui->m_sendAmountSpin->update();

  m_ui->m_amountTextLabel->style()->unpolish(m_ui->m_amountTextLabel);
  m_ui->m_amountTextLabel->style()->polish(m_ui->m_amountTextLabel);
  m_ui->m_amountTextLabel->update();
}

void TransferFrame::setBigTransactionError(bool _error) {
  m_ui->m_amountTextLabel->setProperty("errorState", _error);
  m_ui->m_sendAmountSpin->setProperty("errorState", _error);
  if (_error) {
    m_ui->m_amountTextLabel->setText(tr("TRANSACTION IS TOO BIG"));
    connect(m_ui->m_sendAmountSpin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,
      &TransferFrame::validateAmount, Qt::UniqueConnection);
  } else {
    m_ui->m_amountTextLabel->setText(tr("AMOUNT"));
  }

  m_ui->m_sendAmountSpin->style()->unpolish(m_ui->m_sendAmountSpin);
  m_ui->m_sendAmountSpin->style()->polish(m_ui->m_sendAmountSpin);
  m_ui->m_sendAmountSpin->update();

  m_ui->m_amountTextLabel->style()->unpolish(m_ui->m_amountTextLabel);
  m_ui->m_amountTextLabel->style()->polish(m_ui->m_amountTextLabel);
  m_ui->m_amountTextLabel->update();
}

void TransferFrame::setLabelLikePaymentIdError(bool _error) {
  m_ui->m_sendLabelEdit->setProperty("errorState", _error);
  m_ui->m_labelTextLabel->setProperty("errorState", _error);
  m_ui->m_labelTextLabel->setText(_error ? tr("NOTE: THIS IS NOT A PAYMENT ID FIELD") : tr("LABEL"));

  m_ui->m_sendLabelEdit->style()->unpolish(m_ui->m_sendLabelEdit);
  m_ui->m_sendLabelEdit->style()->polish(m_ui->m_sendLabelEdit);
  m_ui->m_sendLabelEdit->update();

  m_ui->m_labelTextLabel->style()->unpolish(m_ui->m_labelTextLabel);
  m_ui->m_labelTextLabel->style()->polish(m_ui->m_labelTextLabel);
  m_ui->m_labelTextLabel->update();
}

bool TransferFrame::eventFilter(QObject* _object, QEvent* _event) {
  if (_object == m_ui->m_sendAmountSpin) {
    if (_event->type() == QEvent::Wheel && !m_ui->m_sendAmountSpin->hasFocus()) {
      wheelEvent(static_cast<QWheelEvent*>(_event));
      return true;
    }
  }

  return false;
}

void TransferFrame::setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) {
  m_cryptoNoteAdapter = _cryptoNoteAdapter;
  m_ui->m_sendAmountSpin->setSuffix(" " + m_cryptoNoteAdapter->getCurrencyTicker().toUpper());
}

void TransferFrame::setAddressBookManager(IAddressBookManager* _addressBookManager) {
  m_addressBookManager = _addressBookManager;
}

void TransferFrame::setMainWindow(QWidget* _mainWindow) {
  m_mainWindow = _mainWindow;
}

void TransferFrame::setAddressBookModel(QAbstractItemModel* _model) {
  m_addressBookModel = _model;
  m_addressCompleter->setModel(m_addressBookModel);
  m_addressCompleter->setCompletionColumn(AddressBookModel::COLUMN_LABEL);
  m_addressCompleter->setCompletionRole(AddressBookModel::ROLE_LABEL);
  m_addressCompleter->setCaseSensitivity(Qt::CaseInsensitive);
  m_ui->m_sendAddressEdit->setCompleter(m_addressCompleter);

  connect(m_addressCompleter, static_cast<void(QCompleter::*)(const QModelIndex&)>(&QCompleter::activated), this, [&](const QModelIndex& _index) {
      m_ui->m_sendAddressEdit->setText(_index.data(AddressBookModel::ROLE_ADDRESS).toString());
  }, Qt::QueuedConnection);
}

void TransferFrame::addressBookClicked() {
  AddressBookDialog dlg(m_addressBookModel, m_mainWindow);
  if (dlg.exec() == QDialog::Accepted) {
    m_ui->m_sendAddressEdit->setText(dlg.getAddress());
  }
}

void TransferFrame::pasteClicked() {
  m_ui->m_sendAddressEdit->setText(QApplication::clipboard()->text());
}

void TransferFrame::addressChanged(const QString& _address) {
  setAddressError(m_addressCompleter->currentCompletion().isEmpty() && !_address.isEmpty() &&
    !m_cryptoNoteAdapter->isValidAddress(_address));
  Q_EMIT addressChangedSignal(_address);
}

void TransferFrame::labelOrAddressChanged(const QString& _text) {
  QString label = getLabel().trimmed();
  QString address = getAddress().trimmed();
  if (!label.isEmpty() && (m_addressBookManager->findAddressByAddress(address) != std::numeric_limits<quintptr>::max() ||
    m_addressBookManager->findAddressByLabel(label) != std::numeric_limits<quintptr>::max())) {
      setDuplicationError(true);
      return;
  } else if (m_cryptoNoteAdapter->isValidPaymentId(label)) {
    setLabelLikePaymentIdError(true);
    return;
  }

  setDuplicationError(false);
}

void TransferFrame::validateAmount(double _amount) {
  if (_amount > 0) {
    setAmountFormatError(false);
  }
}

void TransferFrame::amountStringChanged(const QString& _amountString) {
  Q_EMIT amountStringChangedSignal(m_ui->m_sendAmountSpin->cleanText());
}

}
