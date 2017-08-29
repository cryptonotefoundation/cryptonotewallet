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
#include "Application/IWalletUiItem.h"
#include "Settings/Settings.h"

namespace Ui {
class OverviewFrame;
}

namespace WalletGui {

class ICryptoNoteAdapter;

class OverviewFrame : public QFrame, public IWalletUiItem, public ISettingsObserver {
  Q_OBJECT
  Q_DISABLE_COPY(OverviewFrame)

public:
  explicit OverviewFrame(QWidget* _parent);
  ~OverviewFrame();

  // IWalletUiItem
  virtual void setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) override;
  virtual void setMiningManager(IMiningManager *_miningManager) override;
  virtual void setBlogReader(INewsReader* _blogReader) override;
  virtual void setMainWindow(QWidget* _mainWindow) override;
  virtual void setTransactionsModel(QAbstractItemModel* _model) override;
  virtual void setSortedTransactionsModel(QAbstractItemModel *_model) override;
  virtual void setTransactionPoolModel(QAbstractItemModel *_model) override;
  virtual void setNodeStateModel(QAbstractItemModel* _model) override;
  virtual void setWalletStateModel(QAbstractItemModel* _model) override;
  virtual void setMinerModel(QAbstractItemModel* _model) override;

  // ISettingsObserver
  virtual void settingsUpdated() override;

private:
  QScopedPointer<Ui::OverviewFrame> m_ui;
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  QWidget* m_mainWindow;
  QAbstractItemModel* m_transactionsModel;

  void rowsInserted(const QModelIndex& _parent, int _first, int _last);
  Q_SLOT void showTransactionDetails(const QModelIndex& _index);
};

}
