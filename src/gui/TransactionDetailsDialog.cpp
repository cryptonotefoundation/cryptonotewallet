// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDateTime>

#include "CurrencyAdapter.h"
#include "TransactionDetailsDialog.h"
#include "TransactionsModel.h"

#include "ui_transactiondetailsdialog.h"

namespace WalletGui {

TransactionDetailsDialog::TransactionDetailsDialog(const QModelIndex& _index, QWidget* _parent) : QDialog(_parent),
  m_ui(new Ui::TransactionDetailsDialog), m_detailsTemplate(
    "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
    "</style></head><body style=\" font-family:'Cantarell'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
    "<span style=\" font-weight:600;\">Status: </span>%1</p><br>\n"
    "<span style=\" font-weight:600;\">Date: </span>%2</p><br>\n"
    "<span style=\" font-weight:600;\">To: </span>%4</p><br>\n"
    "<span style=\" font-weight:600;\">Amount: </span>%5</p><br>\n"
    "<span style=\" font-weight:600;\">Fee: </span>%6</p><br>\n"
    "<span style=\" font-weight:600;\">Transaction hash: </span>%8</p></body></html>") {
  m_ui->setupUi(this);

  QModelIndex index = TransactionsModel::instance().index(_index.data(TransactionsModel::ROLE_ROW).toInt(),
    _index.data(TransactionsModel::ROLE_ROW).toInt());

  quint64 numberOfConfirmations = index.data(TransactionsModel::ROLE_NUMBER_OF_CONFIRMATIONS).value<quint64>();
  QString amountText = index.sibling(index.row(), TransactionsModel::COLUMN_AMOUNT).data().toString() + " " +
    CurrencyAdapter::instance().getCurrencyTicker().toUpper();
  QString feeText = CurrencyAdapter::instance().formatAmount(index.data(TransactionsModel::ROLE_FEE).value<quint64>()) + " " +
    CurrencyAdapter::instance().getCurrencyTicker().toUpper();

  m_ui->m_detailsBrowser->setHtml(m_detailsTemplate.arg(QString("%1 confirmations").arg(numberOfConfirmations)).
    arg(index.sibling(index.row(), TransactionsModel::COLUMN_DATE).data().toString()).arg(index.sibling(index.row(),
    TransactionsModel::COLUMN_ADDRESS).data().toString()).arg(amountText).arg(feeText).
    arg(index.sibling(index.row(), TransactionsModel::COLUMN_HASH).data().toString()));
}

TransactionDetailsDialog::~TransactionDetailsDialog() {
}

}
