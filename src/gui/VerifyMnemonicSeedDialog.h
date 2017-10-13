// Copyright (c) 2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDialog>

namespace Ui {
class VerifyMnemonicSeedDialog;
}

namespace WalletGui {

class VerifyMnemonicSeedDialog : public QDialog {
    Q_OBJECT

public:
    VerifyMnemonicSeedDialog(QWidget * _parent);
    void walletClosed();
    ~VerifyMnemonicSeedDialog();

private:
    QScopedPointer<Ui::VerifyMnemonicSeedDialog> m_ui;
    int wordCount = 0;
    bool m_seedsMatch = false;

    QString getLanguageName();
    void initLanguages();

    Q_SLOT void onTextChanged();
    Q_SLOT void languageChanged();
    Q_SLOT void reject();


};

}
