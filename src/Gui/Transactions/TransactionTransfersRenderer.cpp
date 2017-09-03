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

#include <QPersistentModelIndex>

#include "TransactionTransfersRenderer.h"
#include "Settings/Settings.h"
#include "Gui/Common/RightAlignmentColumnDelegate.h"
#include "DrawableTransfersModel.h"
#include "Style/Style.h"
#include "Models/TransactionsModel.h"
#include "Models/TransfersModel.h"
#include "ui_TransactionTransfersRenderer.h"

namespace WalletGui {

namespace {

const int TREE_VIEW_ITEM_HEIGHT = 18;
const QString STYLE_SHEET_TEMPLATE =
  "* {"
    "font-family: %fontFamily%;"
  "}"

  "WalletGui--TransactionTransfersRenderer QTreeView {"
    "background: transparent;"
    "font-size: %fontSizeSmall%;"
  "}"

  "WalletGui--TransactionTransfersRenderer QTreeView::item {"
    "min-height: 18px;"
    "border-left: 5px solid transparent;"
    "border-right: 5px solid transparent;"
  "}";

}

TransactionTransfersRenderer::TransactionTransfersRenderer(ICryptoNoteAdapter* _cryptoNoteAdapter, IAddressBookManager* _addressBookManager,
  const QModelIndex& _transactionIndex, const QString& _walletAddress, QWidget* _parent) : QFrame(_parent), m_ui(new Ui::TransactionTransfersRenderer) {
  m_ui->setupUi(this);
  m_ui->m_transfersView->viewport()->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

  QAbstractItemModel* model = new TransfersModel(_cryptoNoteAdapter, _transactionIndex, this);
  DrawableTransfersModel* drawableModel = new DrawableTransfersModel(_cryptoNoteAdapter, _addressBookManager, _walletAddress, this);
  bool showDetails = model->rowCount() <= 1 || _transactionIndex.data(TransactionsModel::ROLE_SHOW_TRANSFERS).toBool();
  drawableModel->showDetails(showDetails);
  drawableModel->setSourceModel(model);
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(STYLE_SHEET_TEMPLATE));
  m_ui->m_transfersView->setModel(drawableModel);
  m_ui->m_transfersView->setItemDelegateForColumn(TransfersModel::COLUMN_ADDRESS, new RightAlignmentColumnDelegate(false, this));
  m_ui->m_transfersView->header()->setSectionResizeMode(TransfersModel::COLUMN_ADDRESS, QHeaderView::Stretch);
  m_ui->m_transfersView->header()->setSectionResizeMode(TransfersModel::COLUMN_AMOUNT, QHeaderView::Fixed);
  m_ui->m_transfersView->header()->resizeSection(TransfersModel::COLUMN_AMOUNT, 110);
  m_ui->m_transfersView->setFixedHeight(drawableModel->rowCount() * TREE_VIEW_ITEM_HEIGHT);
}

TransactionTransfersRenderer::~TransactionTransfersRenderer() {
}

int TransactionTransfersRenderer::calculateItemHeight(quintptr _transferCount) {
  return _transferCount * TREE_VIEW_ITEM_HEIGHT + 10;
}

}
