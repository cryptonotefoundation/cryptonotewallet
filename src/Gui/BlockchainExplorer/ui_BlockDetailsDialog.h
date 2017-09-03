/********************************************************************************
** Form generated from reading UI file 'BlockDetailsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLOCKDETAILSDIALOG_H
#define UI_BLOCKDETAILSDIALOG_H

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

class Ui_BlockDetailsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *m_blockDetailsTabBar;
    QWidget *m_generalTab;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    WalletGui::WalletLargeBlackTextLabel *m_blockTextLabel;
    WalletGui::WalletLargeBlackTextLabel *m_blockHashLabel;
    WalletGui::CopyMagicLabel *m_copyLabel;
    QGridLayout *gridLayout;
    WalletGui::WalletNormalBlackTextLabel *m_blockSizeLabel;
    WalletGui::WalletNormalGrayTextLabel *m_heightTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_timestampLabel;
    WalletGui::WalletNormalBlackTextLabel *m_heightLabel;
    WalletGui::WalletNormalGrayTextLabel *m_timestampTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_difficultyLabel;
    WalletGui::WalletNormalGrayTextLabel *m_difficultyTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_transactionCountLabel;
    WalletGui::WalletNormalGrayTextLabel *m_transactionsSizeTextLabel;
    WalletGui::WalletNormalGrayTextLabel *m_orphanTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_penaltyLabel;
    WalletGui::WalletNormalBlackTextLabel *m_orphanLabel;
    WalletGui::WalletNormalGrayTextLabel *m_transactionCountTextLabel;
    WalletGui::WalletNormalGrayTextLabel *m_penaltyTextLabel;
    WalletGui::WalletNormalGrayTextLabel *m_baseRewardTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_transactionsSizeLabel;
    WalletGui::WalletNormalGrayTextLabel *m_blockSizeTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_totalTransactionsLabel;
    WalletGui::WalletNormalBlackTextLabel *m_baseRewardLabel;
    WalletGui::WalletNormalGrayTextLabel *m_feeTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_feeLabel;
    WalletGui::WalletNormalGrayTextLabel *m_rewardTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_rewardLabel;
    WalletGui::WalletNormalGrayTextLabel *m_totalCoinsTextLabel;
    WalletGui::WalletNormalBlackTextLabel *m_totalCoinsLabel;
    WalletGui::WalletNormalGrayTextLabel *m_totalTransactionsTextLabel;
    QWidget *m_transactionsTab;
    QVBoxLayout *verticalLayout_3;
    WalletGui::WalletTreeView *m_blockTransactionsView;

    void setupUi(QDialog *BlockDetailsDialog)
    {
        if (BlockDetailsDialog->objectName().isEmpty())
            BlockDetailsDialog->setObjectName(QStringLiteral("BlockDetailsDialog"));
        BlockDetailsDialog->resize(830, 330);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BlockDetailsDialog->sizePolicy().hasHeightForWidth());
        BlockDetailsDialog->setSizePolicy(sizePolicy);
        BlockDetailsDialog->setMinimumSize(QSize(830, 0));
        BlockDetailsDialog->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout = new QVBoxLayout(BlockDetailsDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 12, 0, 0);
        m_blockDetailsTabBar = new QTabWidget(BlockDetailsDialog);
        m_blockDetailsTabBar->setObjectName(QStringLiteral("m_blockDetailsTabBar"));
        m_generalTab = new QWidget();
        m_generalTab->setObjectName(QStringLiteral("m_generalTab"));
        verticalLayout_2 = new QVBoxLayout(m_generalTab);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(25, 30, 25, 25);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        m_blockTextLabel = new WalletGui::WalletLargeBlackTextLabel(m_generalTab);
        m_blockTextLabel->setObjectName(QStringLiteral("m_blockTextLabel"));
        m_blockTextLabel->setIndent(0);

        horizontalLayout_2->addWidget(m_blockTextLabel);

        m_blockHashLabel = new WalletGui::WalletLargeBlackTextLabel(m_generalTab);
        m_blockHashLabel->setObjectName(QStringLiteral("m_blockHashLabel"));
        m_blockHashLabel->setCursor(QCursor(Qt::PointingHandCursor));
        m_blockHashLabel->setIndent(0);
        m_blockHashLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        horizontalLayout_2->addWidget(m_blockHashLabel);

        m_copyLabel = new WalletGui::CopyMagicLabel(m_generalTab);
        m_copyLabel->setObjectName(QStringLiteral("m_copyLabel"));
        m_copyLabel->setIndent(0);

        horizontalLayout_2->addWidget(m_copyLabel, 0, Qt::AlignLeft);

        horizontalLayout_2->setStretch(2, 1);

        verticalLayout_2->addLayout(horizontalLayout_2);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, -1, 0, -1);
        m_blockSizeLabel = new WalletGui::WalletNormalBlackTextLabel(m_generalTab);
        m_blockSizeLabel->setObjectName(QStringLiteral("m_blockSizeLabel"));
        m_blockSizeLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_blockSizeLabel, 6, 1, 1, 1);

        m_heightTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_generalTab);
        m_heightTextLabel->setObjectName(QStringLiteral("m_heightTextLabel"));

        gridLayout->addWidget(m_heightTextLabel, 0, 0, 1, 1);

        m_timestampLabel = new WalletGui::WalletNormalBlackTextLabel(m_generalTab);
        m_timestampLabel->setObjectName(QStringLiteral("m_timestampLabel"));
        m_timestampLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_timestampLabel, 1, 1, 1, 1);

        m_heightLabel = new WalletGui::WalletNormalBlackTextLabel(m_generalTab);
        m_heightLabel->setObjectName(QStringLiteral("m_heightLabel"));
        m_heightLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_heightLabel, 0, 1, 1, 1);

        m_timestampTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_generalTab);
        m_timestampTextLabel->setObjectName(QStringLiteral("m_timestampTextLabel"));

        gridLayout->addWidget(m_timestampTextLabel, 1, 0, 1, 1);

        m_difficultyLabel = new WalletGui::WalletNormalBlackTextLabel(m_generalTab);
        m_difficultyLabel->setObjectName(QStringLiteral("m_difficultyLabel"));
        m_difficultyLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_difficultyLabel, 2, 1, 1, 1);

        m_difficultyTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_generalTab);
        m_difficultyTextLabel->setObjectName(QStringLiteral("m_difficultyTextLabel"));

        gridLayout->addWidget(m_difficultyTextLabel, 2, 0, 1, 1);

        m_transactionCountLabel = new WalletGui::WalletNormalBlackTextLabel(m_generalTab);
        m_transactionCountLabel->setObjectName(QStringLiteral("m_transactionCountLabel"));
        m_transactionCountLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_transactionCountLabel, 4, 1, 1, 1);

        m_transactionsSizeTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_generalTab);
        m_transactionsSizeTextLabel->setObjectName(QStringLiteral("m_transactionsSizeTextLabel"));

        gridLayout->addWidget(m_transactionsSizeTextLabel, 5, 0, 1, 1);

        m_orphanTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_generalTab);
        m_orphanTextLabel->setObjectName(QStringLiteral("m_orphanTextLabel"));

        gridLayout->addWidget(m_orphanTextLabel, 3, 0, 1, 1);

        m_penaltyLabel = new WalletGui::WalletNormalBlackTextLabel(m_generalTab);
        m_penaltyLabel->setObjectName(QStringLiteral("m_penaltyLabel"));
        m_penaltyLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_penaltyLabel, 7, 1, 1, 1);

        m_orphanLabel = new WalletGui::WalletNormalBlackTextLabel(m_generalTab);
        m_orphanLabel->setObjectName(QStringLiteral("m_orphanLabel"));
        m_orphanLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_orphanLabel, 3, 1, 1, 1);

        m_transactionCountTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_generalTab);
        m_transactionCountTextLabel->setObjectName(QStringLiteral("m_transactionCountTextLabel"));

        gridLayout->addWidget(m_transactionCountTextLabel, 4, 0, 1, 1);

        m_penaltyTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_generalTab);
        m_penaltyTextLabel->setObjectName(QStringLiteral("m_penaltyTextLabel"));

        gridLayout->addWidget(m_penaltyTextLabel, 7, 0, 1, 1);

        m_baseRewardTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_generalTab);
        m_baseRewardTextLabel->setObjectName(QStringLiteral("m_baseRewardTextLabel"));

        gridLayout->addWidget(m_baseRewardTextLabel, 8, 0, 1, 1);

        m_transactionsSizeLabel = new WalletGui::WalletNormalBlackTextLabel(m_generalTab);
        m_transactionsSizeLabel->setObjectName(QStringLiteral("m_transactionsSizeLabel"));
        m_transactionsSizeLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_transactionsSizeLabel, 5, 1, 1, 1);

        m_blockSizeTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_generalTab);
        m_blockSizeTextLabel->setObjectName(QStringLiteral("m_blockSizeTextLabel"));

        gridLayout->addWidget(m_blockSizeTextLabel, 6, 0, 1, 1);

        m_totalTransactionsLabel = new WalletGui::WalletNormalBlackTextLabel(m_generalTab);
        m_totalTransactionsLabel->setObjectName(QStringLiteral("m_totalTransactionsLabel"));
        m_totalTransactionsLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_totalTransactionsLabel, 12, 1, 1, 1);

        m_baseRewardLabel = new WalletGui::WalletNormalBlackTextLabel(m_generalTab);
        m_baseRewardLabel->setObjectName(QStringLiteral("m_baseRewardLabel"));
        m_baseRewardLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_baseRewardLabel, 8, 1, 1, 1);

        m_feeTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_generalTab);
        m_feeTextLabel->setObjectName(QStringLiteral("m_feeTextLabel"));

        gridLayout->addWidget(m_feeTextLabel, 9, 0, 1, 1);

        m_feeLabel = new WalletGui::WalletNormalBlackTextLabel(m_generalTab);
        m_feeLabel->setObjectName(QStringLiteral("m_feeLabel"));
        m_feeLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_feeLabel, 9, 1, 1, 1);

        m_rewardTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_generalTab);
        m_rewardTextLabel->setObjectName(QStringLiteral("m_rewardTextLabel"));

        gridLayout->addWidget(m_rewardTextLabel, 10, 0, 1, 1);

        m_rewardLabel = new WalletGui::WalletNormalBlackTextLabel(m_generalTab);
        m_rewardLabel->setObjectName(QStringLiteral("m_rewardLabel"));
        m_rewardLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_rewardLabel, 10, 1, 1, 1);

        m_totalCoinsTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_generalTab);
        m_totalCoinsTextLabel->setObjectName(QStringLiteral("m_totalCoinsTextLabel"));

        gridLayout->addWidget(m_totalCoinsTextLabel, 11, 0, 1, 1);

        m_totalCoinsLabel = new WalletGui::WalletNormalBlackTextLabel(m_generalTab);
        m_totalCoinsLabel->setObjectName(QStringLiteral("m_totalCoinsLabel"));
        m_totalCoinsLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout->addWidget(m_totalCoinsLabel, 11, 1, 1, 1);

        m_totalTransactionsTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_generalTab);
        m_totalTransactionsTextLabel->setObjectName(QStringLiteral("m_totalTransactionsTextLabel"));

        gridLayout->addWidget(m_totalTransactionsTextLabel, 12, 0, 1, 1);

        gridLayout->setColumnStretch(1, 1);

        verticalLayout_2->addLayout(gridLayout);

        verticalLayout_2->setStretch(1, 1);
        m_blockDetailsTabBar->addTab(m_generalTab, QString());
        m_transactionsTab = new QWidget();
        m_transactionsTab->setObjectName(QStringLiteral("m_transactionsTab"));
        verticalLayout_3 = new QVBoxLayout(m_transactionsTab);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(25, 30, 25, 25);
        m_blockTransactionsView = new WalletGui::WalletTreeView(m_transactionsTab);
        m_blockTransactionsView->setObjectName(QStringLiteral("m_blockTransactionsView"));
        m_blockTransactionsView->setFocusPolicy(Qt::NoFocus);
        m_blockTransactionsView->setFrameShape(QFrame::NoFrame);
        m_blockTransactionsView->setAlternatingRowColors(true);
        m_blockTransactionsView->setSelectionMode(QAbstractItemView::NoSelection);
        m_blockTransactionsView->setTextElideMode(Qt::ElideMiddle);
        m_blockTransactionsView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        m_blockTransactionsView->setRootIsDecorated(false);
        m_blockTransactionsView->setUniformRowHeights(true);
        m_blockTransactionsView->setItemsExpandable(false);
        m_blockTransactionsView->setExpandsOnDoubleClick(false);
        m_blockTransactionsView->header()->setStretchLastSection(false);

        verticalLayout_3->addWidget(m_blockTransactionsView);

        m_blockDetailsTabBar->addTab(m_transactionsTab, QString());

        verticalLayout->addWidget(m_blockDetailsTabBar);


        retranslateUi(BlockDetailsDialog);

        m_blockDetailsTabBar->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(BlockDetailsDialog);
    } // setupUi

    void retranslateUi(QDialog *BlockDetailsDialog)
    {
        BlockDetailsDialog->setWindowTitle(QApplication::translate("BlockDetailsDialog", "Block details", 0));
        m_blockTextLabel->setText(QApplication::translate("BlockDetailsDialog", "Block", 0));
#ifndef QT_NO_TOOLTIP
        m_blockHashLabel->setToolTip(QApplication::translate("BlockDetailsDialog", "Click to copy", 0));
#endif // QT_NO_TOOLTIP
        m_blockHashLabel->setText(QApplication::translate("BlockDetailsDialog", "HASH", 0));
        m_copyLabel->setText(QApplication::translate("BlockDetailsDialog", "Copied!", 0));
        m_blockSizeLabel->setText(QApplication::translate("BlockDetailsDialog", "TextLabel", 0));
        m_heightTextLabel->setText(QApplication::translate("BlockDetailsDialog", "HEIGHT", 0));
        m_timestampLabel->setText(QApplication::translate("BlockDetailsDialog", "TextLabel", 0));
        m_heightLabel->setText(QApplication::translate("BlockDetailsDialog", "TextLabel", 0));
        m_timestampTextLabel->setText(QApplication::translate("BlockDetailsDialog", "TIMESTAMP (UTC)", 0));
        m_difficultyLabel->setText(QApplication::translate("BlockDetailsDialog", "TextLabel", 0));
        m_difficultyTextLabel->setText(QApplication::translate("BlockDetailsDialog", "DIFFICULTY", 0));
        m_transactionCountLabel->setText(QApplication::translate("BlockDetailsDialog", "TextLabel", 0));
        m_transactionsSizeTextLabel->setText(QApplication::translate("BlockDetailsDialog", "TOTAL TRANSACTIONS SIZE", 0));
        m_orphanTextLabel->setText(QApplication::translate("BlockDetailsDialog", "ORPHAN", 0));
        m_penaltyLabel->setText(QApplication::translate("BlockDetailsDialog", "TextLabel", 0));
        m_orphanLabel->setText(QApplication::translate("BlockDetailsDialog", "TextLabel", 0));
        m_transactionCountTextLabel->setText(QApplication::translate("BlockDetailsDialog", "TRANSACTIONS", 0));
        m_penaltyTextLabel->setText(QApplication::translate("BlockDetailsDialog", "REWARD PENALTY", 0));
        m_baseRewardTextLabel->setText(QApplication::translate("BlockDetailsDialog", "BASE REWARD", 0));
        m_transactionsSizeLabel->setText(QApplication::translate("BlockDetailsDialog", "TextLabel", 0));
        m_blockSizeTextLabel->setText(QApplication::translate("BlockDetailsDialog", "TOTAL BLOCK SIZE", 0));
        m_totalTransactionsLabel->setText(QApplication::translate("BlockDetailsDialog", "TextLabel", 0));
        m_baseRewardLabel->setText(QApplication::translate("BlockDetailsDialog", "TextLabel", 0));
        m_feeTextLabel->setText(QApplication::translate("BlockDetailsDialog", "TRANSACTIONS FEE", 0));
        m_feeLabel->setText(QApplication::translate("BlockDetailsDialog", "TextLabel", 0));
        m_rewardTextLabel->setText(QApplication::translate("BlockDetailsDialog", "REWARD", 0));
        m_rewardLabel->setText(QApplication::translate("BlockDetailsDialog", "TextLabel", 0));
        m_totalCoinsTextLabel->setText(QApplication::translate("BlockDetailsDialog", "TOTAL COINS IN THE NETWORK", 0));
        m_totalCoinsLabel->setText(QApplication::translate("BlockDetailsDialog", "TextLabel", 0));
        m_totalTransactionsTextLabel->setText(QApplication::translate("BlockDetailsDialog", "TOTAL TRANSACTIONS IN THE NETWORK", 0));
        m_blockDetailsTabBar->setTabText(m_blockDetailsTabBar->indexOf(m_generalTab), QApplication::translate("BlockDetailsDialog", "General", 0));
        m_blockDetailsTabBar->setTabText(m_blockDetailsTabBar->indexOf(m_transactionsTab), QApplication::translate("BlockDetailsDialog", "Transactions", 0));
    } // retranslateUi

};

namespace Ui {
    class BlockDetailsDialog: public Ui_BlockDetailsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLOCKDETAILSDIALOG_H
