// Copyright (c) 2015-2017, The Bytecoin developers
// Copyright (c) 2017-2021, The Karbo developers
//
// This file is part of Karbo.
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

#include <QFileDialog>
#include <QBuffer>
#include <QUrl>

#include "MainWindow.h"
#include "QRCodeDialog.h"
#include "QRLabel.h"

#include "ui_qrcodedialog.h"

namespace WalletGui {

QRCodeDialog::QRCodeDialog(const QString& _title, const QString& _text, QWidget* _parent) : QDialog(_parent, static_cast<Qt::WindowFlags>(Qt::WindowCloseButtonHint)),
  m_ui(new Ui::QRCodeDialog) {
  m_ui->setupUi(this);
  setWindowTitle(_title);
  m_ui->m_qrCodeLabel->showQRCode(_text);
}

QRCodeDialog::~QRCodeDialog() {
}

void QRCodeDialog::saveQRcodeToFile() {
  QString fileName = QFileDialog::getSaveFileName(&MainWindow::instance(), tr("Save QR Code"), QDir::homePath(), "PNG (*.png)");
  if (!fileName.isEmpty()) {
    QPixmap qrcode = m_ui->m_qrCodeLabel->grab();
    QFile f(fileName);
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
      QByteArray ba;
      QBuffer buffer(&ba);
      buffer.open(QIODevice::WriteOnly);
      qrcode.save(&buffer, "PNG");
      f.write(ba);
      f.close();
    }
  }
}

}
