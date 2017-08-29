/********************************************************************************
** Form generated from reading UI file 'NewAddressDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWADDRESSDIALOG_H
#define UI_NEWADDRESSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include "Gui/Common/ClearableLineEdit.h"
#include "Gui/Common/WalletCancelButton.h"
#include "Gui/Common/WalletGrayCheckBox.h"
#include "Gui/Common/WalletOkButton.h"
#include "Gui/Common/WalletTextLabel.h"

QT_BEGIN_NAMESPACE

class Ui_NewAddressDialog
{
public:
    QGridLayout *gridLayout;
    WalletGui::WalletNormalGrayTextLabel *m_labelTextLabel;
    WalletGui::WalletCancelButton *m_cancelButton;
    WalletGui::ClearableLineEdit *m_labelEdit;
    QSpacerItem *verticalSpacer;
    WalletGui::WalletOkButton *m_okButton;
    WalletGui::WalletNormalGrayTextLabel *m_addressTextLabel;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer;
    WalletGui::ClearableLineEdit *m_addressEdit;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    WalletGui::WalletGrayCheckBox *m_donationCheck;
    WalletGui::WalletSmallGrayTextLabel *m_donationWarningLabel;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *NewAddressDialog)
    {
        if (NewAddressDialog->objectName().isEmpty())
            NewAddressDialog->setObjectName(QStringLiteral("NewAddressDialog"));
        NewAddressDialog->resize(584, 255);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(NewAddressDialog->sizePolicy().hasHeightForWidth());
        NewAddressDialog->setSizePolicy(sizePolicy);
        NewAddressDialog->setMinimumSize(QSize(584, 255));
        NewAddressDialog->setMaximumSize(QSize(584, 255));
        gridLayout = new QGridLayout(NewAddressDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setVerticalSpacing(5);
        gridLayout->setContentsMargins(30, 30, 30, 30);
        m_labelTextLabel = new WalletGui::WalletNormalGrayTextLabel(NewAddressDialog);
        m_labelTextLabel->setObjectName(QStringLiteral("m_labelTextLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_labelTextLabel->sizePolicy().hasHeightForWidth());
        m_labelTextLabel->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(m_labelTextLabel, 0, 0, 1, 4);

        m_cancelButton = new WalletGui::WalletCancelButton(NewAddressDialog);
        m_cancelButton->setObjectName(QStringLiteral("m_cancelButton"));

        gridLayout->addWidget(m_cancelButton, 8, 1, 1, 1);

        m_labelEdit = new WalletGui::ClearableLineEdit(NewAddressDialog);
        m_labelEdit->setObjectName(QStringLiteral("m_labelEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_labelEdit->sizePolicy().hasHeightForWidth());
        m_labelEdit->setSizePolicy(sizePolicy2);
        m_labelEdit->setStyleSheet(QLatin1String("[errorState=\"true\"] {\n"
"  border-color: #ef3131;\n"
"}"));
        m_labelEdit->setEchoMode(QLineEdit::Normal);

        gridLayout->addWidget(m_labelEdit, 1, 0, 1, 4);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 7, 0, 1, 4);

        m_okButton = new WalletGui::WalletOkButton(NewAddressDialog);
        m_okButton->setObjectName(QStringLiteral("m_okButton"));
        m_okButton->setEnabled(false);

        gridLayout->addWidget(m_okButton, 8, 2, 1, 2);

        m_addressTextLabel = new WalletGui::WalletNormalGrayTextLabel(NewAddressDialog);
        m_addressTextLabel->setObjectName(QStringLiteral("m_addressTextLabel"));
        sizePolicy1.setHeightForWidth(m_addressTextLabel->sizePolicy().hasHeightForWidth());
        m_addressTextLabel->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(m_addressTextLabel, 3, 0, 1, 4);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_3, 5, 0, 1, 4);

        horizontalSpacer = new QSpacerItem(338, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 8, 0, 1, 1);

        m_addressEdit = new WalletGui::ClearableLineEdit(NewAddressDialog);
        m_addressEdit->setObjectName(QStringLiteral("m_addressEdit"));
        m_addressEdit->setStyleSheet(QLatin1String("[errorState=\"true\"] {\n"
"  border-color: #ef3131;\n"
"}"));
        m_addressEdit->setEchoMode(QLineEdit::Normal);

        gridLayout->addWidget(m_addressEdit, 4, 0, 1, 4);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 2, 0, 1, 4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        m_donationCheck = new WalletGui::WalletGrayCheckBox(NewAddressDialog);
        m_donationCheck->setObjectName(QStringLiteral("m_donationCheck"));

        horizontalLayout->addWidget(m_donationCheck);

        m_donationWarningLabel = new WalletGui::WalletSmallGrayTextLabel(NewAddressDialog);
        m_donationWarningLabel->setObjectName(QStringLiteral("m_donationWarningLabel"));
        m_donationWarningLabel->setMinimumSize(QSize(0, 0));
        m_donationWarningLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        m_donationWarningLabel->setIndent(0);

        horizontalLayout->addWidget(m_donationWarningLabel);

        horizontalSpacer_2 = new QSpacerItem(40, 5, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout, 6, 0, 1, 4);

        QWidget::setTabOrder(m_labelEdit, m_addressEdit);
        QWidget::setTabOrder(m_addressEdit, m_okButton);

        retranslateUi(NewAddressDialog);
        QObject::connect(m_okButton, SIGNAL(clicked()), NewAddressDialog, SLOT(accept()));
        QObject::connect(m_cancelButton, SIGNAL(clicked()), NewAddressDialog, SLOT(reject()));
        QObject::connect(m_addressEdit, SIGNAL(textChanged(QString)), NewAddressDialog, SLOT(validateAddress(QString)));
        QObject::connect(m_labelEdit, SIGNAL(textChanged(QString)), NewAddressDialog, SLOT(validateLabel(QString)));
        QObject::connect(m_donationCheck, SIGNAL(stateChanged(int)), NewAddressDialog, SLOT(donationCheckStateChanged(int)));

        m_okButton->setDefault(true);


        QMetaObject::connectSlotsByName(NewAddressDialog);
    } // setupUi

    void retranslateUi(QDialog *NewAddressDialog)
    {
        NewAddressDialog->setWindowTitle(QApplication::translate("NewAddressDialog", "New contact", 0));
        m_labelTextLabel->setText(QApplication::translate("NewAddressDialog", "LABEL", 0));
        m_cancelButton->setText(QApplication::translate("NewAddressDialog", "Cancel", 0));
        m_okButton->setText(QApplication::translate("NewAddressDialog", "OK", 0));
        m_addressTextLabel->setText(QApplication::translate("NewAddressDialog", "ADDRESS", 0));
        m_donationCheck->setText(QApplication::translate("NewAddressDialog", "DONATION ADDRESS", 0));
        m_donationWarningLabel->setText(QApplication::translate("NewAddressDialog", "Donations to this address will be stopped.", 0));
    } // retranslateUi

};

namespace Ui {
    class NewAddressDialog: public Ui_NewAddressDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWADDRESSDIALOG_H
