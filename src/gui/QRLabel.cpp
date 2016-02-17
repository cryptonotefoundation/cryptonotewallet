// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QImage>

#include "libqrencode/qrencode.h"

#include "QRLabel.h"

namespace WalletGui {

QRLabel::QRLabel(QWidget* _parent) : QLabel(_parent) {
}

QRLabel::~QRLabel() {
}

void QRLabel::showQRCode(const QString& _dataString) {
  QRcode *qrcode = QRcode_encodeString(_dataString.toStdString().c_str(), 1, QR_ECLEVEL_L, QR_MODE_8, 0);
  if (qrcode == nullptr) {
    return;
  }

  QImage qrCodeImage = QImage(qrcode->width + 8, qrcode->width + 8, QImage::Format_RGB32);
  qrCodeImage.fill(0xffffff);
  unsigned char *p = qrcode->data;
  for (int y = 0; y < qrcode->width; y++) {
    for (int x = 0; x < qrcode->width; x++) {
      qrCodeImage.setPixel(x + 4, y + 4, ((*p & 1) ? 0x0 : 0xffffff));
      p++;
    }
  }

  QRcode_free(qrcode);
  setPixmap(QPixmap::fromImage(qrCodeImage).scaled(300, 300));
  setEnabled(true);
}

}
