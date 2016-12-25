// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QImage>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QClipboard>
#include <QApplication>
#include <QFileDialog>
#include "qrencode.h"
#include "MainWindow.h"
#include "QRLabel.h"

namespace WalletGui {

QRLabel::QRLabel(QWidget* _parent) : QLabel(_parent), contextMenu(0) {
    contextMenu = new QMenu();
    QAction* saveImageAction = new QAction(tr("&Save Image..."), this);
    connect(saveImageAction, SIGNAL(triggered()), this, SLOT(saveImage()));
    contextMenu->addAction(saveImageAction);
    QAction* copyImageAction = new QAction(tr("&Copy Image"), this);
    connect(copyImageAction, SIGNAL(triggered()), this, SLOT(copyImage()));
    contextMenu->addAction(copyImageAction);

    setWordWrap(true);
}

QRLabel::~QRLabel() {
}

void QRLabel::showQRCode(const QString& _dataString) {
  QRcode *qrcode = QRcode_encodeString(_dataString.toUtf8().constData(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);
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

QImage QRLabel::exportImage()
{
    if (!pixmap())
        return QImage();
    return pixmap()->toImage();
}

void QRLabel::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && pixmap()) {
        event->accept();
        QMimeData* mimeData = new QMimeData;
        mimeData->setImageData(exportImage());

        QDrag* drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->exec();
    } else {
        QLabel::mousePressEvent(event);
        Q_EMIT clicked();
    }
}

void QRLabel::saveImage()
{
    if (!pixmap())
        return;
    QString fn = QFileDialog::getSaveFileName(&MainWindow::instance(), tr("Save QR Code"), QDir::homePath(), "PNG (*.png)");
    if (!fn.isEmpty()) {
        exportImage().save(fn);
    }
}

void QRLabel::copyImage()
{
    if (!pixmap())
        return;
    QApplication::clipboard()->setImage(exportImage());
}

void QRLabel::contextMenuEvent(QContextMenuEvent* event)
{
    if (!pixmap())
        return;
    contextMenu->exec(event->globalPos());
}

}
