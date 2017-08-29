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

#include <QTime>
#include <QTimerEvent>

#include "OptimizationManager.h"
#include "INodeAdapter.h"
#include "Settings/Settings.h"

namespace WalletGui {

namespace {

const int CHECK_TIMER_INTERVAL = 1000;

}

OptimizationManager::OptimizationManager(ICryptoNoteAdapter* _cryptoNoteAdapter, QObject* _parent) : QObject(_parent),
  m_cryptoNoteAdapter(_cryptoNoteAdapter), m_checkTimerId(-1), m_optimizationTimerId(-1), m_currentOptimizationInterval(0), m_isSynchronized(false) {
  m_cryptoNoteAdapter->addObserver(this);
  m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
}

OptimizationManager::~OptimizationManager() {
}

bool OptimizationManager::isOptimizationEnabled() const {
  return Settings::instance().isOptimizationEnabled();
}

bool OptimizationManager::isFusionTransactionsVisible() const {
  return Settings::instance().isFusionTransactionsVisible();
}

bool OptimizationManager::isOptimizationTimeSetManually() const {
  return Settings::instance().isOptimizationTimeSetManually();
}

quint64 OptimizationManager::getOptimizationThreshold() const {
  return Settings::instance().getOptimizationThreshold();
}

quint64 OptimizationManager::getOptimizationMixin() const {
  return Settings::instance().getOptimizationMixin();
}

quint64 OptimizationManager::getOptimizationInterval() const {
  return Settings::instance().getOptimizationInterval();
}

QTime OptimizationManager::getOptimizationStartTime() const {
  return Settings::instance().getOptimizationStartTime();
}

QTime OptimizationManager::getOptimizationStopTime() const {
  return Settings::instance().getOptimizationStopTime();
}

void OptimizationManager::setOptimizationEnabled(bool _on) {
  Settings::instance().setOptimizationEnabled(_on);
}

void OptimizationManager::setFusionTransactionsVisible(bool _isVisible) {
  bool isVisibleChanged = (_isVisible != Settings::instance().isFusionTransactionsVisible());
  Settings::instance().setFusionTransactionsVisible(_isVisible);
  if (isVisibleChanged) {
    Q_EMIT fusionTransactionsVisibilityChangedSignal(_isVisible);
  }
}

void OptimizationManager::setOptimizationTimeSetManually(bool _on) {
  Settings::instance().setOptimizationTimeSetManually(_on);
}

void OptimizationManager::setOptimizationThreshold(quint64 _threshold) {
  Settings::instance().setOptimizationThreshold(_threshold);
}

void OptimizationManager::setOptimizationMixin(quint64 _mixin) {
  Settings::instance().setOptimizationMixin(_mixin);
}

void OptimizationManager::setOptimizationInterval(quint64 _interval) {
  Settings::instance().setOptimizationInterval(_interval);
}

void OptimizationManager::setOptimizationStartTime(const QTime& _startTime) {
  Settings::instance().setOptimizationStartTime(_startTime);
}

void OptimizationManager::setOptimizationStopTime(const QTime& _stopTime) {
  Settings::instance().setOptimizationStopTime(_stopTime);
}

void OptimizationManager::addObserver(IOptimizationManagerObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  Q_ASSERT(observer != nullptr);
  connect(this, SIGNAL(fusionTransactionsVisibilityChangedSignal(bool)), observer, SLOT(fusionTransactionsVisibilityChanged(bool)));
}

void OptimizationManager::removeObserver(IOptimizationManagerObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  Q_ASSERT(observer != nullptr);
  disconnect(this, SIGNAL(fusionTransactionsVisibilityChangedSignal(bool)), observer, SLOT(fusionTransactionsVisibilityChanged(bool)));
}

void OptimizationManager::walletOpened() {
  Q_ASSERT(m_checkTimerId == -1);
  m_checkTimerId = startTimer(CHECK_TIMER_INTERVAL);
}

void OptimizationManager::walletOpenError(int _initStatus) {
  // Do nothing
}

void OptimizationManager::walletClosed() {
  if (m_checkTimerId != -1) {
    killTimer(m_checkTimerId);
    m_checkTimerId = -1;
  }

  if (m_optimizationTimerId != -1) {
    killTimer(m_optimizationTimerId);
    m_optimizationTimerId = -1;
  }
}

void OptimizationManager::passwordChanged() {
  // Do nothing
}

void OptimizationManager::synchronizationProgressUpdated(quint32 _current, quint32 _total) {
  m_isSynchronized = false;
}

void OptimizationManager::synchronizationCompleted() {
  m_isSynchronized = true;
}

void OptimizationManager::balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) {
  // Do nothing
}

void OptimizationManager::externalTransactionCreated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void OptimizationManager::transactionUpdated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void OptimizationManager::cryptoNoteAdapterInitCompleted(int _status) {
  if (_status == 0) {
    m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
  }
}

void OptimizationManager::cryptoNoteAdapterDeinitCompleted() {
  // Do nothing
}

void OptimizationManager::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_checkTimerId) {
    checkOptimization();
  } else if (_event->timerId() == m_optimizationTimerId) {
    optimize();
  }

  QObject::timerEvent(_event);
}

void OptimizationManager::checkOptimization() {
  if (Settings::instance().isOptimizationEnabled()) {
    bool customTimeEnabled = Settings::instance().isOptimizationTimeSetManually();
    QTime startTime = Settings::instance().getOptimizationStartTime();
    QTime stopTime = Settings::instance().getOptimizationStopTime();
    QTime currentTime = QTime::currentTime();
    if (!customTimeEnabled || startTime == stopTime) {
      ensureStarted();
    } else if (stopTime > startTime) {
      if (currentTime >= startTime && currentTime < stopTime) {
        ensureStarted();
      } else {
        ensureStopped();
      }
    } else {
      if ((currentTime >= startTime && currentTime < QTime(0, 0)) || (currentTime >= QTime(0, 0) && currentTime < stopTime)) {
        ensureStarted();
      } else {
        ensureStopped();
      }
    }
  } else {
    ensureStopped();
  }
}

void OptimizationManager::optimize() {
  if (!m_isSynchronized) {
    return;
  }

  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  Q_ASSERT(walletAdapter->isOpen());
  if (walletAdapter->isTrackingWallet()) {
    return;
  }

  walletAdapter->createFusionTransaction(Settings::instance().getOptimizationThreshold(),
    Settings::instance().getOptimizationMixin(), walletAdapter->getAddress(0));
}

void OptimizationManager::ensureStarted() {
  if (m_optimizationTimerId != -1) {
    if (m_currentOptimizationInterval == Settings::instance().getOptimizationInterval()) {
      return;
    }
  }

  optimize();
  m_currentOptimizationInterval = Settings::instance().getOptimizationInterval();
  m_optimizationTimerId = startTimer(m_currentOptimizationInterval);
}

void OptimizationManager::ensureStopped() {
  if (m_optimizationTimerId == -1) {
    return;
  }

  killTimer(m_optimizationTimerId);
  m_currentOptimizationInterval = 0;
  m_optimizationTimerId = -1;
}

}
