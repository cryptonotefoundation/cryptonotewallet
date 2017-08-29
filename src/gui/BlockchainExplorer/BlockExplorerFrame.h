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

#pragma once

#include <QFrame>
#include <QTreeView>

#include "Application/IWalletUiItem.h"
#include "ICryptoNoteAdapter.h"

class QPropertyAnimation;

namespace Ui {
class BlockExplorerFrame;
}

namespace WalletGui {

class BlockExplorerFrame : public QFrame, public IWalletUiItem, public ICryptoNoteAdapterObserver {
  Q_OBJECT
  Q_DISABLE_COPY(BlockExplorerFrame)

public:
  explicit BlockExplorerFrame(QWidget* _parent);
  ~BlockExplorerFrame();

  // IWalletUiItem
  virtual void setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) override;
  virtual void setMainWindow(QWidget* _mainWindow) override;
  virtual void setBlockChainModel(QAbstractItemModel* _model) override;
  virtual void setTransactionPoolModel(QAbstractItemModel* _model) override;

  // ICryptoNoteAdapterObserver
  Q_SLOT virtual void cryptoNoteAdapterInitCompleted(int _status) override;
  Q_SLOT virtual void cryptoNoteAdapterDeinitCompleted() override;

protected:
  virtual void showEvent(QShowEvent* _event) override;
  virtual void resizeEvent(QResizeEvent* _event) override;

private:
  QScopedPointer<Ui::BlockExplorerFrame> m_ui;
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  QWidget* m_mainWindow;
  QAbstractItemModel* m_blockChainModel;
  QAbstractItemModel* m_transactionPoolModel;
  QPropertyAnimation* m_animation;

  void updateButtonsText();
  void resetFilter();

  Q_SLOT void nextPage();
  Q_SLOT void prevPage();
  Q_SLOT void lastPage();
  Q_SLOT void showBlockDetails(const QModelIndex &_index);
  Q_SLOT void showTransactionDetails(const QModelIndex &_index);
  Q_SLOT void showSearch(bool _on);
  Q_SLOT void applyFilter();
  Q_SLOT void resetFilterClicked();
  Q_SLOT void filterChanged(int _index);
};

}
