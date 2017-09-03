/********************************************************************************
** Form generated from reading UI file 'PoolTransactionDetailsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POOLTRANSACTIONDETAILSDIALOG_H
#define UI_POOLTRANSACTIONDETAILSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include "Gui/Common/CopyMagicLabel.h"
#include "Gui/Common/WalletTextLabel.h"

QT_BEGIN_NAMESPACE

class Ui_PoolTransactionDetailsDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    WalletGui::WalletLargeBlackTextLabel *m_textLabel;
    WalletGui::WalletLargeBlackTextLabel *m_hashLabel;
    WalletGui::CopyMagicLabel *m_copyLabel;
    QGridLayout *gridLayout;
    WalletGui::WalletNormalGrayTextLabel *m_amountTextLabel;
    WalletGui::WalletNormalGrayTextLabel *m_sizeTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_sizeLabel;
    WalletGui::WalletNormalGrayTextLabel *m_mixinTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_mixinLabel;
    WalletGui::WalletNormalGrayTextLabel *m_paymentIdTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_paymentIdLabel;
    WalletGui::WalletNormalBlackTextLabel *m_amountLabel;
    WalletGui::WalletNormalGrayTextLabel *m_feeTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_feeLabel;

    void setupUi(QDialog *PoolTransactionDetailsDialog)
    {
        if (PoolTransactionDetailsDialog->objectName().isEmpty())
            PoolTransactionDetailsDialog->setObjectName(QStringLiteral("PoolTransactionDetailsDialog"));
        PoolTransactionDetailsDialog->resize(764, 185);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PoolTransactionDetailsDialog->sizePolicy().hasHeightForWidth());
        PoolTransactionDetailsDialog->setSizePolicy(sizePolicy);
        PoolTransactionDetailsDialog->setMinimumSize(QSize(0, 0));
        PoolTransactionDetailsDialog->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_2 = new QVBoxLayout(PoolTransactionDetailsDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(35, 30, 25, 25);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        m_textLabel = new WalletGui::WalletLargeBlackTextLabel(PoolTransactionDetailsDialog);
        m_textLabel->setObjectName(QStringLiteral("m_textLabel"));
        m_textLabel->setIndent(0);

        horizontalLayout_2->addWidget(m_textLabel);

        m_hashLabel = new WalletGui::WalletLargeBlackTextLabel(PoolTransactionDetailsDialog);
        m_hashLabel->setObjectName(QStringLiteral("m_hashLabel"));
        m_hashLabel->setCursor(QCursor(Qt::PointingHandCursor));
        m_hashLabel->setIndent(0);
        m_hashLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        horizontalLayout_2->addWidget(m_hashLabel);

        m_copyLabel = new WalletGui::CopyMagicLabel(PoolTransactionDetailsDialog);
        m_copyLabel->setObjectName(QStringLiteral("m_copyLabel"));
        m_copyLabel->setIndent(0);

        horizontalLayout_2->addWidget(m_copyLabel);

        horizontalLayout_2->setStretch(2, 1);

        verticalLayout_2->addLayout(horizontalLayout_2);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, -1, 0, -1);
        m_amountTextLabel = new WalletGui::WalletNormalGrayTextLabel(PoolTransactionDetailsDialog);
        m_amountTextLabel->setObjectName(QStringLiteral("m_amountTextLabel"));
        m_amountTextLabel->setIndent(0);

        gridLayout->addWidget(m_amountTextLabel, 0, 0, 1, 1);

        m_sizeTextLabel = new WalletGui::WalletNormalGrayTextLabel(PoolTransactionDetailsDialog);
        m_sizeTextLabel->setObjectName(QStringLiteral("m_sizeTextLabel"));
        m_sizeTextLabel->setIndent(0);

        gridLayout->addWidget(m_sizeTextLabel, 2, 0, 1, 1);

        m_sizeLabel = new WalletGui::WalletNormalBlackTextLabel(PoolTransactionDetailsDialog);
        m_sizeLabel->setObjectName(QStringLiteral("m_sizeLabel"));
        m_sizeLabel->setIndent(0);
        m_sizeLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_sizeLabel, 2, 1, 1, 1);

        m_mixinTextLabel = new WalletGui::WalletNormalGrayTextLabel(PoolTransactionDetailsDialog);
        m_mixinTextLabel->setObjectName(QStringLiteral("m_mixinTextLabel"));
        m_mixinTextLabel->setIndent(0);

        gridLayout->addWidget(m_mixinTextLabel, 5, 0, 1, 1);

        m_mixinLabel = new WalletGui::WalletNormalBlackTextLabel(PoolTransactionDetailsDialog);
        m_mixinLabel->setObjectName(QStringLiteral("m_mixinLabel"));
        m_mixinLabel->setIndent(0);
        m_mixinLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_mixinLabel, 5, 1, 1, 1);

        m_paymentIdTextLabel = new WalletGui::WalletNormalGrayTextLabel(PoolTransactionDetailsDialog);
        m_paymentIdTextLabel->setObjectName(QStringLiteral("m_paymentIdTextLabel"));
        m_paymentIdTextLabel->setIndent(0);

        gridLayout->addWidget(m_paymentIdTextLabel, 8, 0, 1, 1);

        m_paymentIdLabel = new WalletGui::WalletNormalBlackTextLabel(PoolTransactionDetailsDialog);
        m_paymentIdLabel->setObjectName(QStringLiteral("m_paymentIdLabel"));
        m_paymentIdLabel->setIndent(0);
        m_paymentIdLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_paymentIdLabel, 8, 1, 1, 1);

        m_amountLabel = new WalletGui::WalletNormalBlackTextLabel(PoolTransactionDetailsDialog);
        m_amountLabel->setObjectName(QStringLiteral("m_amountLabel"));
        m_amountLabel->setIndent(0);
        m_amountLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_amountLabel, 0, 1, 1, 1);

        m_feeTextLabel = new WalletGui::WalletNormalGrayTextLabel(PoolTransactionDetailsDialog);
        m_feeTextLabel->setObjectName(QStringLiteral("m_feeTextLabel"));
        m_feeTextLabel->setIndent(0);

        gridLayout->addWidget(m_feeTextLabel, 1, 0, 1, 1);

        m_feeLabel = new WalletGui::WalletNormalBlackTextLabel(PoolTransactionDetailsDialog);
        m_feeLabel->setObjectName(QStringLiteral("m_feeLabel"));
        m_feeLabel->setIndent(0);
        m_feeLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_feeLabel, 1, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        verticalLayout_2->setStretch(1, 1);

        retranslateUi(PoolTransactionDetailsDialog);

        QMetaObject::connectSlotsByName(PoolTransactionDetailsDialog);
    } // setupUi

    void retranslateUi(QDialog *PoolTransactionDetailsDialog)
    {
        PoolTransactionDetailsDialog->setWindowTitle(QApplication::translate("PoolTransactionDetailsDialog", "Transaction details", 0));
        m_textLabel->setText(QApplication::translate("PoolTransactionDetailsDialog", "Transaction", 0));
#ifndef QT_NO_TOOLTIP
        m_hashLabel->setToolTip(QApplication::translate("PoolTransactionDetailsDialog", "Click to copy", 0));
#endif // QT_NO_TOOLTIP
        m_hashLabel->setText(QApplication::translate("PoolTransactionDetailsDialog", "HASH", 0));
        m_copyLabel->setText(QApplication::translate("PoolTransactionDetailsDialog", "Copied!", 0));
        m_amountTextLabel->setText(QApplication::translate("PoolTransactionDetailsDialog", "AMOUNT", 0));
        m_sizeTextLabel->setText(QApplication::translate("PoolTransactionDetailsDialog", "SIZE", 0));
        m_sizeLabel->setText(QApplication::translate("PoolTransactionDetailsDialog", "TextLabel", 0));
        m_mixinTextLabel->setText(QApplication::translate("PoolTransactionDetailsDialog", "MIXIN", 0));
        m_mixinLabel->setText(QApplication::translate("PoolTransactionDetailsDialog", "TextLabel", 0));
        m_paymentIdTextLabel->setText(QApplication::translate("PoolTransactionDetailsDialog", "PAYMENT ID", 0));
        m_paymentIdLabel->setText(QApplication::translate("PoolTransactionDetailsDialog", "TextLabel", 0));
        m_amountLabel->setText(QApplication::translate("PoolTransactionDetailsDialog", "TextLabel", 0));
        m_feeTextLabel->setText(QApplication::translate("PoolTransactionDetailsDialog", "FEE", 0));
        m_feeLabel->setText(QApplication::translate("PoolTransactionDetailsDialog", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class PoolTransactionDetailsDialog: public Ui_PoolTransactionDetailsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POOLTRANSACTIONDETAILSDIALOG_H
