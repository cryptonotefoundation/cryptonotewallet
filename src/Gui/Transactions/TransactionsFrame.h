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

class QPropertyAnimation;

namespace Ui {
class TransactionsFrame;
}

namespace WalletGui {

class TransactionsFrame : public QFrame, public IWalletUiItem {
  Q_OBJECT
  Q_DISABLE_COPY(TransactionsFrame)

public:
  explicit TransactionsFrame(QWidget* _parent);
  ~TransactionsFrame();

  // IWalletUiItem
  virtual void setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) override;
  virtual void setAddressBookManager(IAddressBookManager* _addressBookManager) override;
  virtual void setMainWindow(QWidget* _mainWindow) override;
  virtual void setWalletStateModel(QAbstractItemModel* _model) override;
  virtual void setTransactionsModel(QAbstractItemModel* _model) override;
  virtual void setSortedTransactionsModel(QAbstractItemModel* _model) override;

private:
  QScopedPointer<Ui::TransactionsFrame> m_ui;
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  IAddressBookManager* m_addressBookManager;
  QWidget* m_mainWindow;
  QAbstractItemModel* m_transactionsModel;
  QAbstractItemModel* m_walletStateModel;
  QAbstractItemModel* m_filterByAgeModel;
  QAbstractItemModel* m_filterByPeriodModel;
  QAbstractItemModel* m_filterByHashModel;
  QAbstractItemModel* m_filterByAddressModel;
  QPropertyAnimation* m_animation;

  void rowsInserted(const QModelIndex& _parent, int _first, int _last);
  void resetFilter();

  Q_SLOT void exportToCsv();
  Q_SLOT void transactionDoubleClicked(const QModelIndex& _index);
  Q_SLOT void transactionClicked(const QModelIndex& _index);
  Q_SLOT void filterChanged(int _index);
  Q_SLOT void filterPeriodChanged(const QDateTime& _dateTime);
  Q_SLOT void filterHashChanged(const QString& _hash);
  Q_SLOT void filterAddressChanged(const QString& _hash);
  Q_SLOT void showFilter(bool _on);
  Q_SLOT void resetClicked();
};

}
