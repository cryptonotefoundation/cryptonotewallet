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

#include <QStyledItemDelegate>

#include "ICryptoNoteAdapter.h"
#include "IWalletAdapter.h"

namespace WalletGui {

class IAddressBookManager;

class TransactionsDelegate : public QStyledItemDelegate, public IWalletAdapterObserver, public ICryptoNoteAdapterObserver {
  Q_OBJECT
  Q_DISABLE_COPY(TransactionsDelegate)

public:
  explicit TransactionsDelegate(ICryptoNoteAdapter* _cryptoNoteAdapter, IAddressBookManager* _addressBookManager,QAbstractItemModel* _viewModel,
    QAbstractItemModel* _transactionsModel, QAbstractItemModel* _walletModel, QObject* _parent);
  ~TransactionsDelegate();

  // QStyledItemDelegate
  virtual void paint(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index) const override;
  virtual QSize sizeHint(const QStyleOptionViewItem& _option, const QModelIndex& _index) const override;

  // IWalletAdapterObserver
  Q_SLOT virtual void walletOpened() override;
  Q_SLOT virtual void walletOpenError(int _initStatus) override;
  Q_SLOT virtual void walletClosed() override;
  Q_SLOT virtual void passwordChanged() override;
  Q_SLOT virtual void synchronizationProgressUpdated(quint32 _current, quint32 _total) override;
  Q_SLOT virtual void synchronizationCompleted() override;
  Q_SLOT virtual void balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) override;
  Q_SLOT virtual void externalTransactionCreated(quintptr _transactionId, const FullTransactionInfo& _transaction) override;
  Q_SLOT virtual void transactionUpdated(quintptr _transactionId, const FullTransactionInfo& _transaction) override;

  // ICryptoNoteAdapterObserver
  Q_SLOT virtual void cryptoNoteAdapterInitCompleted(int _status) override;
  Q_SLOT virtual void cryptoNoteAdapterDeinitCompleted() override;

private:
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  IAddressBookManager* m_addressBookManager;
  QAbstractItemModel* m_transactionsModel;
  QAbstractItemModel* m_walletModel;

  int getTransfersColumnSize(const QModelIndex& _index) const;
  void drawTransfers(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index) const;
  void drawShowTransfersButton(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index) const;
};

}
