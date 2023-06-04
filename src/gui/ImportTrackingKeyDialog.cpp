// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2017 The befrank developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QApplication>
#include <QFileDialog>

#include "ImportTrackingKeyDialog.h"

#include "ui_importtrackingkeydialog.h"

namespace WalletGui {

ImportTrackingKeyDialog::ImportTrackingKeyDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::ImportTrackingKeyDialog) {
  m_ui->setupUi(this);
}

ImportTrackingKeyDialog::~ImportTrackingKeyDialog() {
}

QString ImportTrackingKeyDialog::getKeyString() const {
  return m_ui->m_keyEdit->toPlainText().trimmed();
}

QString ImportTrackingKeyDialog::getFilePath() const {
  return m_ui->m_pathEdit->text().trimmed();
}

void ImportTrackingKeyDialog::selectPathClicked() {
  QString filePath = QFileDialog::getSaveFileName(this, tr("Tracking wallet file"),
#ifdef Q_OS_WIN
    QApplication::applicationDirPath(),
#else
    QDir::homePath(),
#endif
    tr("Tracking wallets (*.trackingwallet)")
    );

  if (!filePath.isEmpty() && !filePath.endsWith(".trackingwallet")) {
    filePath.append(".trackingwallet");
  }

  m_ui->m_pathEdit->setText(filePath);
}

}
