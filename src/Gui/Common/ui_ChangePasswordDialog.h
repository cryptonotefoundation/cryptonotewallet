/********************************************************************************
** Form generated from reading UI file 'ChangePasswordDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANGEPASSWORDDIALOG_H
#define UI_CHANGEPASSWORDDIALOG_H

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

class Ui_ChangePasswordDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *m_newPasswordTextLabel;
    QLabel *m_newPasswordConfirmationTextLabel;
    QLabel *m_oldPasswordTextLabel;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;
    QLineEdit *m_newPasswordEdit;
    QSpacerItem *verticalSpacer_3;
    QLineEdit *m_oldPasswordEdit;
    QLineEdit *m_newPasswordConfirmationEdit;
    QSpacerItem *horizontalSpacer;
    WalletGui::WalletCancelButton *m_cancelButton;
    WalletGui::WalletOkButton *m_okButton;

    void setupUi(QDialog *ChangePasswordDialog)
    {
        if (ChangePasswordDialog->objectName().isEmpty())
            ChangePasswordDialog->setObjectName(QStringLiteral("ChangePasswordDialog"));
        ChangePasswordDialog->resize(338, 279);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ChangePasswordDialog->sizePolicy().hasHeightForWidth());
        ChangePasswordDialog->setSizePolicy(sizePolicy);
        ChangePasswordDialog->setMinimumSize(QSize(338, 279));
        ChangePasswordDialog->setMaximumSize(QSize(338, 279));
        gridLayout = new QGridLayout(ChangePasswordDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setVerticalSpacing(5);
        gridLayout->setContentsMargins(30, 30, 30, 30);
        m_newPasswordTextLabel = new QLabel(ChangePasswordDialog);
        m_newPasswordTextLabel->setObjectName(QStringLiteral("m_newPasswordTextLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_newPasswordTextLabel->sizePolicy().hasHeightForWidth());
        m_newPasswordTextLabel->setSizePolicy(sizePolicy1);
        m_newPasswordTextLabel->setStyleSheet(QLatin1String("[errorState=\"true\"] {\n"
"  color: #ef3131;\n"
"}"));

        gridLayout->addWidget(m_newPasswordTextLabel, 3, 0, 1, 4);

        m_newPasswordConfirmationTextLabel = new QLabel(ChangePasswordDialog);
        m_newPasswordConfirmationTextLabel->setObjectName(QStringLiteral("m_newPasswordConfirmationTextLabel"));
        sizePolicy1.setHeightForWidth(m_newPasswordConfirmationTextLabel->sizePolicy().hasHeightForWidth());
        m_newPasswordConfirmationTextLabel->setSizePolicy(sizePolicy1);
        m_newPasswordConfirmationTextLabel->setStyleSheet(QLatin1String("[errorState=\"true\"] {\n"
"  color: #ef3131;\n"
"}"));

        gridLayout->addWidget(m_newPasswordConfirmationTextLabel, 6, 0, 1, 4);

        m_oldPasswordTextLabel = new QLabel(ChangePasswordDialog);
        m_oldPasswordTextLabel->setObjectName(QStringLiteral("m_oldPasswordTextLabel"));
        m_oldPasswordTextLabel->setStyleSheet(QLatin1String("[errorState=\"true\"] {\n"
"  color: #ef3131;\n"
"}"));

        gridLayout->addWidget(m_oldPasswordTextLabel, 0, 0, 1, 4);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 8, 0, 1, 4);

        verticalSpacer_2 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 2, 0, 1, 4);

        m_newPasswordEdit = new QLineEdit(ChangePasswordDialog);
        m_newPasswordEdit->setObjectName(QStringLiteral("m_newPasswordEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_newPasswordEdit->sizePolicy().hasHeightForWidth());
        m_newPasswordEdit->setSizePolicy(sizePolicy2);
        m_newPasswordEdit->setStyleSheet(QLatin1String("[errorState=\"true\"] {\n"
"  border-color: #ef3131;\n"
"}"));
        m_newPasswordEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(m_newPasswordEdit, 4, 0, 1, 4);

        verticalSpacer_3 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_3, 5, 0, 1, 4);

        m_oldPasswordEdit = new QLineEdit(ChangePasswordDialog);
        m_oldPasswordEdit->setObjectName(QStringLiteral("m_oldPasswordEdit"));
        m_oldPasswordEdit->setStyleSheet(QLatin1String("[errorState=\"true\"] {\n"
"  border-color: #ef3131;\n"
"}"));
        m_oldPasswordEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(m_oldPasswordEdit, 1, 0, 1, 4);

        m_newPasswordConfirmationEdit = new QLineEdit(ChangePasswordDialog);
        m_newPasswordConfirmationEdit->setObjectName(QStringLiteral("m_newPasswordConfirmationEdit"));
        m_newPasswordConfirmationEdit->setStyleSheet(QLatin1String("[errorState=\"true\"] {\n"
"  border-color: #ef3131;\n"
"}"));
        m_newPasswordConfirmationEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(m_newPasswordConfirmationEdit, 7, 0, 1, 4);

        horizontalSpacer = new QSpacerItem(130, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 9, 0, 1, 1);

        m_cancelButton = new WalletGui::WalletCancelButton(ChangePasswordDialog);
        m_cancelButton->setObjectName(QStringLiteral("m_cancelButton"));

        gridLayout->addWidget(m_cancelButton, 9, 1, 1, 1);

        m_okButton = new WalletGui::WalletOkButton(ChangePasswordDialog);
        m_okButton->setObjectName(QStringLiteral("m_okButton"));
        m_okButton->setEnabled(false);

        gridLayout->addWidget(m_okButton, 9, 2, 1, 2);

        QWidget::setTabOrder(m_oldPasswordEdit, m_newPasswordEdit);
        QWidget::setTabOrder(m_newPasswordEdit, m_newPasswordConfirmationEdit);
        QWidget::setTabOrder(m_newPasswordConfirmationEdit, m_okButton);

        retranslateUi(ChangePasswordDialog);
        QObject::connect(m_newPasswordEdit, SIGNAL(textChanged(QString)), ChangePasswordDialog, SLOT(checkPassword(QString)));
        QObject::connect(m_newPasswordConfirmationEdit, SIGNAL(textChanged(QString)), ChangePasswordDialog, SLOT(checkPassword(QString)));
        QObject::connect(m_okButton, SIGNAL(clicked()), ChangePasswordDialog, SLOT(accept()));
        QObject::connect(m_cancelButton, SIGNAL(clicked()), ChangePasswordDialog, SLOT(reject()));
        QObject::connect(m_oldPasswordEdit, SIGNAL(textChanged(QString)), ChangePasswordDialog, SLOT(passwordChanged(QString)));

        m_okButton->setDefault(true);


        QMetaObject::connectSlotsByName(ChangePasswordDialog);
    } // setupUi

    void retranslateUi(QDialog *ChangePasswordDialog)
    {
        ChangePasswordDialog->setWindowTitle(QApplication::translate("ChangePasswordDialog", "Change password", 0));
        m_newPasswordTextLabel->setText(QApplication::translate("ChangePasswordDialog", "NEW PASSWORD", 0));
        m_newPasswordConfirmationTextLabel->setText(QApplication::translate("ChangePasswordDialog", "CONFIRM", 0));
        m_oldPasswordTextLabel->setText(QApplication::translate("ChangePasswordDialog", "OLD PASSWORD", 0));
        m_cancelButton->setText(QApplication::translate("ChangePasswordDialog", "Cancel", 0));
        m_okButton->setText(QApplication::translate("ChangePasswordDialog", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class ChangePasswordDialog: public Ui_ChangePasswordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGEPASSWORDDIALOG_H
