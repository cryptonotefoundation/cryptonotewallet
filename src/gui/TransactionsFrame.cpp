// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2011-2013 The Bitcoin developers
// Copyright (c) 2016-2019 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QFileDialog>
#include <QLabel>
#include <QClipboard>
#include <QHBoxLayout>
#include <QComboBox>
#include <QDateTimeEdit>

#include "CurrencyAdapter.h"
#include "MainWindow.h"
#include "SortedTransactionsModel.h"
#include "TransactionsFrame.h"
#include "TransactionDetailsDialog.h"
#include "TransactionsListModel.h"
#include "TransactionsModel.h"
#include "WalletAdapter.h"

#include "ui_transactionsframe.h"

namespace WalletGui {

TransactionsFrame::TransactionsFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::TransactionsFrame),
  m_transactionsModel(new TransactionsListModel) {
  m_ui->setupUi(this);
  m_ui->m_transactionsView->setSortingEnabled(true);
  m_ui->m_transactionsView->sortByColumn(0, Qt::AscendingOrder);
  m_transactionsModel->setSortRole(Qt::EditRole);
  m_transactionsModel->setDynamicSortFilter(true);
  m_ui->m_transactionsView->setModel(m_transactionsModel.data());
  m_ui->m_transactionsView->header()->setSectionResizeMode(TransactionsModel::COLUMN_STATE, QHeaderView::Fixed);
  m_ui->m_transactionsView->header()->resizeSection(TransactionsModel::COLUMN_STATE, 25);
  m_ui->m_transactionsView->header()->resizeSection(TransactionsModel::COLUMN_DATE, 90);
  m_ui->m_transactionsView->header()->resizeSection(TransactionsModel::COLUMN_FEE, 45);
  m_ui->m_transactionsView->header()->resizeSection(TransactionsModel::COLUMN_ADDRESS, 100);
  m_ui->m_transactionsView->header()->resizeSection(TransactionsModel::COLUMN_PAYMENT_ID, 200);
  m_ui->m_transactionsView->header()->resizeSection(TransactionsModel::COLUMN_HASH, 200);
  m_ui->m_transactionsView->header()->resizeSection(TransactionsModel::COLUMN_SECRET_KEY, 200);

  connect(m_ui->m_transactionsView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TransactionsFrame::computeSelected);

  m_ui->m_transactionsView->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_ui->m_transactionsView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &TransactionsFrame::walletClosed);

  contextMenu = new QMenu();
  contextMenu->addAction(QString(tr("Copy transaction &hash")), this, SLOT(copyTxHash()));
  contextMenu->addAction(QString(tr("Copy Payment &ID")), this, SLOT(copyPaymentID()));
  contextMenu->addAction(QString(tr("Copy &amount")), this, SLOT(copyAmount()));
  contextMenu->addAction(QString(tr("Show &details")), this, SLOT(showTxDetails()));

  m_ui->m_selectedAmountLabel->hide();

  m_ui->m_dateSelect->addItem(tr("Unconfirmed"), Unconfirmed);
  m_ui->m_dateSelect->addItem(tr("All"), All);
  m_ui->m_dateSelect->addItem(tr("Today"), Today);
  m_ui->m_dateSelect->addItem(tr("This week"), ThisWeek);
  m_ui->m_dateSelect->addItem(tr("This month"), ThisMonth);
  m_ui->m_dateSelect->addItem(tr("Last month"), LastMonth);
  m_ui->m_dateSelect->addItem(tr("This year"), ThisYear);
  m_ui->m_dateSelect->addItem(tr("Range..."), Range);
  m_ui->m_dateSelect->setCurrentIndex(1);

  m_ui->m_typeSelect->addItem(tr("All types"), AllTypes);
  m_ui->m_typeSelect->addItem(tr("Incoming"), Incoming);
  m_ui->m_typeSelect->addItem(tr("Outgoing"), Outgoing);
  m_ui->m_typeSelect->addItem(tr("Mined"), Mining);
  m_ui->m_typeSelect->addItem(tr("Optimization"), Fusion);
  m_ui->m_typeSelect->addItem(tr("Sent to myself"), InOut);

  m_ui->m_dateRangeArea->addWidget(createDateRangeWidget());

  connect(m_ui->m_dateSelect, SIGNAL(activated(int)), this, SLOT(chooseDate(int)));
  connect(m_ui->m_typeSelect, SIGNAL(activated(int)), this, SLOT(chooseType(int)));
  connect(m_ui->m_searchFor, SIGNAL(textChanged(QString)), this, SLOT(changedSearchFor(QString)));

  // set sorting date range to include unconfirmed
  includeUnconfirmed();
  // set sorting to include all types of transactions
  SortedTransactionsModel::instance().setTxType(-1);

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
    QByteArray res;
    res.append("\"Date\",\"Amount\",\"Fee\",\"Hash\",\"Height\",\"Address\",\"Payment ID\",\"Key\"\n");
    QModelIndexList selection = m_ui->m_transactionsView->selectionModel()->selectedRows();
    if(selection.isEmpty()) {
      m_ui->m_transactionsView->selectAll();
      QModelIndexList all = m_ui->m_transactionsView->selectionModel()->selectedRows();
      foreach (QModelIndex index, all){
        res.append("\"").append(index.sibling(index.row(), TransactionsModel::COLUMN_DATE).data().toString().toUtf8()).append("\",");
        res.append("\"").append(index.sibling(index.row(), TransactionsModel::COLUMN_AMOUNT).data().toString().toUtf8()).append("\",");
        res.append("\"").append(CurrencyAdapter::instance().formatAmount(index.data(TransactionsModel::ROLE_FEE).value<quint64>())).append("\",");
        res.append("\"").append(index.sibling(index.row(), TransactionsModel::COLUMN_HASH).data().toString().toUtf8()).append("\",");
        res.append("\"").append(index.data(TransactionsModel::ROLE_HEIGHT).toString().toUtf8()).append("\",");
        res.append("\"").append(index.sibling(index.row(), TransactionsModel::COLUMN_ADDRESS).data().toString().toUtf8()).append("\",");
        res.append("\"").append(index.sibling(index.row(), TransactionsModel::COLUMN_PAYMENT_ID).data().toString().toUtf8()).append("\",");
        res.append("\"").append(index.data(TransactionsModel::ROLE_SECRET_KEY).toByteArray().toHex().toUpper()).append("\"\n");
      }
      m_ui->m_transactionsView->clearSelection();
    } else {
      foreach (QModelIndex index, selection){
        res.append("\"").append(index.sibling(index.row(), TransactionsModel::COLUMN_DATE).data().toString().toUtf8()).append("\",");
        res.append("\"").append(index.sibling(index.row(), TransactionsModel::COLUMN_AMOUNT).data().toString().toUtf8()).append("\",");
        res.append("\"").append(CurrencyAdapter::instance().formatAmount(index.data(TransactionsModel::ROLE_FEE).value<quint64>())).append("\",");
        res.append("\"").append(index.sibling(index.row(), TransactionsModel::COLUMN_HASH).data().toString().toUtf8()).append("\",");
        res.append("\"").append(index.data(TransactionsModel::ROLE_HEIGHT).toString().toUtf8()).append("\",");
        res.append("\"").append(index.sibling(index.row(), TransactionsModel::COLUMN_ADDRESS).data().toString().toUtf8()).append("\",");
        res.append("\"").append(index.sibling(index.row(), TransactionsModel::COLUMN_PAYMENT_ID).data().toString().toUtf8()).append("\",");
        res.append("\"").append(index.data(TransactionsModel::ROLE_SECRET_KEY).toByteArray().toHex().toUpper()).append("\"\n");
      }
    }

    QFile f(file);
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
      f.write(res);
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
    m_ui->m_selectedAmount->show();
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

QWidget *TransactionsFrame::createDateRangeWidget()
{
    dateRangeWidget = new QFrame();
    dateRangeWidget->setFrameStyle(QFrame::Panel | QFrame::Plain);
    dateRangeWidget->setStyleSheet("QFrame { border: 0; max-width: 240px; }");
    dateRangeWidget->setContentsMargins(1,1,1,1);
    QHBoxLayout *layout = new QHBoxLayout(dateRangeWidget);
    layout->setContentsMargins(0,0,0,0);
    layout->addSpacing(12);
    layout->addWidget(new QLabel(tr("From")));

    dateFrom = new QDateTimeEdit(this);
    dateFrom->setDisplayFormat("dd/MM/yy");
    dateFrom->setCalendarPopup(true);
    dateFrom->setMinimumWidth(80);
    dateFrom->setDate(QDate::currentDate().addDays(-7));
    layout->addWidget(dateFrom);
    layout->addWidget(new QLabel(tr("to")));

    dateTo = new QDateTimeEdit(this);
    dateTo->setDisplayFormat("dd/MM/yy");
    dateTo->setCalendarPopup(true);
    dateTo->setMinimumWidth(80);
    dateTo->setDate(QDate::currentDate());
    layout->addWidget(dateTo);
    layout->addStretch();

    // Hide by default
    dateRangeWidget->setVisible(false);

    // Notify on change
    connect(dateFrom, SIGNAL(dateChanged(QDate)), this, SLOT(dateRangeChanged()));
    connect(dateTo, SIGNAL(dateChanged(QDate)), this, SLOT(dateRangeChanged()));

    return dateRangeWidget;
}

void TransactionsFrame::dateRangeChanged()
{
    if(!m_transactionsModel)
        return;
    SortedTransactionsModel::instance().setDateRange(
            QDateTime(dateFrom->date()),
            QDateTime(dateTo->date()).addDays(1));
}

void TransactionsFrame::chooseDate(int idx)
{
    if(!m_transactionsModel)
        return;
    QDate current = QDate::currentDate();
    dateRangeWidget->setVisible(false);
    switch(m_ui->m_dateSelect->itemData(idx).toInt())
    {
    case Unconfirmed:
        SortedTransactionsModel::instance().setDateRange(
                QDateTime(),
                QDateTime());
        break;

    case All:
        SortedTransactionsModel::instance().setDateRange(
                QDateTime(),
                SortedTransactionsModel::MAX_DATE);
        break;
    case Today:
        SortedTransactionsModel::instance().setDateRange(
                QDateTime(current),
                SortedTransactionsModel::MAX_DATE);
        break;
    case ThisWeek: {
        // Find last Monday
        QDate startOfWeek = current.addDays(-(current.dayOfWeek()-1));
        SortedTransactionsModel::instance().setDateRange(
                QDateTime(startOfWeek),
                SortedTransactionsModel::MAX_DATE);

        } break;
    case ThisMonth:
        SortedTransactionsModel::instance().setDateRange(
                QDateTime(QDate(current.year(), current.month(), 1)),
                SortedTransactionsModel::MAX_DATE);
        break;
    case LastMonth:
        if (current.month() == 1){
            SortedTransactionsModel::instance().setDateRange(
                QDateTime(QDate(current.year()-1, 12, 1)),
                QDateTime(QDate(current.year(), current.month(), 1)));
        }
        else {
            SortedTransactionsModel::instance().setDateRange(
                QDateTime(QDate(current.year(), current.month()-1, 1)),
                QDateTime(QDate(current.year(), current.month(), 1)));
        }
        break;
    case ThisYear:
        SortedTransactionsModel::instance().setDateRange(
                QDateTime(QDate(current.year(), 1, 1)),
                SortedTransactionsModel::MAX_DATE);
        break;
    case Range:
        dateRangeWidget->setVisible(true);
        dateRangeChanged();
        break;
    }
}

void TransactionsFrame::chooseType(int idx)
{
    if(!m_transactionsModel)
        return;

    switch(m_ui->m_typeSelect->itemData(idx).toInt())
    {
    case AllTypes:
        SortedTransactionsModel::instance().setTxType(-1);
        break;
    case Incoming:
        SortedTransactionsModel::instance().setTxType(1);
        break;
    case Outgoing: {
        SortedTransactionsModel::instance().setTxType(2);
        } break;
    case Mining:
        SortedTransactionsModel::instance().setTxType(0);
        break;
    case Fusion:
        SortedTransactionsModel::instance().setTxType(4);
        break;
    case InOut:
        SortedTransactionsModel::instance().setTxType(3);
        break;
    }
}

void TransactionsFrame::changedSearchFor(const QString &searchstring)
{
  if(!m_transactionsModel)
     return;
  SortedTransactionsModel::instance().setSearchFor(searchstring);
}

void TransactionsFrame::resetFilterClicked() {
  m_ui->m_searchFor->clear();
  m_ui->m_dateSelect->setCurrentIndex(1);
  m_ui->m_typeSelect->setCurrentIndex(0);
  SortedTransactionsModel::instance().setTxType(-1);
  m_ui->m_transactionsView->sortByColumn(0, Qt::AscendingOrder);
  dateRangeWidget->setVisible(false);
  includeUnconfirmed();
  m_ui->m_transactionsView->clearSelection();
}

void TransactionsFrame::includeUnconfirmed() {
  SortedTransactionsModel::instance().setDateRange(
  QDateTime(),
  SortedTransactionsModel::MAX_DATE);
}

void TransactionsFrame::walletClosed() {
  m_ui->m_selectedAmount->setText("");
  m_ui->m_selectedAmount->hide();
  m_ui->m_selectedAmountLabel->hide();
}

void TransactionsFrame::reloadTransactions() {
  resetFilterClicked();
}

}
