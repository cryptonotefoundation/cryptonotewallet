/********************************************************************************
** Form generated from reading UI file 'KeyDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEYDIALOG_H
#define UI_KEYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include "Gui/Common/WalletBlueButton.h"
#include "Gui/Common/WalletCancelButton.h"
#include "Gui/Common/WalletOkButton.h"
#include "Gui/Common/WalletTextLabel.h"

QT_BEGIN_NAMESPACE

class Ui_KeyDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *m_keyEdit;
    QSpacerItem *verticalSpacer;
    WalletGui::WalletSmallGrayTextLabel *m_descriptionLabel;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    WalletGui::WalletNormalBlueButton *m_fileButton;
    QSpacerItem *horizontalSpacer;
    WalletGui::WalletCancelButton *m_cancelButton;
    WalletGui::WalletOkButton *m_okButton;

    void setupUi(QDialog *KeyDialog)
    {
        if (KeyDialog->objectName().isEmpty())
            KeyDialog->setObjectName(QStringLiteral("KeyDialog"));
        KeyDialog->resize(585, 188);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(KeyDialog->sizePolicy().hasHeightForWidth());
        KeyDialog->setSizePolicy(sizePolicy);
        KeyDialog->setMinimumSize(QSize(585, 188));
        KeyDialog->setMaximumSize(QSize(585, 188));
        verticalLayout = new QVBoxLayout(KeyDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        m_keyEdit = new QTextEdit(KeyDialog);
        m_keyEdit->setObjectName(QStringLiteral("m_keyEdit"));
        m_keyEdit->setMinimumSize(QSize(0, 80));
        m_keyEdit->setMaximumSize(QSize(16777215, 80));
        m_keyEdit->setFrameShape(QFrame::NoFrame);

        verticalLayout->addWidget(m_keyEdit);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        m_descriptionLabel = new WalletGui::WalletSmallGrayTextLabel(KeyDialog);
        m_descriptionLabel->setObjectName(QStringLiteral("m_descriptionLabel"));
        m_descriptionLabel->setWordWrap(true);

        verticalLayout->addWidget(m_descriptionLabel, 0, Qt::AlignTop);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        m_fileButton = new WalletGui::WalletNormalBlueButton(KeyDialog);
        m_fileButton->setObjectName(QStringLiteral("m_fileButton"));
        m_fileButton->setMinimumSize(QSize(100, 0));

        horizontalLayout->addWidget(m_fileButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_cancelButton = new WalletGui::WalletCancelButton(KeyDialog);
        m_cancelButton->setObjectName(QStringLiteral("m_cancelButton"));

        horizontalLayout->addWidget(m_cancelButton);

        m_okButton = new WalletGui::WalletOkButton(KeyDialog);
        m_okButton->setObjectName(QStringLiteral("m_okButton"));

        horizontalLayout->addWidget(m_okButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(KeyDialog);
        QObject::connect(m_fileButton, SIGNAL(clicked()), KeyDialog, SLOT(fileClicked()));
        QObject::connect(m_okButton, SIGNAL(clicked()), KeyDialog, SLOT(accept()));
        QObject::connect(m_cancelButton, SIGNAL(clicked()), KeyDialog, SLOT(reject()));
        QObject::connect(m_keyEdit, SIGNAL(textChanged()), KeyDialog, SLOT(keyChanged()));

        QMetaObject::connectSlotsByName(KeyDialog);
    } // setupUi

    void retranslateUi(QDialog *KeyDialog)
    {
        KeyDialog->setWindowTitle(QApplication::translate("KeyDialog", "Key", 0));
        m_descriptionLabel->setText(QString());
        m_fileButton->setText(QApplication::translate("KeyDialog", "File", 0));
        m_cancelButton->setText(QApplication::translate("KeyDialog", "Cancel", 0));
        m_okButton->setText(QApplication::translate("KeyDialog", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class KeyDialog: public Ui_KeyDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEYDIALOG_H
