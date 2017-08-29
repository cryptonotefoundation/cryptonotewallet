/********************************************************************************
** Form generated from reading UI file 'P2pBindPortErrorDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_P2PBINDPORTERRORDIALOG_H
#define UI_P2PBINDPORTERRORDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include "Gui/Common/WalletOkButton.h"

QT_BEGIN_NAMESPACE

class Ui_P2pBindPortErrorDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *m_portErrorLabel;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *m_acceptRadio;
    QLabel *label_2;
    QSpinBox *m_newPortSpin;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *m_rejectRadio;
    QLabel *label;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    WalletGui::WalletOkButton *m_okButton;

    void setupUi(QDialog *P2pBindPortErrorDialog)
    {
        if (P2pBindPortErrorDialog->objectName().isEmpty())
            P2pBindPortErrorDialog->setObjectName(QStringLiteral("P2pBindPortErrorDialog"));
        P2pBindPortErrorDialog->resize(506, 166);
        verticalLayout = new QVBoxLayout(P2pBindPortErrorDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        m_portErrorLabel = new QLabel(P2pBindPortErrorDialog);
        m_portErrorLabel->setObjectName(QStringLiteral("m_portErrorLabel"));

        verticalLayout->addWidget(m_portErrorLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        m_acceptRadio = new QRadioButton(P2pBindPortErrorDialog);
        m_acceptRadio->setObjectName(QStringLiteral("m_acceptRadio"));
        m_acceptRadio->setChecked(true);

        horizontalLayout_2->addWidget(m_acceptRadio);

        label_2 = new QLabel(P2pBindPortErrorDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        m_newPortSpin = new QSpinBox(P2pBindPortErrorDialog);
        m_newPortSpin->setObjectName(QStringLiteral("m_newPortSpin"));
        m_newPortSpin->setMinimum(1);
        m_newPortSpin->setMaximum(65535);

        horizontalLayout_2->addWidget(m_newPortSpin);

        label_3 = new QLabel(P2pBindPortErrorDialog);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_2->addWidget(label_3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        m_rejectRadio = new QRadioButton(P2pBindPortErrorDialog);
        m_rejectRadio->setObjectName(QStringLiteral("m_rejectRadio"));

        horizontalLayout_3->addWidget(m_rejectRadio);

        label = new QLabel(P2pBindPortErrorDialog);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_okButton = new WalletGui::WalletOkButton(P2pBindPortErrorDialog);
        m_okButton->setObjectName(QStringLiteral("m_okButton"));

        horizontalLayout->addWidget(m_okButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(P2pBindPortErrorDialog);
        QObject::connect(m_okButton, SIGNAL(clicked()), P2pBindPortErrorDialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(P2pBindPortErrorDialog);
    } // setupUi

    void retranslateUi(QDialog *P2pBindPortErrorDialog)
    {
        P2pBindPortErrorDialog->setWindowTitle(QApplication::translate("P2pBindPortErrorDialog", "Warning", 0));
        m_portErrorLabel->setText(QApplication::translate("P2pBindPortErrorDialog", "P2P port %1 is in use on this PC. To continue please choose:", 0));
        m_acceptRadio->setText(QString());
        label_2->setText(QApplication::translate("P2pBindPortErrorDialog", "Bind to", 0));
        label_3->setText(QApplication::translate("P2pBindPortErrorDialog", "port", 0));
        m_rejectRadio->setText(QString());
        label->setText(QApplication::translate("P2pBindPortErrorDialog", "Exit", 0));
        m_okButton->setText(QApplication::translate("P2pBindPortErrorDialog", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class P2pBindPortErrorDialog: public Ui_P2pBindPortErrorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_P2PBINDPORTERRORDIALOG_H
