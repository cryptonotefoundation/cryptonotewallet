// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Chavezcoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Chavezcoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Chavezcoin.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <QObject>

namespace WalletGui {

class IOptimizationManagerObserver {
public:
  virtual ~IOptimizationManagerObserver() {}
  virtual void fusionTransactionsVisibilityChanged(bool _isVisible) = 0;
};

class IOptimizationManager {
public:
  virtual ~IOptimizationManager() {}
  virtual bool isOptimizationEnabled() const = 0;
  virtual bool isFusionTransactionsVisible() const = 0;
  virtual bool isOptimizationTimeSetManually() const = 0;
  virtual quint64 getOptimizationThreshold() const = 0;
  virtual quint64 getOptimizationMixin() const = 0;
  virtual quint64 getOptimizationInterval() const = 0;
  virtual QTime getOptimizationStartTime() const = 0;
  virtual QTime getOptimizationStopTime() const = 0;

  virtual void setOptimizationEnabled(bool _on) = 0;
  virtual void setFusionTransactionsVisible(bool _isVisible) = 0;
  virtual void setOptimizationTimeSetManually(bool _on) = 0;
  virtual void setOptimizationThreshold(quint64 _threshold) = 0;
  virtual void setOptimizationMixin(quint64 _mixin) = 0;
  virtual void setOptimizationInterval(quint64 _interval) = 0;
  virtual void setOptimizationStartTime(const QTime& _startTime) = 0;
  virtual void setOptimizationStopTime(const QTime& _stopTime) = 0;

  virtual void addObserver(IOptimizationManagerObserver* _observer) = 0;
  virtual void removeObserver(IOptimizationManagerObserver* _observer) = 0;
};

}
