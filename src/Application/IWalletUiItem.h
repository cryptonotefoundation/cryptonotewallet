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

class QAbstractItemModel;
class QWidget;

namespace WalletGui {

class ICryptoNoteAdapter;
class IAddressBookManager;
class IDonationManager;
class IOptimizationManager;
class IMiningManager;
class IApplicationEventHandler;
class INewsReader;

class IWalletUiItem {
public:
  virtual ~IWalletUiItem() {}
  virtual void setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) {}
  virtual void setAddressBookManager(IAddressBookManager* _addressBookManager) {}
  virtual void setOptimizationManager(IOptimizationManager* _optimizationManager) {}
  virtual void setDonationManager(IDonationManager* _donationManager) {}
  virtual void setMiningManager(IMiningManager* _miningManager) {}
  virtual void setApplicationEventHandler(IApplicationEventHandler* _applicationEventHandler) {}
  virtual void setBlogReader(INewsReader* _blogReader) {}
  virtual void setMainWindow(QWidget* _mainWindow) {}
  virtual void setTransactionsModel(QAbstractItemModel* _model) {}
  virtual void setSortedTransactionsModel(QAbstractItemModel* _model) {}
  virtual void setNodeStateModel(QAbstractItemModel* _model) {}
  virtual void setWalletStateModel(QAbstractItemModel* _model) {}
  virtual void setAddressBookModel(QAbstractItemModel* _model) {}
  virtual void setSortedAddressBookModel(QAbstractItemModel* _model) {}
  virtual void setBlockChainModel(QAbstractItemModel* _model) {}
  virtual void setTransactionPoolModel(QAbstractItemModel* _model) {}
  virtual void setMinerModel(QAbstractItemModel* _model) {}
  virtual void updateStyle() {}
};

}
