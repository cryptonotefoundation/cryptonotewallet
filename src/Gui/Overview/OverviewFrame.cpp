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
#include <QDataWidgetMapper>

#include "OverviewFrame.h"
#include "Gui/Common/NewTransactionDelegate.h"
#include "Gui/Common/TransactionDetailsDialog.h"
#include "Gui/Common/TransactionsAmountDelegate.h"
#include "Gui/Common/TransactionsHeaderView.h"
#include "Gui/Common/TransactionsTimeDelegate.h"
#include "Gui/Common/RightAlignmentColumnDelegate.h"
#include "Models/TransactionsModel.h"
#include "Models/SortedTransactionsModel.h"
#include "Models/WalletStateModel.h"
#include "RecentTransactionsModel.h"
#include "Style/Style.h"

#include "ui_OverviewFrame.h"

namespace WalletGui {

namespace {

const char OVERVIEW_STYLE_SHEET_TEMPLATE[] =
  "* {"
    "font-family: %fontFamily%;"
  "}"

  "WalletGui--OverviewFrame {"
    "background-color: #ffffff;"
    "border: none;"
  "}"

  "WalletGui--OverviewFrame #m_allTransactionsButton {"
    "margin-top: 2px;"
  "}"

  "WalletGui--OverviewFrame #m_newsFrame {"
    "min-height: 130px;"
    "max-height: 130px;"
    "border: none;"
    "border-top: 1px solid %borderColor%;"
    "background-color: %backgroundColorGray%;"
  "}";

}

OverviewFrame::OverviewFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::OverviewFrame), m_cryptoNoteAdapter(nullptr),
  m_mainWindow(nullptr) {
  m_ui->setupUi(this);

  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(OVERVIEW_STYLE_SHEET_TEMPLATE));
  Settings::instance().addObserver(this);
  m_ui->m_newsFrame->setVisible(Settings::instance().isNewsEnabled());
}

OverviewFrame::~OverviewFrame() {
  Settings::instance().removeObserver(this);
}

void OverviewFrame::setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) {
  m_cryptoNoteAdapter = _cryptoNoteAdapter;
  m_ui->m_overviewHeaderFrame->setCryptoNoteAdapter(m_cryptoNoteAdapter);
}

void OverviewFrame::setMiningManager(IMiningManager* _miningManager) {
  m_ui->m_overviewHeaderFrame->setMiningManager(_miningManager);
}

void OverviewFrame::setBlogReader(INewsReader* _blogReader) {
  m_ui->m_blogFrame->setNewsReader(_blogReader);
}

void OverviewFrame::setMainWindow(QWidget* _mainWindow) {
  m_mainWindow = _mainWindow;
  QList<QPushButton*> buttonList = m_mainWindow->findChildren<QPushButton*>("m_transactionsButton");
  Q_ASSERT(!buttonList.isEmpty());
  connect(m_ui->m_allTransactionsButton, &WalletTinyLinkLikeButton::clicked, buttonList.first(), &QPushButton::click);
  m_ui->m_overviewHeaderFrame->setMainWindow(m_mainWindow);
}

void OverviewFrame::setTransactionsModel(QAbstractItemModel* _model) {
  m_transactionsModel = _model;
}

void OverviewFrame::setSortedTransactionsModel(QAbstractItemModel* _model) {
  RecentTransactionsModel* recentTransactionsModel = new RecentTransactionsModel(_model, this);
  int newTransactionColumn = TransactionsModel::findProxyColumn(recentTransactionsModel, TransactionsModel::COLUMN_NEW_TRANSACTION);
  int amountColumn = TransactionsModel::findProxyColumn(recentTransactionsModel, TransactionsModel::COLUMN_AMOUNT);
  int hashColumn = TransactionsModel::findProxyColumn(recentTransactionsModel, TransactionsModel::COLUMN_HASH);
  int timeColumn = TransactionsModel::findProxyColumn(recentTransactionsModel, TransactionsModel::COLUMN_TIME);
  m_ui->m_recentTransactionsView->setModel(recentTransactionsModel);
  m_ui->m_recentTransactionsView->setLinkLikeColumnSet(QSet<int>() << hashColumn);
  m_ui->m_recentTransactionsView->setHorizontalHeader(new TransactionsHeaderView(this));
  m_ui->m_recentTransactionsView->setItemDelegateForColumn(newTransactionColumn, new NewTransactionDelegate(this));
  m_ui->m_recentTransactionsView->setItemDelegateForColumn(timeColumn, new TransactionsTimeDelegate(this));
  m_ui->m_recentTransactionsView->setItemDelegateForColumn(amountColumn, new TransactionsAmountDelegate(true, this));
  m_ui->m_recentTransactionsView->horizontalHeader()->setSectionResizeMode(newTransactionColumn, QHeaderView::Fixed);
  m_ui->m_recentTransactionsView->horizontalHeader()->setSectionResizeMode(timeColumn, QHeaderView::Fixed);
  m_ui->m_recentTransactionsView->horizontalHeader()->setSectionResizeMode(amountColumn, QHeaderView::Fixed);
  m_ui->m_recentTransactionsView->horizontalHeader()->setSectionResizeMode(hashColumn, QHeaderView::Stretch);
  m_ui->m_recentTransactionsView->horizontalHeader()->resizeSection(newTransactionColumn, 6);
  m_ui->m_recentTransactionsView->horizontalHeader()->resizeSection(timeColumn, 180);
  m_ui->m_recentTransactionsView->horizontalHeader()->resizeSection(amountColumn, 220);

  connect(recentTransactionsModel, &QAbstractItemModel::rowsInserted, this, &OverviewFrame::rowsInserted);
}

void OverviewFrame::setTransactionPoolModel(QAbstractItemModel* _model) {
  m_ui->m_overviewHeaderFrame->setTransactionPoolModel(_model);
}

void OverviewFrame::setNodeStateModel(QAbstractItemModel* _model) {
  m_ui->m_overviewHeaderFrame->setNodeStateModel(_model);
}

void OverviewFrame::setWalletStateModel(QAbstractItemModel* _model) {
  m_ui->m_overviewHeaderFrame->setWalletStateModel(_model);
}

void OverviewFrame::setMinerModel(QAbstractItemModel* _model) {
  m_ui->m_overviewHeaderFrame->setMinerModel(_model);
}

void OverviewFrame::settingsUpdated() {
  m_ui->m_newsFrame->setVisible(Settings::instance().isNewsEnabled());
}

void OverviewFrame::rowsInserted(const QModelIndex& _parent, int _first, int _last) {
  if (_first != _last) {
    return;
  }

  QModelIndex index = m_ui->m_recentTransactionsView->model()->index(_first, TransactionsModel::COLUMN_NEW_TRANSACTION);
  if (index.data(TransactionsModel::ROLE_NUMBER_OF_CONFIRMATIONS).value<quint32>() == 0) {
    m_ui->m_recentTransactionsView->openPersistentEditor(index);
  }
}

void OverviewFrame::showTransactionDetails(const QModelIndex& _index) {
  if (!_index.isValid()) {
    return;
  }

  TransactionDetailsDialog dlg(m_cryptoNoteAdapter, m_transactionsModel, _index, m_mainWindow);
  dlg.exec();
}

}
