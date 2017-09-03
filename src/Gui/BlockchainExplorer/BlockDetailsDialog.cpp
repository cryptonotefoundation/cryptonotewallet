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

#include "BlockDetailsDialog.h"
#include "Settings/Settings.h"
#include "Gui/Common/RightAlignmentColumnDelegate.h"
#include "Models/BlockchainModel.h"
#include "TransactionsBlockModel.h"
#include "Style/Style.h"
#include "ui_BlockDetailsDialog.h"

namespace WalletGui {

namespace {

const char BLOCK_DETAILS_DIALOG_STYLE_SHEET_TEMPLATE[] =


"#BlockDetailsDialog {"
  "min-height: 700px;"
  "max-height: 700px;"
"}"

"#BlockDetailsDialog > #m_blockDetailsTabBar {"
  "border: none;"
"}"

"#BlockDetailsDialog > #m_blockDetailsTabBar::pane {"
  "border: none;"
  "border-top: 1px solid %borderColorDark%;"
  "top: -1px;"
"}"

"#BlockDetailsDialog > #m_blockDetailsTabBar::tab-bar {"
  "alignment: center;"
"}"

"#BlockDetailsDialog > #m_blockDetailsTabBar QTabBar::tab {"
  "font-size: %fontSizeLarge%;"
  "font-weight: bold;"
  "min-width: 380px;"
  "max-width: 380px;"
  "min-height: 30px;"
  "max-height: 30px;"
  "border: 1px solid %borderColorDark%;"
  "margin-left: 2px;"
  "margin-right: 2px;"
  "border-top-left-radius: 2px;"
  "border-top-right-radius: 2px;"
"}"

"#BlockDetailsDialog > #m_blockDetailsTabBar QTabBar::tab:selected {"
  "background: #ffffff;"
  "border: 1px solid %borderColorDark%;"
  "border-bottom: 1px solid #ffffff;"
"}"

"#BlockDetailsDialog > #m_blockDetailsTabBar QTabBar::tab:!selected {"
  "background: %backgroundColorGray%;"
  "border: 1px solid %borderColorDark%;"
"}"

"#m_generalTab {"
  "background-color: #ffffff;"
  "border: none;"
"}"

"#m_generalTab > WalletGui--WalletNormalGrayTextLabel, "
"#m_generalTab >WalletGui--WalletNormalBlackTextLabel {"
  "min-height: 41px;"
  "max-height: 41px;"
  "padding-left: 30px;"
  "padding-right: 30px;"
  "border: none;"
"}"

"#m_generalTab > #m_blockTextLabel,"
"#m_generalTab > #m_blockHashLabel {"
  "padding-left: 0px;"
  "padding-right: 0px;"
"}"

"#m_generalTab, #m_transactionsTab {"
  "background-color: #ffffff;"
"}"

"#m_generalTab > #m_heightTextLabel,"
"#m_generalTab > #m_heightLabel,"
"#m_generalTab > #m_difficultyTextLabel,"
"#m_generalTab > #m_difficultyLabel,"
"#m_generalTab > #m_transactionCountTextLabel,"
"#m_generalTab > #m_transactionCountLabel,"
"#m_generalTab > #m_blockSizeTextLabel,"
"#m_generalTab > #m_blockSizeLabel,"
"#m_generalTab > #m_baseRewardTextLabel,"
"#m_generalTab > #m_baseRewardLabel,"
"#m_generalTab > #m_rewardTextLabel,"
"#m_generalTab > #m_rewardLabel,"
"#m_generalTab > #m_totalTransactionsTextLabel,"
"#m_generalTab > #m_totalTransactionsLabel {"
  "border: none;"
  "background-color: %backgroundColorAlternate%;"
"}";

}

BlockDetailsDialog::BlockDetailsDialog(QAbstractItemModel* m_blockChainModel, const QModelIndex &_index, QWidget* _parent) :
  QDialog(_parent, static_cast<Qt::WindowFlags>(Qt::WindowCloseButtonHint)), m_ui(new Ui::BlockDetailsDialog) {
  m_ui->setupUi(this);
  TransactionsBlockModel* transactionsBlockModel = new TransactionsBlockModel(m_blockChainModel, this);
  m_ui->m_blockTransactionsView->setModel(transactionsBlockModel);
  RightAlignmentColumnDelegate* blockExplorerDelegate = new RightAlignmentColumnDelegate(false, this);
  m_ui->m_blockTransactionsView->setItemDelegate(blockExplorerDelegate);
  m_ui->m_blockTransactionsView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  m_ui->m_blockTransactionsView->header()->setSectionResizeMode(1, QHeaderView::Stretch);
  m_ui->m_blockTransactionsView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  m_ui->m_blockTransactionsView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  m_ui->m_blockTransactionsView->header()->setSectionResizeMode(4, QHeaderView::Stretch);

  m_mapper.setModel(m_blockChainModel);
  m_mapper.addMapping(m_ui->m_blockHashLabel, BlockchainModel::COLUMN_BLOCK_HASH, "text");
  m_mapper.addMapping(m_ui->m_heightLabel, BlockchainModel::COLUMN_BLOCK_HEIGHT, "text");
  m_mapper.addMapping(m_ui->m_timestampLabel, BlockchainModel::COLUMN_BLOCK_TIME, "text");
  m_mapper.addMapping(m_ui->m_difficultyLabel, BlockchainModel::COLUMN_BLOCK_DIFFICULTY, "text");
  m_mapper.addMapping(m_ui->m_orphanLabel, BlockchainModel::COLUMN_BLOCK_IS_ORPHANED, "text");
  m_mapper.addMapping(m_ui->m_transactionCountLabel, BlockchainModel::COLUMN_BLOCK_TRANSACTION_COUNT, "text");
  m_mapper.addMapping(m_ui->m_transactionsSizeLabel, BlockchainModel::COLUMN_BLOCK_TRANSACTIONS_SIZE, "text");
  m_mapper.addMapping(m_ui->m_blockSizeLabel, BlockchainModel::COLUMN_BLOCK_SIZE, "text");
  m_mapper.addMapping(m_ui->m_penaltyLabel, BlockchainModel::COLUMN_BLOCK_REWARD_PENALTY, "text");
  m_mapper.addMapping(m_ui->m_baseRewardLabel, BlockchainModel::COLUMN_BLOCK_BASE_REWARD, "text");
  m_mapper.addMapping(m_ui->m_feeLabel, BlockchainModel::COLUMN_BLOCK_FEE, "text");
  m_mapper.addMapping(m_ui->m_rewardLabel, BlockchainModel::COLUMN_BLOCK_REWARD, "text");
  m_mapper.addMapping(m_ui->m_totalCoinsLabel, BlockchainModel::COLUMN_BLOCK_TOTAL_COIN_COUNT, "text");
  m_mapper.addMapping(m_ui->m_totalTransactionsLabel, BlockchainModel::COLUMN_BLOCK_TOTAL_TRANSACTION_COUNT, "text");
  m_mapper.setCurrentModelIndex(_index);
  transactionsBlockModel->setBlockIndex(_index);
  m_ui->m_blockTransactionsView->setRootIndex(transactionsBlockModel->index(0, 0));
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(BLOCK_DETAILS_DIALOG_STYLE_SHEET_TEMPLATE));
  m_ui->m_blockHashLabel->installEventFilter(this);
}

BlockDetailsDialog::~BlockDetailsDialog() {
}

bool WalletGui::BlockDetailsDialog::eventFilter(QObject* _object, QEvent* _event) {
  if (_object == m_ui->m_blockHashLabel && _event->type() == QEvent::MouseButtonRelease) {
    QApplication::clipboard()->setText(m_ui->m_blockHashLabel->text());
     m_ui->m_copyLabel->start();
  }

  return QDialog::eventFilter(_object, _event);
}

}
