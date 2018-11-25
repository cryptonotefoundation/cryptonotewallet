// Copyright (c) 2015-2017, The Bytecoin developers
// Copyright (c) 2018, The Karbo developers
//
// This file is part of Karbovanets.
//
// Karbovanets is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Karbovanets is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Karbovanets.  If not, see <http://www.gnu.org/licenses/>.

#include <QTime>
#include <QTimerEvent>

#include "OptimizationManager.h"
#include "WalletAdapter.h"
#include "gui/WalletEvents.h"
#include "NodeAdapter.h"
#include "Settings.h"

namespace WalletGui {

namespace {

const int CHECK_TIMER_INTERVAL = 1000;

}

OptimizationManager::OptimizationManager(QObject* _parent) : QObject(_parent),
  m_checkTimerId(-1), m_optimizationTimerId(-1), m_currentOptimizationInterval(0), m_isSynchronized(false) {
    connect(&WalletAdapter::instance(), &WalletAdapter::walletInitCompletedSignal, this, &OptimizationManager::walletOpened);
    connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &OptimizationManager::walletClosed);
    connect(&WalletAdapter::instance(), &WalletAdapter::walletSynchronizationProgressUpdatedSignal, this, &OptimizationManager::synchronizationProgressUpdated, Qt::QueuedConnection);
    connect(&WalletAdapter::instance(), &WalletAdapter::walletSynchronizationCompletedSignal, this, &OptimizationManager::synchronizationCompleted, Qt::QueuedConnection);
}

OptimizationManager::~OptimizationManager() {
}

void OptimizationManager::walletOpened() {
  Q_ASSERT(m_checkTimerId == -1);
  m_checkTimerId = startTimer(CHECK_TIMER_INTERVAL);
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

void OptimizationManager::synchronizationProgressUpdated() {
  m_isSynchronized = false;
}

void OptimizationManager::synchronizationCompleted() {
  m_isSynchronized = true;
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

  Q_ASSERT(WalletAdapter::instance().isOpen());
  if (Settings::instance().isTrackingMode()) {
    return;
  }

  size_t fusionReadyCount = WalletAdapter::instance().estimateFusion(CurrencyAdapter::instance().getMinimumFee());
  const size_t MAX_FUSION_OUTPUT_COUNT = 4;
  const quint64 mixin = Settings::instance().getOptimizationMixin();
  size_t estimatedFusionInputsCount = CurrencyAdapter::instance().getCurrency().getApproximateMaximumInputCount(CurrencyAdapter::instance().getCurrency().fusionTxMaxSize(), MAX_FUSION_OUTPUT_COUNT, mixin);
  if (estimatedFusionInputsCount < CurrencyAdapter::instance().getCurrency().fusionTxMinInputCount()) {
    // Mixin is too big
    return;
  }
  std::list<CryptoNote::TransactionOutputInformation> fusionInputs = WalletAdapter::instance().getFusionTransfersToSend(Settings::instance().getOptimizationThreshold(), CurrencyAdapter::instance().getCurrency().fusionTxMinInputCount(), estimatedFusionInputsCount);
  if (fusionInputs.size() < CurrencyAdapter::instance().getCurrency().fusionTxMinInputCount()) {
    //nothing to optimize
    return;
  }
  quint64 amount = 0;
  WalletAdapter::instance().sendFusionTransaction(fusionInputs, 0, "", mixin);
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
