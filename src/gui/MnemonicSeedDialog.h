// Copyright (c) 2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDialog>

namespace Ui {
class MnemonicSeedDialog;
}

namespace WalletGui {

class MnemonicSeedDialog : public QDialog {
    Q_OBJECT

public:
    MnemonicSeedDialog(QWidget * _parent);
    void walletOpened();
    void walletClosed();
    ~MnemonicSeedDialog();

private:
    QScopedPointer<Ui::MnemonicSeedDialog> m_ui;

    void initLanguages();
    QString getLanguageName();

    Q_SLOT void languageChanged();
};

}
