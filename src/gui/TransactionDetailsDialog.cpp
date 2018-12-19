// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDateTime>
#include "crypto/crypto.h"
#include "CurrencyAdapter.h"
#include "TransactionDetailsDialog.h"
#include "TransactionsModel.h"
#include <IWalletLegacy.h>
#include "WalletAdapter.h"
#include "Common/StringTools.h"
#include "CryptoNoteCore/CryptoNoteBasic.h"
#include "TransactionsModel.h"

#include "ui_transactiondetailsdialog.h"

using namespace CryptoNote;

namespace WalletGui {

TransactionDetailsDialog::TransactionDetailsDialog(const QModelIndex& _index, QWidget* _parent) : QDialog(_parent),
  m_ui(new Ui::TransactionDetailsDialog), m_detailsTemplate(tr(
    "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
    "</style></head><body style=\" font-family:'Cantarell'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
    "<p><span style=\" font-weight:600;\">Status: </span>%1<br>\n"
    "<span style=\" font-weight:600;\">Date: </span>%2<br>\n"
    "<span style=\" font-weight:600;\">To: </span>%4<br>\n"
    "<span style=\" font-weight:600;\">Amount: </span>%5<br>\n"
    "<span style=\" font-weight:600;\">Fee: </span>%6<br>\n"
    "<span style=\" font-weight:600;\">Payment ID: </span>%7<br>\n"
    "<span style=\" font-weight:600;\">Transaction Hash: </span>%8<br>\n"
    "<span style=\" font-weight:600;\">Transaction Key: </span>%9<br>\n"
    "<span style=\" font-weight:600;\">Transaction Proof: </span>%10\n</p></body></html>")) {
  m_ui->setupUi(this);

  QModelIndex index = TransactionsModel::instance().index(_index.data(TransactionsModel::ROLE_ROW).toInt(),
    _index.data(TransactionsModel::ROLE_ROW).toInt());

  quint64 numberOfConfirmations = index.data(TransactionsModel::ROLE_NUMBER_OF_CONFIRMATIONS).value<quint64>();
  QString amountText = index.sibling(index.row(), TransactionsModel::COLUMN_AMOUNT).data().toString() + " " +
    CurrencyAdapter::instance().getCurrencyTicker().toUpper();
  QString feeText = CurrencyAdapter::instance().formatAmount(index.data(TransactionsModel::ROLE_FEE).value<quint64>()) + " " +
    CurrencyAdapter::instance().getCurrencyTicker().toUpper();

  QString state;
  CryptoNote::WalletLegacyTransaction transaction;
  CryptoNote::TransactionId transaction_id = index.row();
  if(WalletAdapter::instance().getTransaction(transaction_id, transaction)) {
     if(transaction.state == CryptoNote::WalletLegacyTransactionState::Failed)
        state = tr("Failed");

     else if(transaction.state == CryptoNote::WalletLegacyTransactionState::Cancelled)
        state = tr("Cancelled");
  }
  if(state.isEmpty())
    state = QString(tr("%n confirmation(s)", "", numberOfConfirmations));

  Crypto::Hash tx_hash;
  size_t size;
  QString transactionHash = index.sibling(index.row(), TransactionsModel::COLUMN_HASH).data().toString();
  std::string tx_hash_str = transactionHash.toStdString();
  Common::fromHex(tx_hash_str, &tx_hash, sizeof(tx_hash), size);
  Crypto::SecretKey tx_key = WalletAdapter::instance().getTxKey(tx_hash);
  QString transactionKey;
  QString transactionProof;
  TransactionType transactionType = static_cast<TransactionType>(index.data(TransactionsModel::ROLE_TYPE).value<quint8>());
  if (tx_key != NULL_SECRET_KEY && transactionType == TransactionType::OUTPUT) {
    transactionKey = QString::fromStdString(Common::podToHex(tx_key)).toUpper();
    transactionProof = WalletAdapter::instance().getTxProof(tx_hash, CurrencyAdapter::instance().internalAddress(index.sibling(index.row(),                                                                                  TransactionsModel::COLUMN_ADDRESS).data().toString()), tx_key);
  } else {
    transactionKey = QString(tr("(n/a)"));
    transactionProof = QString(tr("(n/a)"));
  }

  m_ui->m_detailsBrowser->setHtml(m_detailsTemplate.arg(state).
    arg(index.sibling(index.row(), TransactionsModel::COLUMN_DATE).data().toString()).arg(index.sibling(index.row(),
    TransactionsModel::COLUMN_ADDRESS).data().toString()).arg(amountText).arg(feeText).
    arg(index.sibling(index.row(), TransactionsModel::COLUMN_PAYMENT_ID).data().toString()).
    arg(transactionHash).arg(transactionKey).arg(transactionProof));
}

TransactionDetailsDialog::~TransactionDetailsDialog() {
}

}
