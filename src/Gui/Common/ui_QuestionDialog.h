/********************************************************************************
** Form generated from reading UI file 'QuestionDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUESTIONDIALOG_H
#define UI_QUESTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "Gui/Common/WalletCancelButton.h"
#include "Gui/Common/WalletOkButton.h"

QT_BEGIN_NAMESPACE

class Ui_QuestionDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *m_questionLabel;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    WalletGui::WalletCancelButton *m_cancelButton;
    WalletGui::WalletOkButton *m_okButton;

    void setupUi(QDialog *QuestionDialog)
    {
        if (QuestionDialog->objectName().isEmpty())
            QuestionDialog->setObjectName(QStringLiteral("QuestionDialog"));
        QuestionDialog->resize(481, 116);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QuestionDialog->sizePolicy().hasHeightForWidth());
        QuestionDialog->setSizePolicy(sizePolicy);
        QuestionDialog->setMinimumSize(QSize(481, 116));
        QuestionDialog->setMaximumSize(QSize(481, 130));
        verticalLayout = new QVBoxLayout(QuestionDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        m_questionLabel = new QLabel(QuestionDialog);
        m_questionLabel->setObjectName(QStringLiteral("m_questionLabel"));

        verticalLayout->addWidget(m_questionLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_cancelButton = new WalletGui::WalletCancelButton(QuestionDialog);
        m_cancelButton->setObjectName(QStringLiteral("m_cancelButton"));

        horizontalLayout->addWidget(m_cancelButton);

        m_okButton = new WalletGui::WalletOkButton(QuestionDialog);
        m_okButton->setObjectName(QStringLiteral("m_okButton"));

        horizontalLayout->addWidget(m_okButton);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 1);

        retranslateUi(QuestionDialog);
        QObject::connect(m_okButton, SIGNAL(clicked()), QuestionDialog, SLOT(accept()));
        QObject::connect(m_cancelButton, SIGNAL(clicked()), QuestionDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(QuestionDialog);
    } // setupUi

    void retranslateUi(QDialog *QuestionDialog)
    {
        QuestionDialog->setWindowTitle(QString());
        m_questionLabel->setText(QString());
        m_cancelButton->setText(QApplication::translate("QuestionDialog", "No", 0));
        m_okButton->setText(QApplication::translate("QuestionDialog", "Yes", 0));
    } // retranslateUi

};

namespace Ui {
    class QuestionDialog: public Ui_QuestionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUESTIONDIALOG_H
