// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QFileDialog>
#include <QLabel>
#include <QClipboard>

#include "CurrencyAdapter.h"
#include "MainWindow.h"
#include "SortedTransactionsModel.h"
#include "TransactionsFrame.h"
#include "TransactionDetailsDialog.h"
#include "TransactionsListModel.h"
#include "TransactionsModel.h"

#include "ui_transactionsframe.h"

namespace WalletGui {

TransactionsFrame::TransactionsFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::TransactionsFrame),
  m_transactionsModel(new TransactionsListModel) {
  m_ui->setupUi(this);
  m_ui->m_transactionsView->setModel(m_transactionsModel.data());
  m_ui->m_transactionsView->header()->setSectionResizeMode(TransactionsModel::COLUMN_STATE, QHeaderView::Fixed);
  m_ui->m_transactionsView->header()->resizeSection(TransactionsModel::COLUMN_STATE, 25);
  m_ui->m_transactionsView->header()->resizeSection(TransactionsModel::COLUMN_DATE, 90);
  m_ui->m_transactionsView->header()->resizeSection(TransactionsModel::COLUMN_ADDRESS, 100);
  m_ui->m_transactionsView->header()->resizeSection(TransactionsModel::COLUMN_PAYMENT_ID, 200);
  m_ui->m_transactionsView->header()->resizeSection(TransactionsModel::COLUMN_HASH, 200);
  // m_ui->m_transactionsView->header()->setSectionResizeMode(TransactionsModel::COLUMN_HASH, QHeaderView::Stretch);

  connect(m_ui->m_transactionsView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TransactionsFrame::computeSelected);

  m_ui->m_transactionsView->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_ui->m_transactionsView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

  contextMenu = new QMenu();
  contextMenu->addAction(QString(tr("Copy transaction &hash")), this, SLOT(copyTxHash()));
  contextMenu->addAction(QString(tr("Copy Payment &ID")), this, SLOT(copyPaymentID()));
  contextMenu->addAction(QString(tr("Copy &amount")), this, SLOT(copyAmount()));
  contextMenu->addAction(QString(tr("Show &details")), this, SLOT(showTxDetails()));

  m_ui->m_selectedAmountLabel->hide();
}

TransactionsFrame::~TransactionsFrame() {
}

void TransactionsFrame::scrollToTransaction(const QModelIndex& _index) {
  QModelIndex sortedModelIndex = SortedTransactionsModel::instance().mapFromSource(_index);
  QModelIndex index = static_cast<QSortFilterProxyModel*>(m_ui->m_transactionsView->model())->mapFromSource(sortedModelIndex);
  m_ui->m_transactionsView->scrollTo(index);
  m_ui->m_transactionsView->setFocus();
  m_ui->m_transactionsView->setCurrentIndex(index);
}

void TransactionsFrame::exportToCsv() {
  QString file = QFileDialog::getSaveFileName(&MainWindow::instance(), tr("Select CSV file"), QDir::homePath(), "CSV (*.csv)");
  if (!file.isEmpty()) {
    QByteArray csv = TransactionsModel::instance().toCsv();
    QFile f(file);
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
      f.write(csv);
      f.close();
    }
  }
}

void TransactionsFrame::showTransactionDetails(const QModelIndex& _index) {
  if (!_index.isValid()) {
    return;
  }

  TransactionDetailsDialog dlg(_index, &MainWindow::instance());
  dlg.exec();
}

void TransactionsFrame::onCustomContextMenu(const QPoint &point)
{
  index = m_ui->m_transactionsView->indexAt(point);
  contextMenu->exec(m_ui->m_transactionsView->mapToGlobal(point));
}

void TransactionsFrame::copyTxHash(){
  QApplication::clipboard()->setText(index.sibling(index.row(), TransactionsModel::COLUMN_HASH).data().toString());
}

void TransactionsFrame::copyAmount(){
  QApplication::clipboard()->setText(index.sibling(index.row(), TransactionsModel::COLUMN_AMOUNT).data().toString());
}

void TransactionsFrame::copyPaymentID(){
  QApplication::clipboard()->setText(index.sibling(index.row(), TransactionsModel::COLUMN_PAYMENT_ID).data().toString());
}
void TransactionsFrame::showTxDetails(){
  showTransactionDetails(index);
}

void TransactionsFrame::computeSelected() {
  double amount = 0;
  if(!m_ui->m_transactionsView->selectionModel())
    return;

    QModelIndexList selection = m_ui->m_transactionsView->selectionModel()->selectedRows();

    foreach (QModelIndex index, selection){
        QString amountstring = index.sibling(index.row(), TransactionsModel::COLUMN_AMOUNT).data().toString().remove(',');
        amount += amountstring.toDouble();
    }
    QString amountText = QString::number(amount, 'f', 12) + " " + CurrencyAdapter::instance().getCurrencyTicker().toUpper();
    if (amount < 0) amountText = "<span style='color:red;'>" + amountText + "</span>";
    m_ui->m_selectedAmount->setText(amountText);
    m_ui->m_selectedAmountLabel->show();
}

QString TransactionsFrame::formatAmount(int64_t _amount) const {
  QString s =CurrencyAdapter::instance().formatAmount(static_cast<uint64_t>(std::abs(_amount)));
    if (_amount < 0) {
      s.insert(0, "-");
    }
  return s;
}

}
