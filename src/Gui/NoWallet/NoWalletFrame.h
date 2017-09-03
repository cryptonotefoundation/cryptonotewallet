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
#include "IWalletAdapter.h"
#include "ICryptoNoteAdapter.h"

namespace Ui {
class NoWalletFrame;
}

namespace WalletGui {

class NoWalletFrame : public QFrame, public IWalletUiItem, public IWalletAdapterObserver, public ICryptoNoteAdapterObserver {
  Q_OBJECT
  Q_DISABLE_COPY(NoWalletFrame)

public:
  explicit NoWalletFrame(QWidget* _parent);
  ~NoWalletFrame();

  void openWallet(const QString& _walletFile, const QString& _legacyKeysFile);

  // IWalletUiItem
  virtual void setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) override;
  virtual void setMainWindow(QWidget *_mainWindow) override;

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
  QScopedPointer<Ui::NoWalletFrame> m_ui;
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  QString m_walletFile;
  QString m_legacyKeysFile;
  QTimer* m_timer = NULL;

  Q_INVOKABLE void loadWallet(const QString& _password);
  Q_INVOKABLE void loadLegacyKeys(const QString& _password);
  void setPasswordError(bool _isErrorOccured);
  
  Q_SLOT void okClicked();
  Q_SLOT void passwordChanged(const QString& _password);

private slots:
  void attemptToProceedWithOk();

Q_SIGNALS:
  void createWalletClickedSignal();
  void openWalletClickedSignal();
};

}
