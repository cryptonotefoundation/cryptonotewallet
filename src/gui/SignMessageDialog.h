// Copyright (c) 2016-2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDialog>

namespace Ui {
class SignMessageDialog;
}

namespace WalletGui {

class SignMessageDialog : public QDialog {
    Q_OBJECT

public:
    SignMessageDialog(QWidget * _parent);
    void sign();
    void verify();
    void walletOpened();
    void walletClosed();
    ~SignMessageDialog();

private:
    QString m_address;

    Q_SLOT void messageChanged();
    Q_SLOT void verifyMessage();
    Q_SLOT void changeTitle(int _variant);

    QScopedPointer<Ui::SignMessageDialog> m_ui;
};

}
