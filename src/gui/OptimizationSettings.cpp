// Copyright (c) 2018 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QtMath>

#include "ui_optimizationsettingsdialog.h"
#include "OptimizationSettings.h"
#include "CurrencyAdapter.h"
#include "WalletAdapter.h"
#include "MainWindow.h"
#include "Settings.h"

namespace Ui {
class OptimizationSettingsDialog;
}

namespace WalletGui {

namespace {

const int MIN_THRESHOLD_ORDER_VALUE = 0;
const int MAX_THRESHOLD_ORDER_VALUE = 4;
const int MIN_MIXIN_VALUE = 0;
const int MAX_MIXIN_VALUE = 5;
const quint64 MINUTE_MSECS = 1000 * 60;
const quint64 HOUR_MSECS = MINUTE_MSECS * 60;

}

OptimizationSettingsDialog::OptimizationSettingsDialog(QWidget* _parent) : QDialog(_parent),
    m_ui(new Ui::OptimizationSettingsDialog) {
    m_ui->setupUi(this);
    m_ui->m_thresholdSlider->setRange(MIN_THRESHOLD_ORDER_VALUE, MAX_THRESHOLD_ORDER_VALUE);
    m_ui->m_mixinSpin->setRange(MIN_MIXIN_VALUE, MAX_MIXIN_VALUE);
    m_ui->m_mixinSlider->setRange(MIN_MIXIN_VALUE, MAX_MIXIN_VALUE);
    initOptimizationPeriods();
    m_currencyMultiplier = CurrencyAdapter::instance().parseAmount("1");
    initThresholdCombo();
    load();
}

OptimizationSettingsDialog::~OptimizationSettingsDialog() {
}

void OptimizationSettingsDialog::load() {
  m_ui->m_enableOptimizationCheck->setChecked(Settings::instance().isOptimizationEnabled());
  m_ui->m_optimizationTimeCheck->setChecked(Settings::instance().isOptimizationTimeSetManually());
  m_ui->m_startTimeEdit->setTime(Settings::instance().getOptimizationStartTime());
  m_ui->m_stopTimeEdit->setTime(Settings::instance().getOptimizationStopTime());
  int index = m_ui->m_periodCombo->findData(Settings::instance().getOptimizationInterval());
  if (index == -1) {
    Settings::instance().setOptimizationInterval(1000 * 60 * 30);
  }

  m_ui->m_periodCombo->setCurrentIndex(m_ui->m_periodCombo->findData(Settings::instance().getOptimizationInterval()));
  quint64 coinCount = Settings::instance().getOptimizationThreshold() / m_currencyMultiplier;
  m_ui->m_thresholdSlider->setValue(std::log10(coinCount));
  m_ui->m_mixinSlider->setValue(Settings::instance().getOptimizationMixin());
  updateEstimateValue();
}

void OptimizationSettingsDialog::save() {
  Settings::instance().setOptimizationEnabled(m_ui->m_enableOptimizationCheck->isChecked());
  Settings::instance().setOptimizationTimeSetManually(m_ui->m_optimizationTimeCheck->isChecked());
  Settings::instance().setOptimizationStartTime(m_ui->m_startTimeEdit->time());
  Settings::instance().setOptimizationStopTime(m_ui->m_stopTimeEdit->time());
  Settings::instance().setOptimizationInterval(m_ui->m_periodCombo->currentData().value<quint64>());
  Settings::instance().setOptimizationThreshold(qPow(10, m_ui->m_thresholdSlider->value()) * m_currencyMultiplier);
  Settings::instance().setOptimizationMixin(m_ui->m_mixinSlider->value());

  accept();
}

void OptimizationSettingsDialog::initOptimizationPeriods() {
  m_ui->m_periodCombo->addItem(tr("30 minutes"), 30 * MINUTE_MSECS);
  m_ui->m_periodCombo->addItem(tr("1 hour"), HOUR_MSECS);
  m_ui->m_periodCombo->addItem(tr("1.5 hours"), HOUR_MSECS + 30 * MINUTE_MSECS);
  m_ui->m_periodCombo->addItem(tr("2 hours"), 2 * HOUR_MSECS);
  m_ui->m_periodCombo->addItem(tr("2.5 hours"), 2 * HOUR_MSECS + 30 * MINUTE_MSECS);
  m_ui->m_periodCombo->addItem(tr("3 hours"), 3 * HOUR_MSECS);
  m_ui->m_periodCombo->addItem(tr("3.5 hours"), 3 * HOUR_MSECS + 30 * MINUTE_MSECS);
  m_ui->m_periodCombo->addItem(tr("4 hours"), 4 * HOUR_MSECS);
  m_ui->m_periodCombo->addItem(tr("4.5 hours"), 4 * HOUR_MSECS + 30 * MINUTE_MSECS);
  m_ui->m_periodCombo->addItem(tr("5 hours"), 5 * HOUR_MSECS);
  m_ui->m_periodCombo->addItem(tr("5.5 hours"), 5 * HOUR_MSECS + 30 * MINUTE_MSECS);
  m_ui->m_periodCombo->addItem(tr("6 hours"), 6 * HOUR_MSECS);
}

void OptimizationSettingsDialog::initThresholdCombo() {
  for (int i = MIN_THRESHOLD_ORDER_VALUE; i <= MAX_THRESHOLD_ORDER_VALUE; ++i) {
    int value = qPow(10, i);
    m_ui->m_thresholdCombo->addItem(QString("%1 %2").arg(value).arg(CurrencyAdapter::instance().getCurrencyTicker().toUpper()), i);
  }
}

void OptimizationSettingsDialog::updateEstimateValue() {
  quintptr estimate = 0;
  if (WalletAdapter::instance().isOpen()) {
    estimate = WalletAdapter::instance().estimateFusion(qPow(10, m_ui->m_thresholdSlider->value()) * m_currencyMultiplier);
    if (estimate == 0) {
      m_ui->m_nonOptimizedOutputsLabel->hide();
      m_ui->m_nonOptimizedOutputsTextLabel->setText(tr("Wallet is currently optimized for this target"));
    } else {
      m_ui->m_nonOptimizedOutputsLabel->show();
      m_ui->m_nonOptimizedOutputsLabel->setText(QString::number(estimate));
      m_ui->m_nonOptimizedOutputsTextLabel->setText(tr(" outputs below selected target"));
    }
  } else {
    m_ui->m_nonOptimizedOutputsLabel->hide();
    m_ui->m_nonOptimizedOutputsTextLabel->setText(tr("Wallet is closed"));
  }
}

void OptimizationSettingsDialog::thresholdChanged(int _value) {
  if (m_ui->m_thresholdCombo->currentIndex() != _value) {
    m_ui->m_thresholdCombo->setCurrentIndex(_value);
  }

  if (m_ui->m_thresholdSlider->value() != _value) {
    m_ui->m_thresholdSlider->setValue(_value);
  }

  updateEstimateValue();
}

void OptimizationSettingsDialog::mixinChanged(int _mixin) {
  if (m_ui->m_mixinSlider->value() != _mixin) {
    m_ui->m_mixinSlider->setValue(_mixin);
  }

  if (m_ui->m_mixinSpin->value() != _mixin) {
    m_ui->m_mixinSpin->setValue(_mixin);
  }
}

}

