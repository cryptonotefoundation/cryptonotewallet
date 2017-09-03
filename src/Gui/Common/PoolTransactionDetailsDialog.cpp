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

#include <QApplication>
#include <QClipboard>
#include <QDataWidgetMapper>

#include "PoolTransactionDetailsDialog.h"
#include "Settings/Settings.h"
#include "Models/TransactionPoolModel.h"
#include "Style/Style.h"
#include "ui_PoolTransactionDetailsDialog.h"

namespace WalletGui {

namespace {

const char POOL_TRANSACTION_DETAILS_DIALOG_STYLE_SHEET_TEMPLATE[] =
  "* {"
    "font-family: %fontFamily%;"
  "}"

  "WalletGui--PoolTransactionDetailsDialog {"
    "background-color: #ffffff;"
  "}"

  "WalletGui--PoolTransactionDetailsDialog WalletGui--WalletNormalGrayTextLabel, "
  "WalletGui--PoolTransactionDetailsDialog WalletGui--WalletNormalBlackTextLabel {"
    "min-height: 41px;"
    "max-height: 41px;"
    "padding-left: 30px;"
    "padding-right: 30px;"
    "border: none;"
  "}"

  "WalletGui--PoolTransactionDetailsDialog #m_textLabel,"
  "WalletGui--PoolTransactionDetailsDialog #m_hashLabel {"
    "padding-left: 0px;"
    "padding-right: 0px;"
  "}"

  "WalletGui--PoolTransactionDetailsDialog #m_amountTextLabel,"
  "WalletGui--PoolTransactionDetailsDialog #m_amountLabel,"
  "WalletGui--PoolTransactionDetailsDialog #m_sizeTextLabel,"
  "WalletGui--PoolTransactionDetailsDialog #m_sizeLabel,"
  "WalletGui--PoolTransactionDetailsDialog #m_paymentIdTextLabel,"
  "WalletGui--PoolTransactionDetailsDialog #m_paymentIdLabel {"
    "background-color: %backgroundColorAlternate%;"
  "}";

}

PoolTransactionDetailsDialog::PoolTransactionDetailsDialog(QAbstractItemModel* _transactionPoolModel, const QModelIndex& _index, QWidget* _parent) : QDialog(_parent, static_cast<Qt::WindowFlags>(Qt::WindowCloseButtonHint)),
  m_ui(new Ui::PoolTransactionDetailsDialog), m_index(_index) {
  m_ui->setupUi(this);

  QDataWidgetMapper* mapper = new QDataWidgetMapper(this);

  mapper->setModel(_transactionPoolModel);
  mapper->addMapping(m_ui->m_hashLabel, TransactionPoolModel::COLUMN_HASH, "text");
  mapper->addMapping(m_ui->m_amountLabel, TransactionPoolModel::COLUMN_AMOUNT, "text");
  mapper->addMapping(m_ui->m_feeLabel, TransactionPoolModel::COLUMN_FEE, "text");
  mapper->addMapping(m_ui->m_sizeLabel, TransactionPoolModel::COLUMN_SIZE, "text");
  mapper->addMapping(m_ui->m_mixinLabel, TransactionPoolModel::COLUMN_MIXIN, "text");
  mapper->addMapping(m_ui->m_paymentIdLabel, TransactionPoolModel::COLUMN_PAYMENT_ID, "text");
  mapper->setCurrentModelIndex(m_index);
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(POOL_TRANSACTION_DETAILS_DIALOG_STYLE_SHEET_TEMPLATE));
  m_ui->m_hashLabel->installEventFilter(this);
}

PoolTransactionDetailsDialog::~PoolTransactionDetailsDialog() {
}

void PoolTransactionDetailsDialog::showEvent(QShowEvent* _event) {
  QDialog::showEvent(_event);
  adjustSize();
  setFixedWidth(width());
}

bool WalletGui::PoolTransactionDetailsDialog::eventFilter(QObject* _object, QEvent* _event) {
  if (_object == m_ui->m_hashLabel && _event->type() == QEvent::MouseButtonRelease) {
    QApplication::clipboard()->setText(m_ui->m_hashLabel->text());
     m_ui->m_copyLabel->start();
  }

  return QDialog::eventFilter(_object, _event);
}

}
