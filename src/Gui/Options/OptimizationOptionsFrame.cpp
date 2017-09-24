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

#include <QtMath>

#include "OptimizationOptionsFrame.h"
#include "Settings/Settings.h"
#include "ICryptoNoteAdapter.h"
#include "INodeAdapter.h"
#include "IWalletAdapter.h"
#include "IOptimizationManager.h"
#include "Style/Style.h"
#include "ui_OptimizationOptionsFrame.h"

#include "Wallet/IFusionManager.h"

namespace WalletGui {

namespace {

const int MIN_THRESHOLD_ORDER_VALUE = 0;
const int MAX_THRESHOLD_ORDER_VALUE = 6;
const int MIN_MIXIN_VALUE = 0;
const int MAX_MIXIN_VALUE = 5;
const quint64 MINUTE_MSECS = 1000 * 60;
const quint64 HOUR_MSECS = MINUTE_MSECS * 60;
const char OPTIMIZATION_OPTIONS_STYLE_SHEET_TEMPLATE[] =
  "WalletGui--OptimizationOptionsFrame {"
    "background: #ffffff;"
  "}"

  "WalletGui--OptimizationOptionsFrame QTimeEdit,"
  "WalletGui--OptimizationOptionsFrame QSpinBox,"
  "WalletGui--OptimizationOptionsFrame QComboBox,"
  "WalletGui--OptimizationOptionsFrame QLineEdit {"
    "min-height: 25px;"
    "max-height: 25px;"
  "}"

  "WalletGui--OptimizationOptionsFrame QTimeEdit {"
    "min-width: 0px;"
  "}"

  "WalletGui--OptimizationOptionsFrame #m_enableOptimizationCheck {"
    "font-size: %fontSizeLarge%;"
    "font-weight: bold;"
  "}"

  "WalletGui--OptimizationOptionsFrame #m_enableOptimizationCheck::indicator {"
    "width: 20px;"
    "height: 20px;"
  "}"

  "WalletGui--OptimizationOptionsFrame #m_nonOptimizedOutputsLabel {"
    "font-weight: bold;"
  "}";

}

OptimizationOptionsFrame::OptimizationOptionsFrame(QWidget* _parent) : QFrame(_parent),
  m_ui(new Ui::OptimizationOptionsFrame), m_cryptoNoteAdapter(nullptr), m_optimizationManager(nullptr), m_currencyMultiplier(0) {
  m_ui->setupUi(this);
  m_ui->m_thresholdSlider->setRange(MIN_THRESHOLD_ORDER_VALUE, MAX_THRESHOLD_ORDER_VALUE);
  m_ui->m_mixinSpin->setRange(MIN_MIXIN_VALUE, MAX_MIXIN_VALUE);
  m_ui->m_mixinSlider->setRange(MIN_MIXIN_VALUE, MAX_MIXIN_VALUE);
  initOptimizationPeriods();
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(OPTIMIZATION_OPTIONS_STYLE_SHEET_TEMPLATE));
}

OptimizationOptionsFrame::~OptimizationOptionsFrame() {
}

void OptimizationOptionsFrame::load() {
  Q_ASSERT(m_optimizationManager != nullptr);
  m_ui->m_enableOptimizationCheck->setDisabled(true);
  m_ui->m_enableOptimizationCheck->setChecked(m_optimizationManager->isOptimizationEnabled());
  m_ui->m_optimizationTimeCheck->setChecked(m_optimizationManager->isOptimizationTimeSetManually());
  m_ui->m_startTimeEdit->setTime(m_optimizationManager->getOptimizationStartTime());
  m_ui->m_stopTimeEdit->setTime(m_optimizationManager->getOptimizationStopTime());
  int index = m_ui->m_periodCombo->findData(m_optimizationManager->getOptimizationInterval());
  if (index == -1) {
    m_optimizationManager->setOptimizationInterval(1000 * 60 * 30);
  }

  m_ui->m_periodCombo->setCurrentIndex(m_ui->m_periodCombo->findData(m_optimizationManager->getOptimizationInterval()));
  quint64 coinCount = m_optimizationManager->getOptimizationThreshold() / m_currencyMultiplier;
  m_ui->m_thresholdSlider->setValue(std::log10(coinCount));
  m_ui->m_mixinSlider->setValue(m_optimizationManager->getOptimizationMixin());
  m_ui->m_showFusionTransactionsCheck->setChecked(m_optimizationManager->isFusionTransactionsVisible());
  updateEstimateValue();
}

void OptimizationOptionsFrame::save() {
  m_optimizationManager->setOptimizationEnabled(m_ui->m_enableOptimizationCheck->isChecked());
  m_optimizationManager->setOptimizationTimeSetManually(m_ui->m_optimizationTimeCheck->isChecked());
  m_optimizationManager->setOptimizationStartTime(m_ui->m_startTimeEdit->time());
  m_optimizationManager->setOptimizationStopTime(m_ui->m_stopTimeEdit->time());
  m_optimizationManager->setOptimizationInterval(m_ui->m_periodCombo->currentData().value<quint64>());
  m_optimizationManager->setOptimizationThreshold(qPow(10, m_ui->m_thresholdSlider->value()) * m_currencyMultiplier);
  m_optimizationManager->setOptimizationMixin(m_ui->m_mixinSlider->value());
  m_optimizationManager->setFusionTransactionsVisible(m_ui->m_showFusionTransactionsCheck->isChecked());
}

void OptimizationOptionsFrame::setData(const QVariantMap& _data) {
  // Do nothing
}

bool OptimizationOptionsFrame::isEnabled() const {
  return true;
}

bool OptimizationOptionsFrame::needToRestartApplication() const {
  return false;
}

bool OptimizationOptionsFrame::canAccept() const {
  return true;
}

void OptimizationOptionsFrame::setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) {
  Q_ASSERT(_cryptoNoteAdapter != nullptr);
  m_cryptoNoteAdapter = _cryptoNoteAdapter;
  m_currencyMultiplier = m_cryptoNoteAdapter->parseAmount("1");
  initThresholdCombo();
}

void OptimizationOptionsFrame::setOptimizationManager(IOptimizationManager* _optimizationManager) {
  m_optimizationManager = _optimizationManager;
}

void OptimizationOptionsFrame::initOptimizationPeriods() {
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

void OptimizationOptionsFrame::initThresholdCombo() {
  for (int i = MIN_THRESHOLD_ORDER_VALUE; i <= MAX_THRESHOLD_ORDER_VALUE; ++i) {
    int value = qPow(10, i);
    m_ui->m_thresholdCombo->addItem(QString("%1 %2").arg(value).arg(m_cryptoNoteAdapter->getCurrencyTicker().toUpper()), i);
  }
}

void OptimizationOptionsFrame::updateEstimateValue() {
  quintptr estimate = 0;
  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  if (walletAdapter->isOpen()) {
    estimate =  walletAdapter->getOutputsToOptimizeCount(qPow(10, m_ui->m_thresholdSlider->value()) * m_currencyMultiplier);
    if (estimate == 0) {
      m_ui->m_nonOptimizedOutputsLabel->hide();
      m_ui->m_nonOptimizedOutputsTextLabel->setText(tr("Wallet is currently optimized for this target"));
    } else {
      m_ui->m_nonOptimizedOutputsLabel->show();
      m_ui->m_nonOptimizedOutputsLabel->setText(QString::number(estimate));
      m_ui->m_nonOptimizedOutputsTextLabel->setText(tr("outputs below selected target"));
    }
  } else {
    m_ui->m_nonOptimizedOutputsLabel->hide();
    m_ui->m_nonOptimizedOutputsTextLabel->setText(tr("Wallet is closed"));
  }
}

void OptimizationOptionsFrame::thresholdChanged(int _value) {
  if (m_ui->m_thresholdCombo->currentIndex() != _value) {
    m_ui->m_thresholdCombo->setCurrentIndex(_value);
  }

  if (m_ui->m_thresholdSlider->value() != _value) {
    m_ui->m_thresholdSlider->setValue(_value);
  }

  updateEstimateValue();
}

void OptimizationOptionsFrame::mixinChanged(int _mixin) {
  if (m_ui->m_mixinSlider->value() != _mixin) {
    m_ui->m_mixinSlider->setValue(_mixin);
  }

  if (m_ui->m_mixinSpin->value() != _mixin) {
    m_ui->m_mixinSpin->setValue(_mixin);
  }
}

}
