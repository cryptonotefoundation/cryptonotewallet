// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "DepositDetailsDialog.h"
#include "CurrencyAdapter.h"
#include "DepositModel.h"

#include "ui_depositdetailsdialog.h"

namespace WalletGui {

DepositDetailsDialog::DepositDetailsDialog(const QModelIndex &_index, QWidget* _parent) : QDialog(_parent),
  m_ui(new Ui::DepositDetailsDialog), m_detailsTemplate(
  "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
  "</style></head><body style=\" font-family:'Cantarell'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
  "<span style=\" font-weight:600;\">Deposit info: </span></p><br>\n"
  "<span style=\" font-weight:600;\">Status: </span>%1</p><br>\n"
  "<span style=\" font-weight:600;\">Amount: </span>%2</p><br>\n"
  "<span style=\" font-weight:600;\">Interest: </span>%3</p><br>\n"
  "<span style=\" font-weight:600;\">Sum: </span>%4</p><br>\n"
  "<span style=\" font-weight:600;\">Year interest rate: </span>%5</p><br>\n"
  "<span style=\" font-weight:600;\">Term: </span>%6</p><br>\n"
  "<span style=\" font-weight:600;\">Unlock height: </span>%7</p><br>\n"
  "<span style=\" font-weight:600;\">Expected unlock time: </span>%8</p><br>\n"
  "<span style=\" font-weight:600;\">Creating transaction: </span>%9</p><br>\n"
  "<span style=\" font-weight:600;\">Creating height: </span>%10</p><br>\n"
  "<span style=\" font-weight:600;\">Creating time: </span>%11</p><br>\n"
  "<span style=\" font-weight:600;\">Spending transaction: </span>%12</p><br>\n"
  "<span style=\" font-weight:600;\">Spending height: </span>%13</p><br>\n"
  "<span style=\" font-weight:600;\">Spending time: </span>%14</p><br>\n"
  "</body></html>") {
  m_ui->setupUi(this);

  QModelIndex depositIndex = DepositModel::instance().index(_index.data(DepositModel::ROLE_ROW).toUInt(), 0);
  QString depositAmount = depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_AMOUNT).data().toString() + " " +
    CurrencyAdapter::instance().getCurrencyTicker().toUpper();
  QString depositInterest = depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_INTEREST).data().toString() + " " +
    CurrencyAdapter::instance().getCurrencyTicker().toUpper();
  QString depositSum = depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_SUM).data().toString() + " " +
    CurrencyAdapter::instance().getCurrencyTicker().toUpper();
  QString depositInfo = m_detailsTemplate.
    arg(depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_STATE).data().toString()).
    arg(depositAmount).arg(depositInterest).arg(depositSum).
    arg(depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_YEAR_RATE).data().toString()).
    arg(depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_TERM).data().toString()).
    arg(depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_UNLOCK_HEIGHT).data().toString()).
    arg(depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_UNLOCK_TIME).data().toString()).
    arg(depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_CREATRING_TRANSACTION_HASH).data().toString()).
    arg(depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_CREATING_HEIGHT).data().toString()).
    arg(depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_CREATING_TIME).data().toString()).
    arg(depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_SPENDING_TRANSACTION_HASH).data().toString()).
    arg(depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_SPENDING_HEIGHT).data().toString()).
    arg(depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_SPENDING_TIME).data().toString());
  m_ui->m_detailsBrowser->setHtml(depositInfo);
}

DepositDetailsDialog::~DepositDetailsDialog() {
}

}

