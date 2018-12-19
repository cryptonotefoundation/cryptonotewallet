// Copyright (c) 2016-2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDialog>

namespace Ui {
class GetBalanceProofDialog;
}

namespace WalletGui {

class GetBalanceProofDialog : public QDialog {
    Q_OBJECT

public:
    GetBalanceProofDialog(QWidget * _parent);
    void walletBalanceUpdated();
    ~GetBalanceProofDialog();

private:
    quint64 m_amount;
    QString m_message;
    QString m_proof;

    Q_SLOT void genProof();
    Q_SLOT void copyProof();
    Q_SLOT void saveProof();

    QScopedPointer<Ui::GetBalanceProofDialog> m_ui;
};

}
