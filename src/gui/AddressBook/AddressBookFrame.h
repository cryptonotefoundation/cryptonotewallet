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

class QLabel;
class QPersistentModelIndex;

namespace Ui {
class AddressBookFrame;
}

namespace WalletGui {

class AddressBookDelegate;

class AddressBookFrame : public QFrame, public IWalletUiItem {
  Q_OBJECT
  Q_DISABLE_COPY(AddressBookFrame)

public:
  explicit AddressBookFrame(QWidget* _parent);
  ~AddressBookFrame();

  // IWalletUiItem
  virtual void setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) override;
  virtual void setAddressBookManager(IAddressBookManager* _addressBookManager) override;
  virtual void setDonationManager(IDonationManager *_donationManager) override;
  virtual void setMainWindow(QWidget* _mainWindow) override;
  virtual void setAddressBookModel(QAbstractItemModel* _model) override;
  virtual void setSortedAddressBookModel(QAbstractItemModel* _model) override;

protected:
  void resizeEvent(QResizeEvent* _event) override;

private:
  QScopedPointer<Ui::AddressBookFrame> m_ui;
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  IAddressBookManager* m_addressBookManager;
  IDonationManager* m_donationManager;
  QWidget* m_mainWindow;
  QAbstractItemModel* m_addressBookModel;
  QAbstractItemModel* m_sortedAddressBookModel;
  QLabel* m_helperLabel;
  AddressBookDelegate* m_addressBookDelegate;

  void rowsInserted(const QModelIndex& _parent, int _first, int _last);
  void rowsRemoved(const QModelIndex& _parent, int _first, int _last);
  void sendToClicked(const QString& _address);

  Q_SLOT void addClicked();
  Q_SLOT void editClicked(const QPersistentModelIndex& _index);
  Q_SLOT void deleteClicked(const QPersistentModelIndex& _index);
  Q_SLOT void contextMenu(const QPoint& _pos);

Q_SIGNALS:
  void sendToSignal(const QString& _address);
};

}
