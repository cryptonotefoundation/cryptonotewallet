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

#include <QLayout>
#include <QPainter>
#include <QPushButton>
#include <QStyleOptionViewItem>
#include <QTime>

#include "TransactionsDelegate.h"
#include "Settings/Settings.h"
#include "Models/TransactionsModel.h"
#include "Models/WalletStateModel.h"
#include "Style/Style.h"
#include "TransactionTransfersRenderer.h"

Q_DECLARE_METATYPE(QList<CryptoNote::WalletTransfer>)

namespace WalletGui {

namespace {

const char SHOW_TRANSFERS_BUTTON_STYLE_SHEET[] =
  "QPushButton {"
    "border: none;"
    "min-width: 30px;"
    "max-width: 30px;"
    "min-height: 30px;"
    "max-height: 30px;"
    "image: url(:icons/arrow-circle);"
  "}"
  "QPushButton:hover {"
    "image: url(:icons/arrow-circle-hover);"
  "}"
    "QPushButton:checked {"
    "image: url(:icons/arrow-circle-up);"
  "}"
  "QPushButton:checked:hover {"
    "image: url(:icons/arrow-circle-up-hover);"
  "}";

}

TransactionsDelegate::TransactionsDelegate(ICryptoNoteAdapter* _cryptoNoteAdapter, IAddressBookManager* _addressBookManager,
  QAbstractItemModel* _viewModel, QAbstractItemModel* _transactionsModel, QAbstractItemModel* _walletModel, QObject* _parent) :
  QStyledItemDelegate(_parent), m_cryptoNoteAdapter(_cryptoNoteAdapter), m_addressBookManager(_addressBookManager),
  m_transactionsModel(_transactionsModel), m_walletModel(_walletModel) {
  m_cryptoNoteAdapter->addObserver(this);
  m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
}

TransactionsDelegate::~TransactionsDelegate() {
}

void TransactionsDelegate::paint(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index) const {
  if (!_index.isValid()) {
    QStyledItemDelegate::paint(_painter, _option, _index);
    return;
  }

  int column = _index.data(TransactionsModel::ROLE_COLUMN).toInt();
  switch (column) {
  case TransactionsModel::COLUMN_TRANSFERS: {
    QStyleOptionViewItem option(_option);
    initStyleOption(&option, _index);
    int row = _index.data(TransactionsModel::ROLE_ROW).toInt();
    QModelIndex index = m_transactionsModel->index(row, column);
    drawTransfers(_painter, option, index);
    return;
  }
  case TransactionsModel::COLUMN_SHOW_TRANSFERS: {
    QStyleOptionViewItem option(_option);
    initStyleOption(&option, _index);
    int row = _index.data(TransactionsModel::ROLE_ROW).toInt();
    QModelIndex index = m_transactionsModel->index(row, column);
    drawShowTransfersButton(_painter, option, index);
    return;
  }
  default:
    break;
  }

  QStyledItemDelegate::paint(_painter, _option, _index);
}

QSize TransactionsDelegate::sizeHint(const QStyleOptionViewItem& _option, const QModelIndex& _index) const {
  if (_index.data(TransactionsModel::ROLE_COLUMN).toInt() == TransactionsModel::COLUMN_TRANSFERS) {
    return QSize(0, getTransfersColumnSize(_index));
  }

  return QSize(0, Settings::instance().getCurrentStyle().viewItemHeight());
}

void TransactionsDelegate::walletOpened() {
  // Do nothing
}

void TransactionsDelegate::walletOpenError(int _initStatus) {
  // Do nothing
}

void TransactionsDelegate::walletClosed() {
  // Do nothing
}

void TransactionsDelegate::passwordChanged() {
  // Do nothing
}

void TransactionsDelegate::synchronizationProgressUpdated(quint32 _current, quint32 _total) {
  // Do nothing
}

void TransactionsDelegate::synchronizationCompleted() {
  // Do nothing
}

void TransactionsDelegate::balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) {
  // Do nothing
}

void TransactionsDelegate::externalTransactionCreated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void TransactionsDelegate::transactionUpdated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void TransactionsDelegate::cryptoNoteAdapterInitCompleted(int _status) {
  if (_status == 0) {
    m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
  }
}

void TransactionsDelegate::cryptoNoteAdapterDeinitCompleted() {
  // Do nothings
}

int TransactionsDelegate::getTransfersColumnSize(const QModelIndex& _index) const {
  if (_index.data(TransactionsModel::ROLE_SHOW_TRANSFERS).toBool()) {
    return std::max(TransactionTransfersRenderer::calculateItemHeight(_index.data(TransactionsModel::ROLE_TRANSFER_COUNT).toInt()),
      Settings::instance().getCurrentStyle().viewItemHeight());
  }

  return Settings::instance().getCurrentStyle().viewItemHeight();
}

void TransactionsDelegate::drawTransfers(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index) const {
  QString walletAddress = m_walletModel->index(0, WalletStateModel::COLUMN_ADDRESS).data().toString();
  TransactionTransfersRenderer renderer(m_cryptoNoteAdapter, m_addressBookManager, _index, walletAddress, nullptr);
  renderer.resize(_option.rect.size());
  _painter->save();
  _option.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &_option, _painter, _option.widget);
  _painter->translate(_option.rect.topLeft());
  renderer.render(_painter, QPoint(), QRegion(), QWidget::DrawChildren);
  _painter->restore();
}

void TransactionsDelegate::drawShowTransfersButton(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index) const {
  bool transfersIsVisible = _index.data(TransactionsModel::ROLE_SHOW_TRANSFERS).toBool();
  bool isHovered = (_option.state & QStyle::State_MouseOver);
  QString pixmapFile = ":icons/arrow-circle";
  pixmapFile.append(transfersIsVisible ? "-up" : "");
  pixmapFile.append(isHovered ? "-hover" : "");
  QPixmap pixmap(pixmapFile);
  _option.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &_option, _painter, _option.widget);
  if (_index.data(TransactionsModel::ROLE_TRANSFER_COUNT).toInt() < 2) {
    return;
  }

  QRect itemRect(_option.rect);
  itemRect.setHeight(Settings::instance().getCurrentStyle().viewItemHeight());
  QRect pixmapRect(pixmap.rect());
  pixmapRect.moveCenter(itemRect.center());
  pixmapRect.moveLeft(itemRect.left());
  _option.widget->style()->drawItemPixmap(_painter, pixmapRect, _option.decorationAlignment, pixmap);
}

}
