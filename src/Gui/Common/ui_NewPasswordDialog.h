/********************************************************************************
** Form generated from reading UI file 'NewPasswordDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWPASSWORDDIALOG_H
#define UI_NEWPASSWORDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include "Gui/Common/WalletCancelButton.h"
#include "Gui/Common/WalletOkButton.h"

QT_BEGIN_NAMESPACE

class Ui_NewPasswordDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *m_passwordTextLabel;
    QLineEdit *m_passwordEdit;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    WalletGui::WalletCancelButton *m_cancelButton;
    QSpacerItem *verticalSpacer_2;
    WalletGui::WalletOkButton *m_okButton;
    QLineEdit *m_passwordConfirmationEdit;
    QLabel *m_passwordConfirmationTextLabel;

    void setupUi(QDialog *NewPasswordDialog)
    {
        if (NewPasswordDialog->objectName().isEmpty())
            NewPasswordDialog->setObjectName(QStringLiteral("NewPasswordDialog"));
        NewPasswordDialog->resize(346, 212);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(NewPasswordDialog->sizePolicy().hasHeightForWidth());
        NewPasswordDialog->setSizePolicy(sizePolicy);
        NewPasswordDialog->setMinimumSize(QSize(346, 212));
        NewPasswordDialog->setMaximumSize(QSize(346, 212));
        gridLayout = new QGridLayout(NewPasswordDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setVerticalSpacing(5);
        gridLayout->setContentsMargins(30, 30, 30, 30);
        m_passwordTextLabel = new QLabel(NewPasswordDialog);
        m_passwordTextLabel->setObjectName(QStringLiteral("m_passwordTextLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_passwordTextLabel->sizePolicy().hasHeightForWidth());
        m_passwordTextLabel->setSizePolicy(sizePolicy1);
        m_passwordTextLabel->setStyleSheet(QLatin1String("[errorState=\"true\"] {\n"
"  color: #ef3131;\n"
"}"));

        gridLayout->addWidget(m_passwordTextLabel, 0, 0, 1, 4);

        m_passwordEdit = new QLineEdit(NewPasswordDialog);
        m_passwordEdit->setObjectName(QStringLiteral("m_passwordEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_passwordEdit->sizePolicy().hasHeightForWidth());
        m_passwordEdit->setSizePolicy(sizePolicy2);
        m_passwordEdit->setStyleSheet(QLatin1String("[errorState=\"true\"] {\n"
"  border-color: #ef3131;\n"
"}"));
        m_passwordEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(m_passwordEdit, 1, 0, 1, 4);

        horizontalSpacer = new QSpacerItem(138, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 7, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 6, 0, 1, 4);

        m_cancelButton = new WalletGui::WalletCancelButton(NewPasswordDialog);
        m_cancelButton->setObjectName(QStringLiteral("m_cancelButton"));

        gridLayout->addWidget(m_cancelButton, 7, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 2, 0, 1, 4);

        m_okButton = new WalletGui::WalletOkButton(NewPasswordDialog);
        m_okButton->setObjectName(QStringLiteral("m_okButton"));
        m_okButton->setEnabled(false);

        gridLayout->addWidget(m_okButton, 7, 2, 1, 2);

        m_passwordConfirmationEdit = new QLineEdit(NewPasswordDialog);
        m_passwordConfirmationEdit->setObjectName(QStringLiteral("m_passwordConfirmationEdit"));
        m_passwordConfirmationEdit->setStyleSheet(QLatin1String("[errorState=\"true\"] {\n"
"  border-color: #ef3131;\n"
"}"));
        m_passwordConfirmationEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(m_passwordConfirmationEdit, 5, 0, 1, 4);

        m_passwordConfirmationTextLabel = new QLabel(NewPasswordDialog);
        m_passwordConfirmationTextLabel->setObjectName(QStringLiteral("m_passwordConfirmationTextLabel"));
        sizePolicy1.setHeightForWidth(m_passwordConfirmationTextLabel->sizePolicy().hasHeightForWidth());
        m_passwordConfirmationTextLabel->setSizePolicy(sizePolicy1);
        m_passwordConfirmationTextLabel->setStyleSheet(QLatin1String("[errorState=\"true\"] {\n"
"  color: #ef3131;\n"
"}"));

        gridLayout->addWidget(m_passwordConfirmationTextLabel, 4, 0, 1, 4);

        QWidget::setTabOrder(m_passwordEdit, m_passwordConfirmationEdit);
        QWidget::setTabOrder(m_passwordConfirmationEdit, m_okButton);

        retranslateUi(NewPasswordDialog);
        QObject::connect(m_okButton, SIGNAL(clicked()), NewPasswordDialog, SLOT(accept()));
        QObject::connect(m_cancelButton, SIGNAL(clicked()), NewPasswordDialog, SLOT(reject()));
        QObject::connect(m_passwordEdit, SIGNAL(textChanged(QString)), NewPasswordDialog, SLOT(checkPassword(QString)));
        QObject::connect(m_passwordConfirmationEdit, SIGNAL(textChanged(QString)), NewPasswordDialog, SLOT(checkPassword(QString)));

        m_okButton->setDefault(true);


        QMetaObject::connectSlotsByName(NewPasswordDialog);
    } // setupUi

    void retranslateUi(QDialog *NewPasswordDialog)
    {
        NewPasswordDialog->setWindowTitle(QApplication::translate("NewPasswordDialog", "Enter password", 0));
        m_passwordTextLabel->setText(QApplication::translate("NewPasswordDialog", "PASSWORD", 0));
        m_cancelButton->setText(QApplication::translate("NewPasswordDialog", "Cancel", 0));
        m_okButton->setText(QApplication::translate("NewPasswordDialog", "OK", 0));
        m_passwordConfirmationTextLabel->setText(QApplication::translate("NewPasswordDialog", "CONFIRM", 0));
    } // retranslateUi

};

namespace Ui {
    class NewPasswordDialog: public Ui_NewPasswordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWPASSWORDDIALOG_H
