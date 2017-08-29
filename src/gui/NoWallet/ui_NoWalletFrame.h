/********************************************************************************
** Form generated from reading UI file 'NoWalletFrame.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOWALLETFRAME_H
#define UI_NOWALLETFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "Gui/Common/WalletBlueButton.h"
#include "Gui/Common/WalletOkButton.h"
#include "Gui/Common/WalletTextLabel.h"

QT_BEGIN_NAMESPACE

class Ui_NoWalletFrame
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    WalletGui::WalletHeaderLabel *m_welcomeLabel;
    QSpacerItem *verticalSpacer_4;
    QFrame *m_passwordFrame;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *m_passwordTextLabel;
    QLabel *m_walletNameLabel;
    QLabel *m_walletTextLabel;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *m_passwordEdit;
    WalletGui::WalletOkButton *m_okButton;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    WalletGui::WalletLargeBlueButton *m_createWalletButton;
    QSpacerItem *horizontalSpacer;
    WalletGui::WalletLargeBlueButton *m_openWalletButton;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QFrame *NoWalletFrame)
    {
        if (NoWalletFrame->objectName().isEmpty())
            NoWalletFrame->setObjectName(QStringLiteral("NoWalletFrame"));
        NoWalletFrame->resize(838, 660);
        NoWalletFrame->setFrameShape(QFrame::NoFrame);
        NoWalletFrame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(NoWalletFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 170, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        m_welcomeLabel = new WalletGui::WalletHeaderLabel(NoWalletFrame);
        m_welcomeLabel->setObjectName(QStringLiteral("m_welcomeLabel"));

        verticalLayout->addWidget(m_welcomeLabel, 0, Qt::AlignHCenter);

        verticalSpacer_4 = new QSpacerItem(20, 25, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_4);

        m_passwordFrame = new QFrame(NoWalletFrame);
        m_passwordFrame->setObjectName(QStringLiteral("m_passwordFrame"));
        m_passwordFrame->setMinimumSize(QSize(476, 0));
        m_passwordFrame->setMaximumSize(QSize(338, 16777215));
        m_passwordFrame->setFrameShape(QFrame::NoFrame);
        m_passwordFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(m_passwordFrame);
        verticalLayout_2->setSpacing(8);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        m_passwordTextLabel = new QLabel(m_passwordFrame);
        m_passwordTextLabel->setObjectName(QStringLiteral("m_passwordTextLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_passwordTextLabel->sizePolicy().hasHeightForWidth());
        m_passwordTextLabel->setSizePolicy(sizePolicy);
        m_passwordTextLabel->setStyleSheet(QLatin1String("[errorState=\"true\"] {\n"
"  color: #ef3131;\n"
"}"));

        horizontalLayout_4->addWidget(m_passwordTextLabel);

        m_walletNameLabel = new QLabel(m_passwordFrame);
        m_walletNameLabel->setObjectName(QStringLiteral("m_walletNameLabel"));

        horizontalLayout_4->addWidget(m_walletNameLabel);

        m_walletTextLabel = new QLabel(m_passwordFrame);
        m_walletTextLabel->setObjectName(QStringLiteral("m_walletTextLabel"));

        horizontalLayout_4->addWidget(m_walletTextLabel);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        m_passwordEdit = new QLineEdit(m_passwordFrame);
        m_passwordEdit->setObjectName(QStringLiteral("m_passwordEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_passwordEdit->sizePolicy().hasHeightForWidth());
        m_passwordEdit->setSizePolicy(sizePolicy1);
        m_passwordEdit->setStyleSheet(QLatin1String("[errorState=\"true\"] {\n"
"  border-color: #ef3131;\n"
"}"));
        m_passwordEdit->setEchoMode(QLineEdit::Password);

        horizontalLayout_5->addWidget(m_passwordEdit);

        m_okButton = new WalletGui::WalletOkButton(m_passwordFrame);
        m_okButton->setObjectName(QStringLiteral("m_okButton"));
        m_okButton->setEnabled(true);

        horizontalLayout_5->addWidget(m_okButton);


        verticalLayout_2->addLayout(horizontalLayout_5);


        verticalLayout->addWidget(m_passwordFrame, 0, Qt::AlignHCenter);

        verticalSpacer_2 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        m_createWalletButton = new WalletGui::WalletLargeBlueButton(NoWalletFrame);
        m_createWalletButton->setObjectName(QStringLiteral("m_createWalletButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_createWalletButton->sizePolicy().hasHeightForWidth());
        m_createWalletButton->setSizePolicy(sizePolicy2);
        m_createWalletButton->setMinimumSize(QSize(220, 50));
        m_createWalletButton->setMaximumSize(QSize(220, 50));

        horizontalLayout->addWidget(m_createWalletButton);

        horizontalSpacer = new QSpacerItem(32, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_openWalletButton = new WalletGui::WalletLargeBlueButton(NoWalletFrame);
        m_openWalletButton->setObjectName(QStringLiteral("m_openWalletButton"));
        sizePolicy2.setHeightForWidth(m_openWalletButton->sizePolicy().hasHeightForWidth());
        m_openWalletButton->setSizePolicy(sizePolicy2);
        m_openWalletButton->setMinimumSize(QSize(220, 50));
        m_openWalletButton->setMaximumSize(QSize(220, 50));

        horizontalLayout->addWidget(m_openWalletButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_3 = new QSpacerItem(20, 376, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        retranslateUi(NoWalletFrame);
        QObject::connect(m_okButton, SIGNAL(clicked()), NoWalletFrame, SLOT(okClicked()));
        QObject::connect(m_passwordEdit, SIGNAL(textChanged(QString)), NoWalletFrame, SLOT(passwordChanged(QString)));
        QObject::connect(m_passwordEdit, SIGNAL(returnPressed()), m_okButton, SLOT(click()));

        m_okButton->setDefault(true);


        QMetaObject::connectSlotsByName(NoWalletFrame);
    } // setupUi

    void retranslateUi(QFrame *NoWalletFrame)
    {
        NoWalletFrame->setWindowTitle(QApplication::translate("NoWalletFrame", "Frame", 0));
        m_welcomeLabel->setText(QApplication::translate("NoWalletFrame", "Welcome to Intensecoin Wallet!", 0));
        m_passwordTextLabel->setText(QApplication::translate("NoWalletFrame", "PASSWORD FOR", 0));
        m_walletNameLabel->setText(QString());
        m_walletTextLabel->setText(QApplication::translate("NoWalletFrame", "WALLET", 0));
        m_okButton->setText(QApplication::translate("NoWalletFrame", "OK", 0));
        m_createWalletButton->setText(QApplication::translate("NoWalletFrame", "Create wallet", 0));
        m_openWalletButton->setText(QApplication::translate("NoWalletFrame", "Open wallet", 0));
    } // retranslateUi

};

namespace Ui {
    class NoWalletFrame: public Ui_NoWalletFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NOWALLETFRAME_H
