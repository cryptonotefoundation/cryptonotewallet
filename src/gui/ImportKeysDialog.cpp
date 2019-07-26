// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2019 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QApplication>
#include <QFileDialog>
#include <QStandardPaths>

#include "ImportKeysDialog.h"

#include "ui_importkeysdialog.h"

namespace WalletGui {

ImportKeysDialog::ImportKeysDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::ImportKeysDialog) {
  m_ui->setupUi(this);
}

ImportKeysDialog::~ImportKeysDialog() {
}

QString ImportKeysDialog::getViewKeyString() const {
  return m_ui->m_viewKeyEdit->text().trimmed();
}

QString ImportKeysDialog::getSpendKeyString() const {
  return m_ui->m_spendKeyEdit->text().trimmed();
}

QString ImportKeysDialog::getFilePath() const {
  return m_ui->m_pathEdit->text().trimmed();
}

void ImportKeysDialog::selectPathClicked() {
  QString filePath = QFileDialog::getSaveFileName(this, tr("Wallet file"),
#ifdef Q_OS_WIN
    //QApplication::applicationDirPath(),
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
#else
    QDir::homePath(),
#endif
    tr("Wallets (*.wallet)")
    );

  if (!filePath.isEmpty() && !filePath.endsWith(".wallet")) {
    filePath.append(".wallet");
  }

  m_ui->m_pathEdit->setText(filePath);
}

}
