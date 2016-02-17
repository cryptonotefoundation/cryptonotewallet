// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDateTime>

#include "CurrencyAdapter.h"
#include "DepositModel.h"
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
    "<span style=\" font-weight:600;\">Transaction hash: </span>%8</p><br><br>\n"
    "<span style=\" font-weight:600;\">Messages: </span></p><br>%9<br><br>\n"
    "%10"
    "</body></html>") {
  m_ui->setupUi(this);

  QModelIndex transactionIndex = TransactionsModel::instance().index(_index.data(TransactionsModel::ROLE_ROW).toInt(),
    _index.data(TransactionsModel::ROLE_ROW).toInt());

  quint64 numberOfConfirmations = transactionIndex.data(TransactionsModel::ROLE_NUMBER_OF_CONFIRMATIONS).value<quint64>();
  QString amountText = transactionIndex.sibling(transactionIndex.row(), TransactionsModel::COLUMN_AMOUNT).data().toString() + " " +
    CurrencyAdapter::instance().getCurrencyTicker().toUpper();
  QString feeText = CurrencyAdapter::instance().formatAmount(transactionIndex.data(TransactionsModel::ROLE_FEE).value<quint64>()) + " " +
    CurrencyAdapter::instance().getCurrencyTicker().toUpper();
  QStringList messageList = _index.data(TransactionsModel::ROLE_MESSAGES).toStringList();

  for (quint32 i = 0; i < messageList.size(); ++i) {
    messageList[i] = messageList[i].toHtmlEscaped().replace("\n", "<br/>");
  }

  CryptoNote::DepositId depositId = transactionIndex.data(TransactionsModel::ROLE_DEPOSIT_ID).value<CryptoNote::DepositId>();

  QString depositInfo;
  if (depositId != CryptoNote::WALLET_LEGACY_INVALID_DEPOSIT_ID) {
    QModelIndex depositIndex = DepositModel::instance().index(depositId, 0);
    QString depositAmount = depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_AMOUNT).data().toString() + " " +
      CurrencyAdapter::instance().getCurrencyTicker().toUpper();
    QString depositInterest = depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_INTEREST).data().toString() + " " +
      CurrencyAdapter::instance().getCurrencyTicker().toUpper();
    QString depositSum = depositIndex.sibling(depositIndex.row(), DepositModel::COLUMN_SUM).data().toString() + " " +
      CurrencyAdapter::instance().getCurrencyTicker().toUpper();
    QString depositInfoTemplate =
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
      "<span style=\" font-weight:600;\">Spending time: </span>%14</p><br>\n";
      depositInfo = depositInfoTemplate.
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
  }

  m_ui->m_detailsBrowser->setHtml(m_detailsTemplate.arg(QString("%1 confirmations").arg(numberOfConfirmations)).
    arg(transactionIndex.sibling(transactionIndex.row(), TransactionsModel::COLUMN_DATE).data().toString()).arg(transactionIndex.sibling(transactionIndex.row(),
    TransactionsModel::COLUMN_ADDRESS).data().toString()).arg(amountText).arg(feeText).
    arg(transactionIndex.sibling(transactionIndex.row(), TransactionsModel::COLUMN_HASH).data().toString()).
    arg(messageList.join("<br/><br/>=========<br/><br/>")).
    arg(depositInfo));
}

TransactionDetailsDialog::~TransactionDetailsDialog() {
}

}
