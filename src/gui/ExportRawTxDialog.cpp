// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "ExportRawTxDialog.h"
#include "ui_exportrawtxdialog.h"
#include <QClipboard>
#include <QFileDialog>
#include <QTextStream>
#include <Common/StringTools.h>
#include "CurrencyAdapter.h"
#include "WalletAdapter.h"
#include "MainWindow.h"
#include <boost/utility/value_init.hpp>

namespace WalletGui {

ExportRawTransactionDialog::ExportRawTransactionDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::ExportRawTransactionDialog) {
  m_ui->setupUi(this);
}

ExportRawTransactionDialog::~ExportRawTransactionDialog() {
  m_ui->m_txEdit->clear();
}

void ExportRawTransactionDialog::setTransaction(const QString& _transaction) {
  m_ui->m_txEdit->setText(_transaction);
}

void ExportRawTransactionDialog::copyTx() {
  QApplication::clipboard()->setText(m_ui->m_txEdit->toPlainText());
}

void ExportRawTransactionDialog::saveTxToFile() {
  QString fileName = QFileDialog::getSaveFileName(&MainWindow::instance(), tr("Save transaction to..."), QDir::homePath(), tr("Raw hex transaction (*.txt)"));
  if (!fileName.isEmpty()) {
    QFile f(fileName);
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
      QTextStream outputStream(&f);
      outputStream << m_ui->m_txEdit->toPlainText();
      f.close();
    }
  }
}

}
