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

#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QThread>

#ifdef Q_OS_WIN
#include <stdint.h>
#endif

#include "BlockExplorerFrame.h"
#include "Settings/Settings.h"
#include "BlockDetailsDialog.h"
#include "Gui/Common/RightAlignmentColumnDelegate.h"
#include "Models/BlockchainModel.h"
#include "Models/TransactionPoolModel.h"
#include "Gui/Common/PoolTransactionDetailsDialog.h"
#include "WindowedBlockModel.h"

#include "ui_BlockExplorerFrame.h"

namespace WalletGui {

namespace {

const quint32 SHOW_FILTER_DURATION = 200;
const quint32 FILTER_FRAME_HEIGHT = 70;

}

BlockExplorerFrame::BlockExplorerFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::BlockExplorerFrame),
  m_cryptoNoteAdapter(nullptr), m_mainWindow(nullptr), m_blockChainModel(nullptr), m_transactionPoolModel(nullptr),
  m_animation(new QPropertyAnimation(this)) {
  m_ui->setupUi(this);
  m_ui->m_filterFrame->hide();
  m_ui->m_resetFilterButton->hide();
  m_ui->m_filterPeriodFrame->hide();

  m_animation->setTargetObject(m_ui->m_filterFrame);
  m_animation->setPropertyName("maximumHeight");
  m_animation->setDuration(SHOW_FILTER_DURATION);

  QDateTime currentDateTime = QDateTime::currentDateTimeUtc();
  m_ui->m_filterBeginDtedit->setDateTime(currentDateTime.addDays(-1));
  m_ui->m_filterEndDtedit->setDateTime(currentDateTime);

  connect(m_animation, &QPropertyAnimation::finished, this, &BlockExplorerFrame::updateButtonsText);
  connect(m_animation, &QPropertyAnimation::finished, this, [this] {
    m_ui->m_blockSearchButton->setEnabled(true);
    m_ui->m_blockExplorerView->setAutoUpdateRowCount(true);
    m_ui->m_blockExplorerView->updateRowCount();
  });
}

BlockExplorerFrame::~BlockExplorerFrame() {
}

void BlockExplorerFrame::setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) {
  m_cryptoNoteAdapter = _cryptoNoteAdapter;
  m_cryptoNoteAdapter->addObserver(this);
}

void BlockExplorerFrame::setMainWindow(QWidget* _mainWindow) {
  m_mainWindow = _mainWindow;
}

void BlockExplorerFrame::setBlockChainModel(QAbstractItemModel* _model) {
  if (_model == nullptr) {
    return;
  }

  m_blockChainModel = _model;
  WindowedBlockModel* windowedBlocksModel = new WindowedBlockModel(m_cryptoNoteAdapter, m_blockChainModel, this);
  m_ui->m_blockExplorerView->setModel(windowedBlocksModel);
  m_ui->m_blockExplorerView->setLinkLikeColumnSet(QSet<int>() << BlockchainModel::COLUMN_BLOCK_HEIGHT << BlockchainModel::COLUMN_BLOCK_HASH);
  RightAlignmentColumnDelegate* blockExplorerDelegate = new RightAlignmentColumnDelegate(false, this);
  m_ui->m_blockExplorerView->setItemDelegateForColumn(BlockchainModel::COLUMN_BLOCK_SIZE, blockExplorerDelegate);
  m_ui->m_blockExplorerView->setItemDelegateForColumn(BlockchainModel::COLUMN_BLOCK_TRANSACTION_COUNT, blockExplorerDelegate);
  m_ui->m_blockExplorerView->horizontalHeader()->setSectionResizeMode(BlockchainModel::COLUMN_BLOCK_TIME, QHeaderView::Fixed);
  m_ui->m_blockExplorerView->horizontalHeader()->setSectionResizeMode(BlockchainModel::COLUMN_BLOCK_HEIGHT, QHeaderView::Fixed);
  m_ui->m_blockExplorerView->horizontalHeader()->setSectionResizeMode(BlockchainModel::COLUMN_BLOCK_SIZE, QHeaderView::Fixed);
  m_ui->m_blockExplorerView->horizontalHeader()->setSectionResizeMode(BlockchainModel::COLUMN_BLOCK_TRANSACTION_COUNT, QHeaderView::Fixed);
  m_ui->m_blockExplorerView->horizontalHeader()->setSectionResizeMode(BlockchainModel::COLUMN_BLOCK_HASH, QHeaderView::Stretch);
  m_ui->m_blockExplorerView->horizontalHeader()->resizeSection(BlockchainModel::COLUMN_BLOCK_TIME, 180);
  m_ui->m_blockExplorerView->horizontalHeader()->resizeSection(BlockchainModel::COLUMN_BLOCK_HEIGHT, 100);
  m_ui->m_blockExplorerView->horizontalHeader()->resizeSection(BlockchainModel::COLUMN_BLOCK_SIZE, 100);
  m_ui->m_blockExplorerView->horizontalHeader()->resizeSection(BlockchainModel::COLUMN_BLOCK_TRANSACTION_COUNT, 100);

  qobject_cast<WalletTableView*>(m_ui->m_blockExplorerView)->updateRowCount();
  windowedBlocksModel->lastPage();
  updateButtonsText();
  connect(windowedBlocksModel, &WindowedBlockModel::pageCountUpdated, this, &BlockExplorerFrame::updateButtonsText);
}

void BlockExplorerFrame::setTransactionPoolModel(QAbstractItemModel* _model) {
  if (_model == nullptr) {
    return;
  }

  m_transactionPoolModel = _model;
  m_ui->m_transactionPoolView->setModel(m_transactionPoolModel);
  m_ui->m_transactionPoolView->horizontalHeader()->setSectionHidden(TransactionPoolModel::COLUMN_MIXIN, true);
  m_ui->m_transactionPoolView->horizontalHeader()->setSectionHidden(TransactionPoolModel::COLUMN_PAYMENT_ID, true);
  m_ui->m_transactionPoolView->setLinkLikeColumnSet(QSet<int>() << TransactionPoolModel::COLUMN_HASH);
  RightAlignmentColumnDelegate* transactionsPoolDelegate = new RightAlignmentColumnDelegate(false, this);
  m_ui->m_transactionPoolView->setItemDelegateForColumn(TransactionPoolModel::COLUMN_AMOUNT, transactionsPoolDelegate);
  m_ui->m_transactionPoolView->setItemDelegateForColumn(TransactionPoolModel::COLUMN_FEE, transactionsPoolDelegate);
  m_ui->m_transactionPoolView->setItemDelegateForColumn(TransactionPoolModel::COLUMN_SIZE, transactionsPoolDelegate);
  m_ui->m_transactionPoolView->horizontalHeader()->setSectionResizeMode(TransactionPoolModel::COLUMN_TIME, QHeaderView::Fixed);
  m_ui->m_transactionPoolView->horizontalHeader()->setSectionResizeMode(TransactionPoolModel::COLUMN_AMOUNT, QHeaderView::Fixed);
  m_ui->m_transactionPoolView->horizontalHeader()->setSectionResizeMode(TransactionPoolModel::COLUMN_FEE, QHeaderView::Fixed);
  m_ui->m_transactionPoolView->horizontalHeader()->setSectionResizeMode(TransactionPoolModel::COLUMN_SIZE, QHeaderView::Fixed);
  m_ui->m_transactionPoolView->horizontalHeader()->setSectionResizeMode(TransactionPoolModel::COLUMN_HASH, QHeaderView::Stretch);

  m_ui->m_transactionPoolView->horizontalHeader()->resizeSection(TransactionPoolModel::COLUMN_TIME, 190);
  m_ui->m_transactionPoolView->horizontalHeader()->resizeSection(TransactionPoolModel::COLUMN_AMOUNT, 200);
  m_ui->m_transactionPoolView->horizontalHeader()->resizeSection(TransactionPoolModel::COLUMN_FEE, 120);
  m_ui->m_transactionPoolView->horizontalHeader()->resizeSection(TransactionPoolModel::COLUMN_SIZE, 120);
}

void BlockExplorerFrame::cryptoNoteAdapterInitCompleted(int _status) {
  if (_status == 0) {
    qobject_cast<WalletTableView*>(m_ui->m_blockExplorerView)->updateRowCount();
    updateButtonsText();
  }
}

void BlockExplorerFrame::cryptoNoteAdapterDeinitCompleted() {
  // Do nothing
}

void BlockExplorerFrame::showEvent(QShowEvent* _event) {
  lastPage();
  QFrame::showEvent(_event);
}

void BlockExplorerFrame::resizeEvent(QResizeEvent* _event) {
  if (_event->oldSize().height() != _event->size().height()) {
    updateButtonsText();
  }

  QFrame::resizeEvent(_event);
}

void BlockExplorerFrame::nextPage() {
  m_ui->m_nextPageButton->setEnabled(false);
  setCursor(Qt::WaitCursor);
  static_cast<WindowedBlockModel*>(m_ui->m_blockExplorerView->model())->nextPage();
  m_ui->m_nextPageButton->setEnabled(true);
  unsetCursor();
  updateButtonsText();
}

void BlockExplorerFrame::prevPage() {
  m_ui->m_prevPageButton->setEnabled(false);
  setCursor(Qt::WaitCursor);
  static_cast<WindowedBlockModel*>(m_ui->m_blockExplorerView->model())->prevPage();
  m_ui->m_prevPageButton->setEnabled(true);
  unsetCursor();
  updateButtonsText();
}

void BlockExplorerFrame::lastPage() {
  setCursor(Qt::WaitCursor);
  static_cast<WindowedBlockModel*>(m_ui->m_blockExplorerView->model())->lastPage();
  unsetCursor();
  updateButtonsText();
}

void BlockExplorerFrame::showBlockDetails(const QModelIndex& _index) {
  if(!_index.isValid()) {
    return;
  }

  QModelIndex filteredIndex = static_cast<WindowedBlockModel*>(m_ui->m_blockExplorerView->model())->mapToSource(_index);
  QModelIndex blockIndex = static_cast<FilteredBlockModel*>(
    static_cast<WindowedBlockModel*>(m_ui->m_blockExplorerView->model())->sourceModel())->mapToSource(filteredIndex);
  BlockDetailsDialog dlg(m_blockChainModel, blockIndex, m_mainWindow);
  dlg.exec();
}

void BlockExplorerFrame::showTransactionDetails(const QModelIndex& _index) {
  PoolTransactionDetailsDialog dlg(m_transactionPoolModel, _index, m_mainWindow);
  dlg.exec();
}

void BlockExplorerFrame::showSearch(bool _on) {
  m_ui->m_blockSearchButton->setText(_on ? tr("Hide search") : tr("Show search"));
  m_ui->m_blockSearchButton->setEnabled(false);
  m_animation->setStartValue(_on ? 0 : FILTER_FRAME_HEIGHT);
  m_animation->setEndValue(_on ? FILTER_FRAME_HEIGHT : 0);
  if(_on) {
    m_ui->m_filterFrame->setMaximumHeight(0);
    m_ui->m_filterFrame->show();
    disconnect(m_animation, &QPropertyAnimation::finished, m_ui->m_filterFrame, &QFrame::hide);
  } else {
    resetFilter();
    connect(m_animation, &QPropertyAnimation::finished, m_ui->m_filterFrame, &QFrame::hide);
    connect(m_animation, &QPropertyAnimation::finished, this, static_cast<void(BlockExplorerFrame::*)()>(&BlockExplorerFrame::update));
  }

  m_ui->m_blockExplorerView->setAutoUpdateRowCount(false);
  m_animation->start();
  m_ui->m_resetFilterButton->setVisible(_on);
}

void BlockExplorerFrame::applyFilter() {
  m_ui->m_applyFilterButton->setEnabled(false);
  setCursor(Qt::WaitCursor);
  QDateTime begin = m_ui->m_filterBeginDtedit->dateTime();
  QDateTime end = m_ui->m_filterEndDtedit->dateTime();
  QString hash = m_ui->m_filterHashEdit->text().trimmed();
  static_cast<WindowedBlockModel*>(
    m_ui->m_blockExplorerView->model())->setFilter(static_cast<FilteredBlockModel::Filter>(m_ui->m_filterCombo->currentIndex()),
      m_ui->m_filterCombo->currentIndex() == FilteredBlockModel::Custom, begin, end, hash);
  lastPage();
  unsetCursor();
  m_ui->m_applyFilterButton->setEnabled(true);
}

void BlockExplorerFrame::resetFilterClicked() {
  resetFilter();
}

void BlockExplorerFrame::resetFilter() {
  QDateTime currentDateTime = QDateTime::currentDateTimeUtc();
  m_ui->m_filterCombo->setCurrentIndex(FilteredBlockModel::All);
  m_ui->m_filterBeginDtedit->setDateTime(currentDateTime);
  m_ui->m_filterEndDtedit->setDateTime(currentDateTime);
  m_ui->m_filterHashEdit->clear();
  static_cast<WindowedBlockModel*>(m_ui->m_blockExplorerView->model())->resetFilter();
  lastPage();
}

void BlockExplorerFrame::filterChanged(int _index) {
  m_ui->m_filterPeriodFrame->setVisible(_index == FilteredBlockModel::Custom);
}

void BlockExplorerFrame::updateButtonsText() {
  QPair<quint64, quint64> prevInterval = static_cast<WindowedBlockModel*>(m_ui->m_blockExplorerView->model())->prevPageInterval();
  QPair<quint64, quint64> nextInterval = static_cast<WindowedBlockModel*>(m_ui->m_blockExplorerView->model())->nextPageInterval();

  if(prevInterval.first == std::numeric_limits<quint32>::max()) {
    m_ui->m_prevPageButton->hide();
  } else {
    m_ui->m_prevPageButton->show();
    m_ui->m_prevPageButton->setText(QString("< %1 - %2").arg(prevInterval.first).arg(prevInterval.second));
  }

  if(nextInterval.first == std::numeric_limits<quint32>::max()) {
    m_ui->m_nextPageButton->hide();
    m_ui->m_topButton->hide();
  } else {
    m_ui->m_nextPageButton->show();
    m_ui->m_topButton->show();
    m_ui->m_nextPageButton->setText(QString("%1 - %2 >").arg(nextInterval.first).arg(nextInterval.second));
  }
}

}
