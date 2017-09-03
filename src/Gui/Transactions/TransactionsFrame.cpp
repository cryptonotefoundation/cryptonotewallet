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

#include <QFileDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>

#include "TransactionsFrame.h"
#include "Settings/Settings.h"
#include "Gui/Common/RightAlignmentColumnDelegate.h"
#include "Gui/Common/NewTransactionDelegate.h"
#include "Gui/Common/TransactionsAmountDelegate.h"
#include "Gui/Common/TransactionDetailsDialog.h"
#include "Gui/Common/TransactionsHeaderView.h"
#include "Gui/Common/TransactionsTimeDelegate.h"
#include "FilteredByAddressTransactionsModel.h"
#include "FilteredByAgeTransactionsModel.h"
#include "FilteredByHashTransactionsModel.h"
#include "FilteredByPeriodTransactionsModel.h"
#include "FilteredTransactionsModel.h"
#include "Models/TransactionsModel.h"
#include "Style/Style.h"
#include "TransactionsDelegate.h"

#include "ui_TransactionsFrame.h"

namespace WalletGui {

namespace {

const char TRANSACTIONS_FRAME_STYLE_SHEET_TEMPLATE[] =
  "WalletGui--TransactionsFrame {"
    "background-color: #ffffff;"
    "border: none;"
  "}"

  "WalletGui--TransactionsFrame #m_resetFilterButton,"
  "WalletGui--TransactionsFrame #m_filterButton {"
    "margin-top: 2px;"
  "}"

  "WalletGui--TransactionsFrame > #m_filterFrame > #m_filterPeriodComboFrame > #m_filterCombo {"
    "min-width: 100px;"
    "max-width: 100px;"
  "}"

  "WalletGui--TransactionsFrame > #m_filterFrame > #m_filterHashFrame > QLineEdit,"
  "WalletGui--TransactionsFrame > #m_filterFrame > #m_filterAddressFrame > QLineEdit {"
    "font-size: %fontSizeNormal%;"
  "}";

const quint32 FILTER_FRAME_HEIGHT = 70;

}

TransactionsFrame::TransactionsFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::TransactionsFrame),
  m_mainWindow(nullptr), m_transactionsModel(nullptr), m_walletStateModel(nullptr), m_filterByAgeModel(nullptr),
  m_filterByPeriodModel(nullptr), m_filterByHashModel(nullptr), m_filterByAddressModel(nullptr), m_animation(new QPropertyAnimation(this)) {
  m_ui->setupUi(this);
  m_ui->m_filterFrame->hide();
  m_ui->m_filterPeriodFrame->hide();
  m_animation->setTargetObject(m_ui->m_filterFrame);
  m_animation->setPropertyName("maximumHeight");
  m_animation->setDuration(200);
  m_ui->m_resetFilterButton->hide();
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(TRANSACTIONS_FRAME_STYLE_SHEET_TEMPLATE));
  connect(m_animation, &QPropertyAnimation::finished, this, [this] {
    m_ui->m_filterButton->setEnabled(true);
  });
}

TransactionsFrame::~TransactionsFrame() {
}

void TransactionsFrame::setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) {
  m_cryptoNoteAdapter = _cryptoNoteAdapter;
}

void TransactionsFrame::setAddressBookManager(IAddressBookManager* _addressBookManager) {
  m_addressBookManager = _addressBookManager;
}

void TransactionsFrame::setMainWindow(QWidget* _mainWindow) {
  m_mainWindow = _mainWindow;
}

void TransactionsFrame::setWalletStateModel(QAbstractItemModel* _model) {
  m_walletStateModel = _model;
}

void TransactionsFrame::setTransactionsModel(QAbstractItemModel* _model) {
  m_transactionsModel = _model;
}

void TransactionsFrame::setSortedTransactionsModel(QAbstractItemModel* _model) {
  FilteredByAgeTransactionsModel* filterByAgeModel = new FilteredByAgeTransactionsModel(this);
  FilteredByPeriodTransactionsModel* filterByPeriodModel = new FilteredByPeriodTransactionsModel(this);
  FilteredByHashTransactionsModel* filteredByHashModel = new FilteredByHashTransactionsModel(this);
  FilteredByAddressTransactionsModel* filteredByAddressModel = new FilteredByAddressTransactionsModel(this);
  m_filterByAgeModel = filterByAgeModel;
  m_filterByPeriodModel = filterByPeriodModel;
  m_filterByHashModel = filteredByHashModel;
  m_filterByAddressModel = filteredByAddressModel;
  FilteredTransactionsModel* transactionsModel = new FilteredTransactionsModel(this);
  TransactionsDelegate* transactionsDelegate = new TransactionsDelegate(m_cryptoNoteAdapter, m_addressBookManager, transactionsModel,
    m_transactionsModel, m_walletStateModel, this);
  filterByAgeModel->setSourceModel(_model);
  filterByPeriodModel->setSourceModel(m_filterByAgeModel);
  filteredByHashModel->setSourceModel(m_filterByPeriodModel);
  filteredByAddressModel->setSourceModel(m_filterByHashModel);
  transactionsModel->setSourceModel(m_filterByAddressModel);
  int newTransactionColumn = TransactionsModel::findProxyColumn(transactionsModel, TransactionsModel::COLUMN_NEW_TRANSACTION);
  int amountColumn = TransactionsModel::findProxyColumn(transactionsModel, TransactionsModel::COLUMN_AMOUNT);
  int transfersColumn = TransactionsModel::findProxyColumn(transactionsModel, TransactionsModel::COLUMN_TRANSFERS);
  int hashColumn = TransactionsModel::findProxyColumn(transactionsModel, TransactionsModel::COLUMN_HASH);
  int timeColumn = TransactionsModel::findProxyColumn(transactionsModel, TransactionsModel::COLUMN_TIME);
  int showTransfersColumn = TransactionsModel::findProxyColumn(transactionsModel, TransactionsModel::COLUMN_SHOW_TRANSFERS);

  m_ui->m_transactionsView->setModel(transactionsModel);
  m_ui->m_transactionsView->setLinkLikeColumnSet(QSet<int>() << hashColumn << showTransfersColumn);
  m_ui->m_transactionsView->setHorizontalHeader(new TransactionsHeaderView(this));
  m_ui->m_transactionsView->setItemDelegateForColumn(newTransactionColumn, new NewTransactionDelegate(this));
  m_ui->m_transactionsView->setItemDelegateForColumn(timeColumn, new TransactionsTimeDelegate(this));
  m_ui->m_transactionsView->setItemDelegateForColumn(amountColumn, new TransactionsAmountDelegate(true, this));
  m_ui->m_transactionsView->setItemDelegateForColumn(transfersColumn, transactionsDelegate);
  m_ui->m_transactionsView->setItemDelegateForColumn(showTransfersColumn, transactionsDelegate);
  m_ui->m_transactionsView->horizontalHeader()->setSectionResizeMode(newTransactionColumn, QHeaderView::Fixed);
  m_ui->m_transactionsView->horizontalHeader()->setSectionResizeMode(timeColumn, QHeaderView::Fixed);
  m_ui->m_transactionsView->horizontalHeader()->setSectionResizeMode(amountColumn, QHeaderView::Fixed);
  m_ui->m_transactionsView->horizontalHeader()->setSectionResizeMode(hashColumn, QHeaderView::Fixed);
  m_ui->m_transactionsView->horizontalHeader()->setSectionResizeMode(transfersColumn, QHeaderView::Stretch);
  m_ui->m_transactionsView->horizontalHeader()->resizeSection(newTransactionColumn, 6);
  m_ui->m_transactionsView->horizontalHeader()->resizeSection(timeColumn, 180);
  m_ui->m_transactionsView->horizontalHeader()->resizeSection(hashColumn, 280);
  m_ui->m_transactionsView->horizontalHeader()->resizeSection(amountColumn, 220);
  m_ui->m_transactionsView->horizontalHeader()->resizeSection(showTransfersColumn, 30);
  QDateTime currentDateTime = QDateTime::currentDateTime();
  m_ui->m_filterBeginDtedit->setDateTime(currentDateTime.addDays(-1));
  m_ui->m_filterEndDtedit->setDateTime(currentDateTime);
  connect(transactionsModel, &QAbstractItemModel::rowsInserted, this, &TransactionsFrame::rowsInserted);
}

void TransactionsFrame::exportToCsv() {
  Q_ASSERT(m_mainWindow != nullptr);
  QString file = QFileDialog::getSaveFileName(m_mainWindow, tr("Select CSV file"), QDir::homePath(), "CSV (*.csv)");
  if (!file.isEmpty()) {
    QByteArray csv = static_cast<TransactionsModel*>(m_transactionsModel)->toCsv();
    QFile f(file);
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
      f.write(csv);
      f.close();
    }
  }
}

void TransactionsFrame::transactionDoubleClicked(const QModelIndex& _index) {
  if (!_index.isValid()) {
    return;
  }

  if (_index.data(TransactionsModel::ROLE_COLUMN).toInt() == TransactionsModel::COLUMN_HASH) {
    TransactionDetailsDialog dlg(m_cryptoNoteAdapter, m_transactionsModel, _index, m_mainWindow);
    dlg.exec();
  }
}

void TransactionsFrame::transactionClicked(const QModelIndex& _index) {
  if (!_index.isValid()) {
    return;
  }

  if (_index.data(TransactionsModel::ROLE_COLUMN).toInt() == TransactionsModel::COLUMN_SHOW_TRANSFERS) {
    m_ui->m_transactionsView->model()->setData(_index, !_index.data(TransactionsModel::ROLE_SHOW_TRANSFERS).toBool(),
      TransactionsModel::ROLE_SHOW_TRANSFERS);
    int transfersColumn = TransactionsModel::findProxyColumn(m_ui->m_transactionsView->model(), TransactionsModel::COLUMN_TRANSFERS);
    QAbstractItemDelegate* delegate = m_ui->m_transactionsView->itemDelegateForColumn(transfersColumn);
    int rowHeight = delegate->sizeHint(QStyleOptionViewItem(), _index.sibling(_index.row(), TransactionsModel::COLUMN_TRANSFERS)).height();
    m_ui->m_transactionsView->setRowHeight(_index.row(), rowHeight);
  }
}

void TransactionsFrame::filterChanged(int _index) {
  static_cast<FilteredByAgeTransactionsModel*>(m_filterByAgeModel)->setFilter(static_cast<FilteredByAgeTransactionsModel::Filter>(_index));
  if (_index == FilteredByAgeTransactionsModel::FILTER_CUSTOM) {
    m_ui->m_filterPeriodFrame->show();
    QDateTime begin = m_ui->m_filterBeginDtedit->dateTime();
    QDateTime end = m_ui->m_filterEndDtedit->dateTime();
    static_cast<FilteredByPeriodTransactionsModel*>(m_filterByPeriodModel)->setFilter(begin, end);
  } else {
    m_ui->m_filterPeriodFrame->hide();
    static_cast<FilteredByPeriodTransactionsModel*>(m_filterByPeriodModel)->setFilter(QDateTime(), QDateTime());
  }
}

void TransactionsFrame::filterPeriodChanged(const QDateTime& _date_time) {
  if (m_ui->m_filterCombo->currentIndex() == FilteredByAgeTransactionsModel::FILTER_CUSTOM) {
    QDateTime begin = m_ui->m_filterBeginDtedit->dateTime();
    QDateTime end = m_ui->m_filterEndDtedit->dateTime();
    static_cast<FilteredByPeriodTransactionsModel*>(m_filterByPeriodModel)->setFilter(begin, end);
  }
}

void TransactionsFrame::filterHashChanged(const QString& _hash) {
  QString hash = m_ui->m_filterHashEdit->text().trimmed();
  static_cast<FilteredByHashTransactionsModel*>(m_filterByHashModel)->setFilter(hash);
}

void TransactionsFrame::filterAddressChanged(const QString& _hash) {
  QString address = m_ui->m_filterAddressEdit->text().trimmed();
  static_cast<FilteredByAddressTransactionsModel*>(m_filterByAddressModel)->setFilter(address);
}

void TransactionsFrame::showFilter(bool _on) {
  m_ui->m_filterButton->setText(_on ? tr("Hide filter") : tr("Show filter"));
  m_ui->m_filterButton->setEnabled(false);
  m_animation->setStartValue(_on ? 0 : FILTER_FRAME_HEIGHT);
  m_animation->setEndValue(_on ? FILTER_FRAME_HEIGHT : 0);
  if (_on) {
    m_ui->m_filterFrame->setMaximumHeight(0);
    m_ui->m_filterFrame->show();
    disconnect(m_animation, &QPropertyAnimation::finished, m_ui->m_filterFrame, &QFrame::hide);
  } else {
    connect(m_animation, &QPropertyAnimation::finished, m_ui->m_filterFrame, &QFrame::hide);
    connect(m_animation, &QPropertyAnimation::finished, this, static_cast<void(TransactionsFrame::*)()>(&TransactionsFrame::update));
  }

  m_animation->start();
  if (!_on) {
    resetFilter();
  }
}

void TransactionsFrame::resetClicked() {
  resetFilter();
}

void TransactionsFrame::rowsInserted(const QModelIndex& _parent, int _first, int _last) {
  if (_first != _last) {
    return;
  }

  QModelIndex index = m_ui->m_transactionsView->model()->index(_first, TransactionsModel::COLUMN_NEW_TRANSACTION);
  if (index.data(TransactionsModel::ROLE_NUMBER_OF_CONFIRMATIONS).value<quint32>() == 0) {
    m_ui->m_transactionsView->openPersistentEditor(index);
  }
}

void TransactionsFrame::resetFilter() {
  QDateTime currentDateTime = QDateTime::currentDateTime();
  m_ui->m_filterCombo->setCurrentIndex(0);
  m_ui->m_filterBeginDtedit->setDateTime(currentDateTime.addDays(-1));
  m_ui->m_filterEndDtedit->setDateTime(currentDateTime);
  m_ui->m_filterHashEdit->clear();
  m_ui->m_filterAddressEdit->clear();
}

}
