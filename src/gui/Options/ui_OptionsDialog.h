/********************************************************************************
** Form generated from reading UI file 'OptionsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONSDIALOG_H
#define UI_OPTIONSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include "Gui/Common/WalletCancelButton.h"
#include "Gui/Common/WalletOkButton.h"
#include "Gui/Options/ConnectionOptionsFrame.h"
#include "Gui/Options/DonationOptionsFrame.h"
#include "Gui/Options/OptimizationOptionsFrame.h"
#include "Gui/Options/PrivacyFrame.h"

QT_BEGIN_NAMESPACE

class Ui_OptionsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *m_optionsTabWidget;
    WalletGui::ConnectionOptionsFrame *m_connectionTab;
    WalletGui::OptimizationOptionsFrame *m_optimizationTab;
    WalletGui::DonationOptionsFrame *m_donationTab;
    WalletGui::PrivacyFrame *m_privacyTab;
    QFrame *m_buttonFrame;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_6;
    QLabel *m_warningLabel;
    QSpacerItem *horizontalSpacer;
    WalletGui::WalletCancelButton *m_cancelButton;
    WalletGui::WalletOkButton *m_okButton;

    void setupUi(QDialog *OptionsDialog)
    {
        if (OptionsDialog->objectName().isEmpty())
            OptionsDialog->setObjectName(QStringLiteral("OptionsDialog"));
        OptionsDialog->resize(715, 500);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(OptionsDialog->sizePolicy().hasHeightForWidth());
        OptionsDialog->setSizePolicy(sizePolicy);
        OptionsDialog->setMinimumSize(QSize(715, 500));
        OptionsDialog->setMaximumSize(QSize(715, 500));
        verticalLayout = new QVBoxLayout(OptionsDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 12, 0, 0);
        m_optionsTabWidget = new QTabWidget(OptionsDialog);
        m_optionsTabWidget->setObjectName(QStringLiteral("m_optionsTabWidget"));
        m_connectionTab = new WalletGui::ConnectionOptionsFrame();
        m_connectionTab->setObjectName(QStringLiteral("m_connectionTab"));
        m_optionsTabWidget->addTab(m_connectionTab, QString());
        m_optimizationTab = new WalletGui::OptimizationOptionsFrame();
        m_optimizationTab->setObjectName(QStringLiteral("m_optimizationTab"));
        m_optionsTabWidget->addTab(m_optimizationTab, QString());
        m_donationTab = new WalletGui::DonationOptionsFrame();
        m_donationTab->setObjectName(QStringLiteral("m_donationTab"));
        m_optionsTabWidget->addTab(m_donationTab, QString());
        m_privacyTab = new WalletGui::PrivacyFrame();
        m_privacyTab->setObjectName(QStringLiteral("m_privacyTab"));
        m_optionsTabWidget->addTab(m_privacyTab, QString());

        verticalLayout->addWidget(m_optionsTabWidget);

        m_buttonFrame = new QFrame(OptionsDialog);
        m_buttonFrame->setObjectName(QStringLiteral("m_buttonFrame"));
        m_buttonFrame->setFrameShape(QFrame::NoFrame);
        m_buttonFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(m_buttonFrame);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(25, 0, 25, 25);
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);

        m_warningLabel = new QLabel(m_buttonFrame);
        m_warningLabel->setObjectName(QStringLiteral("m_warningLabel"));
        m_warningLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(m_warningLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        m_cancelButton = new WalletGui::WalletCancelButton(m_buttonFrame);
        m_cancelButton->setObjectName(QStringLiteral("m_cancelButton"));

        horizontalLayout_2->addWidget(m_cancelButton);

        m_okButton = new WalletGui::WalletOkButton(m_buttonFrame);
        m_okButton->setObjectName(QStringLiteral("m_okButton"));

        horizontalLayout_2->addWidget(m_okButton);


        verticalLayout->addWidget(m_buttonFrame);


        retranslateUi(OptionsDialog);
        QObject::connect(m_okButton, SIGNAL(clicked()), OptionsDialog, SLOT(accept()));
        QObject::connect(m_cancelButton, SIGNAL(clicked()), OptionsDialog, SLOT(reject()));

        m_optionsTabWidget->setCurrentIndex(0);
        m_okButton->setDefault(true);


        QMetaObject::connectSlotsByName(OptionsDialog);
    } // setupUi

    void retranslateUi(QDialog *OptionsDialog)
    {
        OptionsDialog->setWindowTitle(QApplication::translate("OptionsDialog", "Preferences", 0));
        m_optionsTabWidget->setTabText(m_optionsTabWidget->indexOf(m_connectionTab), QApplication::translate("OptionsDialog", "Connection", 0));
        m_optionsTabWidget->setTabText(m_optionsTabWidget->indexOf(m_optimizationTab), QApplication::translate("OptionsDialog", "Wallet optimization", 0));
        m_optionsTabWidget->setTabText(m_optionsTabWidget->indexOf(m_donationTab), QApplication::translate("OptionsDialog", "Donations", 0));
        m_optionsTabWidget->setTabText(m_optionsTabWidget->indexOf(m_privacyTab), QApplication::translate("OptionsDialog", "Privacy", 0));
        m_warningLabel->setText(QApplication::translate("OptionsDialog", "Attention! Restart application to apply changes.", 0));
        m_cancelButton->setText(QApplication::translate("OptionsDialog", "Cancel", 0));
        m_okButton->setText(QApplication::translate("OptionsDialog", "Save", 0));
    } // retranslateUi

};

namespace Ui {
    class OptionsDialog: public Ui_OptionsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONSDIALOG_H
