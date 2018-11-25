// Copyright (c) 2018 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma  once

#include <QDialog>

namespace Ui {
class OptimizationSettingsDialog;
}

namespace WalletGui {

//class NodeModel;

class OptimizationSettingsDialog : public QDialog {
  Q_OBJECT
  Q_DISABLE_COPY(OptimizationSettingsDialog)

public:
  OptimizationSettingsDialog(QWidget* _parent);
  ~OptimizationSettingsDialog();

  void load();

private:
  QScopedPointer<Ui::OptimizationSettingsDialog> m_ui;

  quint64 m_currencyMultiplier;

  void initOptimizationPeriods();
  void initThresholdCombo();
  void updateEstimateValue();

  Q_SLOT void thresholdChanged(int _value);
  Q_SLOT void mixinChanged(int _mixin);
  Q_SLOT void save();

};

}

