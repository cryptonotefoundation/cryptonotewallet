// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QLabel>
#include <QMenu>

namespace WalletGui {

class QRLabel : public QLabel {
  Q_OBJECT

public:
  QRLabel(QWidget* _parent);
  ~QRLabel();

  void showQRCode(const QString& _dataString);
  QImage exportImage();

Q_SIGNALS:
    void clicked();

public Q_SLOTS:
    void saveImage();
    void copyImage();

protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void contextMenuEvent(QContextMenuEvent* event);

private:
    QMenu* contextMenu;
    QString m_qrString;

};

}
