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
#include "IApplicationEventHandler.h"
#include "IDonationManager.h"
#include "IMiningManager.h"

class QAbstractButton;
class QDataWidgetMapper;

namespace Ui {
class MiningFrame;
}

namespace WalletGui {

class IApplicationEventHandler;
class WalletLargeBlueButton;

class MiningFrame : public QFrame, public IWalletUiItem, public IDonationManagerObserver, public IMinerManagerObserver,
  public IApplicationEventHandlerObserver {
  Q_OBJECT
  Q_DISABLE_COPY(MiningFrame)

public:
  explicit MiningFrame(QWidget* _parent);
  ~MiningFrame();

  // IWalletUiItem
  virtual void setCryptoNoteAdapter(ICryptoNoteAdapter *_cryptoNoteAdapter) override;
  virtual void setApplicationEventHandler(IApplicationEventHandler* _applicationEventHandler) override;
  virtual void setDonationManager(IDonationManager* _donationManager) override;
  virtual void setMiningManager(IMiningManager* _miningManager) override;
  virtual void setMainWindow(QWidget *_mainWindow) override;
  virtual void setMinerModel(QAbstractItemModel* _model) override;

  // IDonationManagerObserver
  Q_SLOT virtual void donationManagerOpened() override;
  Q_SLOT virtual void donationManagerClosed() override;
  Q_SLOT virtual void donationMiningEnabled(bool _on) override;
  Q_SLOT virtual void donationMiningAddressChanged(const QString& _address) override;
  Q_SLOT virtual void donationMiningAmountChanged(int _amount) override;
  Q_SLOT virtual void donationChangeEnabled(bool _on) override;
  Q_SLOT virtual void donationChangeAddressChanged(const QString& _address) override;
  Q_SLOT virtual void donationChangeAmountChanged(int _amount) override;

  // IMinerManagerObserver
  Q_SLOT virtual void minersLoaded() override;
  Q_SLOT virtual void minersUnloaded() override;
  Q_SLOT virtual void miningStarted() override;
  Q_SLOT virtual void miningStopped() override;
  Q_SLOT virtual void activeMinerChanged(quintptr _minerIndex) override;
  Q_SLOT virtual void schedulePolicyChanged(int _schedulePolicy) override;
  Q_SLOT virtual void cpuCoreCountChanged(quint32 _cpuCoreCount) override;
  Q_SLOT virtual void minerAdded(quintptr _minerIndex) override;
  Q_SLOT virtual void minerRemoved(quintptr _minerIndex) override;
  Q_SLOT virtual void stateChanged(quintptr _minerIndex, int _newState) override;
  Q_SLOT virtual void hashRateChanged(quintptr _minerIndex, quint32 _hashRate) override;
  Q_SLOT virtual void alternateHashRateChanged(quintptr _minerIndex, quint32 _hashRate) override;
  Q_SLOT virtual void difficultyChanged(quintptr _minerIndex, quint32 _difficulty) override;
  Q_SLOT virtual void goodShareCountChanged(quintptr _minerIndex, quint32 _goodShareCount) override;
  Q_SLOT virtual void goodAlternateShareCountChanged(quintptr _minerIndex, quint32 _goodShareCount) override;
  Q_SLOT virtual void badShareCountChanged(quintptr _minerIndex, quint32 _badShareCount) override;
  Q_SLOT virtual void connectionErrorCountChanged(quintptr _minerIndex, quint32 _connectionErrorCount) override;
  Q_SLOT virtual void lastConnectionErrorTimeChanged(quintptr _minerIndex, const QDateTime& _lastConnectionErrorTime) override;

  // IApplicationEventHandlerObserver
  Q_SLOT virtual void urlReceived(const QUrl& _url) override;
  Q_SLOT virtual void screenLocked() override;
  Q_SLOT virtual void screenUnlocked() override;

protected:
  void resizeEvent(QResizeEvent* _event) override;

private:
  QScopedPointer<Ui::MiningFrame> m_ui;
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  IMiningManager* m_miningManager;
  IDonationManager* m_donationManager;
  QWidget* m_mainWindow;
  QAbstractItemModel* m_minerModel;
  QDataWidgetMapper* m_dataMapper;
  WalletLargeBlueButton* m_restorePoolListButton;


  void initCpuCoresCombo();
  void restoreDefaultPoolsClicked();
  void startMining();
  void stopMining();
  void showRestoreButton();

  Q_SLOT void addPoolClicked();
  Q_SLOT void cpuCoreCountChangedByUser(int _cpuCoreCount);
  Q_SLOT void deleteClicked(const QModelIndex& _index);
  Q_SLOT void miningOnLockedScreenChecked(bool _enabled);
  Q_SLOT void newPoolUrlChanged();
  Q_SLOT void schedulePolicyChanged(QAbstractButton* _button, bool _checked);
  Q_SLOT void startMiningClicked(bool _on);
};

}
