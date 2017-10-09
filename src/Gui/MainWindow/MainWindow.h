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

#include <QMainWindow>

#include "IApplicationEventHandler.h"
#include "ICryptoNoteAdapter.h"
#include "IWalletAdapter.h"

class QActionGroup;
class QDataWidgetMapper;
class QSplashScreen;
class QAbstractButton;
class QAbstractItemModel;
class QSessionManager;

namespace Ui {
  class MainWindow;
}

namespace WalletGui {

class IAddressBookManager;
class IDonationManager;
class IOptimizationManager;
class IMiningManager;
class INewsReader;

class MainWindow : public QMainWindow, public IWalletAdapterObserver, public IApplicationEventHandlerObserver,
  public ICryptoNoteAdapterObserver {
  Q_OBJECT
  Q_DISABLE_COPY(MainWindow)

public:
  MainWindow(ICryptoNoteAdapter* _cryptoNoteAdapter, IAddressBookManager* _addressBookManager,
    IDonationManager* _donationManager, IOptimizationManager* _optimizationManager, IMiningManager* _miningManager,
    IApplicationEventHandler* _applicationEventHandler, INewsReader* _blogReader,
    const QString& _styleSheetTemplate, QWidget* _parent);
  virtual ~MainWindow();

  bool eventFilter(QObject* _object, QEvent* _event) override;

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

  // IApplicationEventHandlerObserver
  Q_SLOT virtual void urlReceived(const QUrl& _url) override;
  Q_SLOT virtual void screenLocked() override;
  Q_SLOT virtual void screenUnlocked() override;

  // ICryptoNoteAdapterObserver
  Q_SLOT virtual void cryptoNoteAdapterInitCompleted(int _status) override;
  Q_SLOT virtual void cryptoNoteAdapterDeinitCompleted() override;

protected:
  void changeEvent(QEvent* _event) override;
  void closeEvent(QCloseEvent* _event) override;

private:
  QScopedPointer<Ui::MainWindow> m_ui;
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  IAddressBookManager* m_addressBookManager;
  IDonationManager* m_donationManager;
  IOptimizationManager* m_optimizationManager;
  IMiningManager* m_miningManager;
  IApplicationEventHandler* m_applicationEventHandler;
  INewsReader* m_blogReader;
  QAbstractItemModel* m_nodeStateModel;
  QAbstractItemModel* m_walletStateModel;
  QAbstractItemModel* m_transactionsModel;
  QAbstractItemModel* m_sortedTranactionsModel;
  QAbstractItemModel* m_fusionTranactionsFilterModel;
  QAbstractItemModel* m_addressBookModel;
  QAbstractItemModel* m_sortedAddressBookModel;
  QAbstractItemModel* m_blockChainModel;
  QAbstractItemModel* m_transactionPoolModel;
  QAbstractItemModel* m_minerModel;
  QMenu* m_recentWalletsMenu;
  QList<QAction*> m_recentWalletsActionList;
  QAction* m_addRecipientAction;
  QString m_styleSheetTemplate;
  QDataWidgetMapper* m_walletStateMapper;
  QMovie* m_syncMovie;

  void createRecentWalletMenu();
  void updateRecentWalletActions();
  void openRecentWallet();
  void themeChanged();
  void setOpenedState();
  void setClosedState();
  void addRecipientTriggered();
  void commitData(QSessionManager& _manager);
  void walletStateModelDataChanged(const QModelIndex& _topLeft, const QModelIndex& _bottomRight, const QVector<int>& _roles);

  Q_SLOT void createWallet();
  Q_SLOT void openWallet();
  Q_SLOT void backupWallet();
  Q_SLOT void saveWalletKeys();
  Q_SLOT void resetWallet();
  Q_SLOT void encryptWallet();
  Q_SLOT void removePendingTx();
  Q_SLOT void exportKey();
  Q_SLOT void exportTrackingKey();
  Q_SLOT void importKey();
  Q_SLOT void aboutQt();
  Q_SLOT void about();
  Q_SLOT void copyAddress();
  Q_SLOT void copyBalance();
  Q_SLOT void setStartOnLoginEnabled(bool _enable);
  Q_SLOT void setMinimizeToTrayEnabled(bool _enable);
  Q_SLOT void setCloseToTrayEnabled(bool _enable);
  Q_SLOT void showPreferences();
  Q_SLOT void communityForumTriggered();
  Q_SLOT void reportIssueTriggered();

Q_SIGNALS:
  void reinitCryptoNoteAdapterSignal();
};

}
