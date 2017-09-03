/********************************************************************************
** Form generated from reading UI file 'TransactionDetailsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSACTIONDETAILSDIALOG_H
#define UI_TRANSACTIONDETAILSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Gui/Common/CopyMagicLabel.h"
#include "Gui/Common/WalletTextLabel.h"
#include "Gui/Common/WalletTreeView.h"

QT_BEGIN_NAMESPACE

class Ui_TransactionDetailsDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QTabWidget *m_transactionDetailsTabBar;
    QWidget *m_transactionTab;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    WalletGui::WalletLargeBlackTextLabel *m_textLabel;
    WalletGui::WalletLargeBlackTextLabel *m_hashLabel;
    WalletGui::CopyMagicLabel *m_copyLabel;
    QGridLayout *gridLayout;
    WalletGui::WalletNormalGrayTextLabel *m_statusTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_statusLabel;
    WalletGui::WalletNormalGrayTextLabel *m_timeTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_timeLabel;
    WalletGui::WalletNormalGrayTextLabel *m_heightTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_heightLabel;
    WalletGui::WalletNormalGrayTextLabel *m_toTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_toLabel;
    WalletGui::WalletNormalGrayTextLabel *m_amountTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_amountLabel;
    WalletGui::WalletNormalGrayTextLabel *m_feeTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_feeLabel;
    WalletGui::WalletNormalGrayTextLabel *m_paymentIdTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_paymentIdLabel;
    QWidget *m_transfersTab;
    QVBoxLayout *verticalLayout_3;
    WalletGui::WalletTreeView *m_transfersView;

    void setupUi(QDialog *TransactionDetailsDialog)
    {
        if (TransactionDetailsDialog->objectName().isEmpty())
            TransactionDetailsDialog->setObjectName(QStringLiteral("TransactionDetailsDialog"));
        TransactionDetailsDialog->resize(739, 296);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TransactionDetailsDialog->sizePolicy().hasHeightForWidth());
        TransactionDetailsDialog->setSizePolicy(sizePolicy);
        TransactionDetailsDialog->setMinimumSize(QSize(0, 0));
        TransactionDetailsDialog->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_2 = new QVBoxLayout(TransactionDetailsDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 12, 0, 0);
        m_transactionDetailsTabBar = new QTabWidget(TransactionDetailsDialog);
        m_transactionDetailsTabBar->setObjectName(QStringLiteral("m_transactionDetailsTabBar"));
        m_transactionTab = new QWidget();
        m_transactionTab->setObjectName(QStringLiteral("m_transactionTab"));
        verticalLayout = new QVBoxLayout(m_transactionTab);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(35, 30, 25, 25);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        m_textLabel = new WalletGui::WalletLargeBlackTextLabel(m_transactionTab);
        m_textLabel->setObjectName(QStringLiteral("m_textLabel"));
        m_textLabel->setIndent(0);

        horizontalLayout_2->addWidget(m_textLabel);

        m_hashLabel = new WalletGui::WalletLargeBlackTextLabel(m_transactionTab);
        m_hashLabel->setObjectName(QStringLiteral("m_hashLabel"));
        m_hashLabel->setCursor(QCursor(Qt::PointingHandCursor));
        m_hashLabel->setIndent(0);
        m_hashLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        horizontalLayout_2->addWidget(m_hashLabel);

        m_copyLabel = new WalletGui::CopyMagicLabel(m_transactionTab);
        m_copyLabel->setObjectName(QStringLiteral("m_copyLabel"));
        m_copyLabel->setIndent(0);

        horizontalLayout_2->addWidget(m_copyLabel, 0, Qt::AlignLeft);

        horizontalLayout_2->setStretch(2, 1);

        verticalLayout->addLayout(horizontalLayout_2);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, -1, 0, -1);
        m_statusTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_transactionTab);
        m_statusTextLabel->setObjectName(QStringLiteral("m_statusTextLabel"));
        m_statusTextLabel->setIndent(0);

        gridLayout->addWidget(m_statusTextLabel, 0, 0, 1, 1);

        m_statusLabel = new WalletGui::WalletNormalBlackTextLabel(m_transactionTab);
        m_statusLabel->setObjectName(QStringLiteral("m_statusLabel"));
        m_statusLabel->setIndent(0);
        m_statusLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_statusLabel, 0, 1, 1, 1);

        m_timeTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_transactionTab);
        m_timeTextLabel->setObjectName(QStringLiteral("m_timeTextLabel"));
        m_timeTextLabel->setIndent(0);

        gridLayout->addWidget(m_timeTextLabel, 1, 0, 1, 1);

        m_timeLabel = new WalletGui::WalletNormalBlackTextLabel(m_transactionTab);
        m_timeLabel->setObjectName(QStringLiteral("m_timeLabel"));
        m_timeLabel->setIndent(0);
        m_timeLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_timeLabel, 1, 1, 1, 1);

        m_heightTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_transactionTab);
        m_heightTextLabel->setObjectName(QStringLiteral("m_heightTextLabel"));
        m_heightTextLabel->setIndent(0);

        gridLayout->addWidget(m_heightTextLabel, 2, 0, 1, 1);

        m_heightLabel = new WalletGui::WalletNormalBlackTextLabel(m_transactionTab);
        m_heightLabel->setObjectName(QStringLiteral("m_heightLabel"));
        m_heightLabel->setIndent(0);
        m_heightLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_heightLabel, 2, 1, 1, 1);

        m_toTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_transactionTab);
        m_toTextLabel->setObjectName(QStringLiteral("m_toTextLabel"));
        m_toTextLabel->setIndent(0);

        gridLayout->addWidget(m_toTextLabel, 3, 0, 1, 1);

        m_toLabel = new WalletGui::WalletNormalBlackTextLabel(m_transactionTab);
        m_toLabel->setObjectName(QStringLiteral("m_toLabel"));
        m_toLabel->setIndent(0);
        m_toLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_toLabel, 3, 1, 1, 1);

        m_amountTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_transactionTab);
        m_amountTextLabel->setObjectName(QStringLiteral("m_amountTextLabel"));
        m_amountTextLabel->setIndent(0);

        gridLayout->addWidget(m_amountTextLabel, 4, 0, 1, 1);

        m_amountLabel = new WalletGui::WalletNormalBlackTextLabel(m_transactionTab);
        m_amountLabel->setObjectName(QStringLiteral("m_amountLabel"));
        m_amountLabel->setIndent(0);
        m_amountLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_amountLabel, 4, 1, 1, 1);

        m_feeTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_transactionTab);
        m_feeTextLabel->setObjectName(QStringLiteral("m_feeTextLabel"));
        m_feeTextLabel->setIndent(0);

        gridLayout->addWidget(m_feeTextLabel, 5, 0, 1, 1);

        m_feeLabel = new WalletGui::WalletNormalBlackTextLabel(m_transactionTab);
        m_feeLabel->setObjectName(QStringLiteral("m_feeLabel"));
        m_feeLabel->setIndent(0);
        m_feeLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_feeLabel, 5, 1, 1, 1);

        m_paymentIdTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_transactionTab);
        m_paymentIdTextLabel->setObjectName(QStringLiteral("m_paymentIdTextLabel"));
        m_paymentIdTextLabel->setIndent(0);

        gridLayout->addWidget(m_paymentIdTextLabel, 6, 0, 1, 1);

        m_paymentIdLabel = new WalletGui::WalletNormalBlackTextLabel(m_transactionTab);
        m_paymentIdLabel->setObjectName(QStringLiteral("m_paymentIdLabel"));
        m_paymentIdLabel->setIndent(0);
        m_paymentIdLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_paymentIdLabel, 6, 1, 1, 1);

        gridLayout->setColumnStretch(1, 1);

        verticalLayout->addLayout(gridLayout);

        verticalLayout->setStretch(1, 1);
        m_transactionDetailsTabBar->addTab(m_transactionTab, QString());
        m_transfersTab = new QWidget();
        m_transfersTab->setObjectName(QStringLiteral("m_transfersTab"));
        verticalLayout_3 = new QVBoxLayout(m_transfersTab);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(25, 30, 25, 25);
        m_transfersView = new WalletGui::WalletTreeView(m_transfersTab);
        m_transfersView->setObjectName(QStringLiteral("m_transfersView"));
        m_transfersView->setFrameShape(QFrame::NoFrame);
        m_transfersView->setAlternatingRowColors(true);
        m_transfersView->setSelectionMode(QAbstractItemView::NoSelection);
        m_transfersView->setTextElideMode(Qt::ElideMiddle);
        m_transfersView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        m_transfersView->setRootIsDecorated(false);
        m_transfersView->setUniformRowHeights(true);
        m_transfersView->setItemsExpandable(false);
        m_transfersView->header()->setStretchLastSection(false);

        verticalLayout_3->addWidget(m_transfersView);

        m_transactionDetailsTabBar->addTab(m_transfersTab, QString());

        verticalLayout_2->addWidget(m_transactionDetailsTabBar);


        retranslateUi(TransactionDetailsDialog);

        m_transactionDetailsTabBar->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TransactionDetailsDialog);
    } // setupUi

    void retranslateUi(QDialog *TransactionDetailsDialog)
    {
        TransactionDetailsDialog->setWindowTitle(QApplication::translate("TransactionDetailsDialog", "Transaction details", 0));
        m_textLabel->setText(QApplication::translate("TransactionDetailsDialog", "Transaction", 0));
#ifndef QT_NO_TOOLTIP
        m_hashLabel->setToolTip(QApplication::translate("TransactionDetailsDialog", "Click to copy", 0));
#endif // QT_NO_TOOLTIP
        m_hashLabel->setText(QApplication::translate("TransactionDetailsDialog", "HASH", 0));
        m_copyLabel->setText(QApplication::translate("TransactionDetailsDialog", "Copied!", 0));
        m_statusTextLabel->setText(QApplication::translate("TransactionDetailsDialog", "STATUS", 0));
        m_statusLabel->setText(QApplication::translate("TransactionDetailsDialog", "TextLabel", 0));
        m_timeTextLabel->setText(QApplication::translate("TransactionDetailsDialog", "TIME", 0));
        m_timeLabel->setText(QApplication::translate("TransactionDetailsDialog", "TextLabel", 0));
        m_heightTextLabel->setText(QApplication::translate("TransactionDetailsDialog", "BLOCK HEIGHT", 0));
        m_heightLabel->setText(QApplication::translate("TransactionDetailsDialog", "TextLabel", 0));
        m_toTextLabel->setText(QApplication::translate("TransactionDetailsDialog", "TRANSFERS", 0));
        m_toLabel->setText(QApplication::translate("TransactionDetailsDialog", "TextLabel", 0));
        m_amountTextLabel->setText(QApplication::translate("TransactionDetailsDialog", "AMOUNT", 0));
        m_amountLabel->setText(QApplication::translate("TransactionDetailsDialog", "TextLabel", 0));
        m_feeTextLabel->setText(QApplication::translate("TransactionDetailsDialog", "FEE", 0));
        m_feeLabel->setText(QApplication::translate("TransactionDetailsDialog", "TextLabel", 0));
        m_paymentIdTextLabel->setText(QApplication::translate("TransactionDetailsDialog", "PAYMENT ID", 0));
        m_paymentIdLabel->setText(QApplication::translate("TransactionDetailsDialog", "TextLabel", 0));
        m_transactionDetailsTabBar->setTabText(m_transactionDetailsTabBar->indexOf(m_transactionTab), QApplication::translate("TransactionDetailsDialog", "General", 0));
        m_transactionDetailsTabBar->setTabText(m_transactionDetailsTabBar->indexOf(m_transfersTab), QApplication::translate("TransactionDetailsDialog", "Transfers", 0));
    } // retranslateUi

};

namespace Ui {
    class TransactionDetailsDialog: public Ui_TransactionDetailsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSACTIONDETAILSDIALOG_H
