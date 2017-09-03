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

#include "IOptionsPage.h"
#include "Application/IWalletUiItem.h"

namespace Ui {
  class OptimizationOptionsFrame;
}

namespace WalletGui {

class OptimizationOptionsFrame : public QFrame, public IOptionsPage, public IWalletUiItem {
  Q_OBJECT
  Q_DISABLE_COPY(OptimizationOptionsFrame)

public:
  explicit OptimizationOptionsFrame(QWidget* _parent = nullptr);
  ~OptimizationOptionsFrame();

  // IOptionsPage
  virtual void load() override;
  virtual void save() override;
  virtual void setData(const QVariantMap& _data) override;
  virtual bool isEnabled() const override;
  virtual bool needToRestartApplication() const override;
  virtual bool canAccept() const override;

  // IWalletUiItem
  virtual void setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) override;
  virtual void setOptimizationManager(IOptimizationManager* _optimizationManager) override;

private:
  QScopedPointer<Ui::OptimizationOptionsFrame> m_ui;
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  IOptimizationManager* m_optimizationManager;
  quint64 m_currencyMultiplier;

  void initOptimizationPeriods();
  void initThresholdCombo();
  void updateEstimateValue();
  Q_SLOT void thresholdChanged(int _value);
  Q_SLOT void mixinChanged(int _mixin);

Q_SIGNALS:
  void showRestartWarningSignal(bool _show) override;
  void disableAcceptButtonSignal(bool _disable) override;
};

}
