// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2021 Karbo developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QFileDialog>
#include <QMessageBox>

#include "OutputDetailsDialog.h"
#include "MainWindow.h"
#include "OutputsModel.h"

#include "ui_outputdetailsdialog.h"

namespace WalletGui {

OutputDetailsDialog::OutputDetailsDialog(const QModelIndex& _index, QWidget* _parent) : QDialog(_parent),
  m_ui(new Ui::OutputDetailsDialog)
{
  m_ui->setupUi(this);
  OutputsModel &m_OutputsModel = OutputsModel::instance();
  QModelIndex modelIndex = m_OutputsModel.index(_index.data(OutputsModel::ROLE_ROW).toInt(), 0);
  m_dataMapper.setModel(&m_OutputsModel);
  m_dataMapper.addMapping(m_ui->m_stateLabel, OutputsModel::COLUMN_STATE, "text");
  m_dataMapper.addMapping(m_ui->m_typeLabel, OutputsModel::COLUMN_TYPE, "text");
  m_dataMapper.addMapping(m_ui->m_keyLabel, OutputsModel::COLUMN_OUTPUT_KEY, "text");
  m_dataMapper.addMapping(m_ui->m_hashLabel, OutputsModel::COLUMN_TX_HASH, "text");
  m_dataMapper.addMapping(m_ui->m_txPublicKeyLabel, OutputsModel::COLUMN_TX_PUBLIC_KEY, "text");
  m_dataMapper.addMapping(m_ui->m_amountLabel, OutputsModel::COLUMN_AMOUNT, "text");
  m_dataMapper.addMapping(m_ui->m_indexLabel, OutputsModel::COLUMN_GLOBAL_OUTPUT_INDEX, "text");
  m_dataMapper.addMapping(m_ui->m_indexInTxLabel, OutputsModel::COLUMN_OUTPUT_IN_TRANSACTION, "text");
  m_dataMapper.addMapping(m_ui->m_spentHeightLabel, OutputsModel::COLUMN_SPENDING_BLOCK_HEIGHT, "text");
  m_dataMapper.addMapping(m_ui->m_timestampLabel, OutputsModel::COLUMN_TIMESTAMP, "text");
  m_dataMapper.addMapping(m_ui->m_spendingTxHashLabel, OutputsModel::COLUMN_SPENDING_TRANSACTION_HASH, "text");
  m_dataMapper.addMapping(m_ui->m_keyImageLabel, OutputsModel::COLUMN_KEY_IMAGE, "text");
  m_dataMapper.addMapping(m_ui->m_inputInTxLabel, OutputsModel::COLUMN_INPUT_IN_TRANSACTION, "text");
  m_dataMapper.setCurrentModelIndex(modelIndex);

  m_ui->m_prevButton->setEnabled(m_dataMapper.currentIndex() > 0);
  m_ui->m_nextButton->setEnabled(m_dataMapper.currentIndex() < OutputsModel::instance().rowCount() - 1);
}

OutputDetailsDialog::~OutputDetailsDialog() {
}

QModelIndex OutputDetailsDialog::getCurrentOutputIndex() const {
  return OutputsModel::instance().index(m_dataMapper.currentIndex(), 0);
}

void OutputDetailsDialog::prevClicked() {
  m_dataMapper.toPrevious();
  m_ui->m_prevButton->setEnabled(m_dataMapper.currentIndex() > 0);
  m_ui->m_nextButton->setEnabled(m_dataMapper.currentIndex() < OutputsModel::instance().rowCount() - 1);
}

void OutputDetailsDialog::nextClicked() {
  m_dataMapper.toNext();
  m_ui->m_prevButton->setEnabled(m_dataMapper.currentIndex() > 0);
  m_ui->m_nextButton->setEnabled(m_dataMapper.currentIndex() < OutputsModel::instance().rowCount() - 1);
}

}
