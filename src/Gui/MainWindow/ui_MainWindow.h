/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Gui/AddressBook/AddressBookFrame.h"
#include "Gui/BlockchainExplorer/BlockExplorerFrame.h"
#include "Gui/Common/CopyMagicLabel.h"
#include "Gui/Common/WalletLinkLikeButton.h"
#include "Gui/Common/WalletTextLabel.h"
#include "Gui/MainWindow/WalletStatusBar.h"
#include "Gui/Mining/MiningFrame.h"
#include "Gui/NoWallet/NoWalletFrame.h"
#include "Gui/Overview/OverviewFrame.h"
#include "Gui/Send/SendFrame.h"
#include "Gui/Transactions/TransactionsFrame.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *m_exitAction;
    QAction *m_createWalletAction;
    QAction *m_openWalletAction;
    QAction *m_encryptWalletAction;
    QAction *m_changePasswordAction;
    QAction *m_aboutIntensecoinAction;
    QAction *m_aboutQtAction;
    QAction *m_backupWalletAction;
    QAction *m_autostartAction;
    QAction *m_minimizeToTrayAction;
    QAction *m_closeToTrayAction;
    QAction *m_preferencesAction;
    QAction *m_recentWalletsAction;
    QAction *m_exportTrackingKeyAction;
    QAction *m_importKeyAction;
    QAction *m_communityForumAction;
    QAction *m_reportIssueAction;
    QAction *m_resetAction;
    QAction *m_saveKeysAction;
    QAction *m_exportKeyAction;
	QAction *m_exportPrivateKeyAction;
    QAction *m_removePendingTxAction;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QFrame *m_headerFrame;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QLabel *m_logoLabel;
    QSpacerItem *horizontalSpacer_5;
    WalletGui::WalletSmallGrayTextLabel *m_noWalletLabel;
    QFrame *m_walletFrame;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_3;
    WalletGui::WalletSmallGrayTextLabel *m_walletLabel;
    QLabel *m_addressLabel;
    QHBoxLayout *horizontalLayout_2;
    QFrame *m_notEncryptedFrame;
    QHBoxLayout *horizontalLayout_4;
    QLabel *m_notEncryptedIconLabel;
    QSpacerItem *horizontalSpacer_11;
    WalletGui::WalletTinyGrayTextLabel *m_notEncryptedTextLabel;
    QSpacerItem *horizontalSpacer_9;
    WalletGui::WalletTinyLinkLikeButton *m_encryptButton;
    QSpacerItem *horizontalSpacer_10;
    WalletGui::WalletTinyLinkLikeButton *m_copyAddressButton;
    QSpacerItem *horizontalSpacer_7;
    WalletGui::CopyMagicLabel *m_copyAddressLabel;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_8;
    QFrame *m_balanceFrame;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_4;
    QLabel *m_balanceTextLabel;
    QLabel *m_balanceLabel;
    WalletGui::CopyMagicLabel *m_balanceCopyLabel;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_5;
    QLabel *m_balanceIconLabel;
    QHBoxLayout *horizontalLayout;
    QFrame *m_toolFrame;
    QVBoxLayout *verticalLayout;
    QPushButton *m_overviewButton;
    QPushButton *m_sendButton;
    QPushButton *m_transactionsButton;
    QPushButton *m_blockExplorerButton;
    QPushButton *m_addressBookButton;
    QPushButton *m_miningButton;
    QSpacerItem *verticalSpacer;
    WalletGui::OverviewFrame *m_overviewFrame;
    WalletGui::SendFrame *m_sendFrame;
    WalletGui::TransactionsFrame *m_transactionsFrame;
    WalletGui::AddressBookFrame *m_addressBookFrame;
    WalletGui::NoWalletFrame *m_noWalletFrame;
    WalletGui::BlockExplorerFrame *m_blockExplorerFrame;
    WalletGui::MiningFrame *m_miningFrame;
    QProgressBar *m_syncProgress;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuSettings;
    QMenu *menuThemes;
    QMenu *menuHelp;
    WalletGui::WalletStatusBar *statusBar;
    QButtonGroup *m_toolButtonGroup;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1273, 824);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1260, 600));
        MainWindow->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/intensecoin"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        m_exitAction = new QAction(MainWindow);
        m_exitAction->setObjectName(QStringLiteral("m_exitAction"));
        m_exitAction->setCheckable(false);
        m_exitAction->setEnabled(true);
        m_createWalletAction = new QAction(MainWindow);
        m_createWalletAction->setObjectName(QStringLiteral("m_createWalletAction"));
        m_createWalletAction->setEnabled(true);
        m_openWalletAction = new QAction(MainWindow);
        m_openWalletAction->setObjectName(QStringLiteral("m_openWalletAction"));
        m_openWalletAction->setEnabled(true);
        m_encryptWalletAction = new QAction(MainWindow);
        m_encryptWalletAction->setObjectName(QStringLiteral("m_encryptWalletAction"));
        m_encryptWalletAction->setEnabled(true);
        m_changePasswordAction = new QAction(MainWindow);
        m_changePasswordAction->setObjectName(QStringLiteral("m_changePasswordAction"));
        m_changePasswordAction->setEnabled(true);
        m_aboutIntensecoinAction = new QAction(MainWindow);
        m_aboutIntensecoinAction->setObjectName(QStringLiteral("m_aboutIntensecoinAction"));
        m_aboutIntensecoinAction->setEnabled(true);
        m_aboutQtAction = new QAction(MainWindow);
        m_aboutQtAction->setObjectName(QStringLiteral("m_aboutQtAction"));
        m_aboutQtAction->setEnabled(true);
        m_backupWalletAction = new QAction(MainWindow);
        m_backupWalletAction->setObjectName(QStringLiteral("m_backupWalletAction"));
        m_backupWalletAction->setEnabled(true);
        m_autostartAction = new QAction(MainWindow);
        m_autostartAction->setObjectName(QStringLiteral("m_autostartAction"));
        m_autostartAction->setCheckable(true);
        m_minimizeToTrayAction = new QAction(MainWindow);
        m_minimizeToTrayAction->setObjectName(QStringLiteral("m_minimizeToTrayAction"));
        m_minimizeToTrayAction->setCheckable(true);
        m_closeToTrayAction = new QAction(MainWindow);
        m_closeToTrayAction->setObjectName(QStringLiteral("m_closeToTrayAction"));
        m_closeToTrayAction->setCheckable(true);
        m_preferencesAction = new QAction(MainWindow);
        m_preferencesAction->setObjectName(QStringLiteral("m_preferencesAction"));
        m_recentWalletsAction = new QAction(MainWindow);
        m_recentWalletsAction->setObjectName(QStringLiteral("m_recentWalletsAction"));
        m_exportTrackingKeyAction = new QAction(MainWindow);
        m_exportTrackingKeyAction->setObjectName(QStringLiteral("m_exportTrackingKeyAction"));
        m_importKeyAction = new QAction(MainWindow);
        m_importKeyAction->setObjectName(QStringLiteral("m_importKeyAction"));
        m_communityForumAction = new QAction(MainWindow);
        m_communityForumAction->setObjectName(QStringLiteral("m_communityForumAction"));
        m_reportIssueAction = new QAction(MainWindow);
        m_reportIssueAction->setObjectName(QStringLiteral("m_reportIssueAction"));
        m_resetAction = new QAction(MainWindow);
        m_resetAction->setObjectName(QStringLiteral("m_resetAction"));
        m_saveKeysAction = new QAction(MainWindow);
        m_saveKeysAction->setObjectName(QStringLiteral("m_saveKeysAction"));
        m_exportKeyAction = new QAction(MainWindow);
        m_exportKeyAction->setObjectName(QStringLiteral("m_exportKeyAction"));
		m_exportPrivateKeyAction = new QAction(MainWindow);
		m_exportPrivateKeyAction->setObjectName(QStringLiteral("m_exportPrivateKeyAction"));
        m_removePendingTxAction = new QAction(MainWindow);
        m_removePendingTxAction->setObjectName(QStringLiteral("m_removePendingTxAction"));
        m_removePendingTxAction->setCheckable(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        m_headerFrame = new QFrame(centralwidget);
        m_headerFrame->setObjectName(QStringLiteral("m_headerFrame"));
        m_headerFrame->setMinimumSize(QSize(0, 116));
        m_headerFrame->setMaximumSize(QSize(16777215, 116));
        m_headerFrame->setFrameShape(QFrame::NoFrame);
        m_headerFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(m_headerFrame);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 25, 0);
        horizontalSpacer_2 = new QSpacerItem(30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        m_logoLabel = new QLabel(m_headerFrame);
        m_logoLabel->setObjectName(QStringLiteral("m_logoLabel"));

        horizontalLayout_3->addWidget(m_logoLabel);

        horizontalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        m_noWalletLabel = new WalletGui::WalletSmallGrayTextLabel(m_headerFrame);
        m_noWalletLabel->setObjectName(QStringLiteral("m_noWalletLabel"));

        horizontalLayout_3->addWidget(m_noWalletLabel);

        m_walletFrame = new QFrame(m_headerFrame);
        m_walletFrame->setObjectName(QStringLiteral("m_walletFrame"));
        m_walletFrame->setFrameShape(QFrame::NoFrame);
        m_walletFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(m_walletFrame);
        verticalLayout_3->setSpacing(5);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_3->addItem(verticalSpacer_3);

        m_walletLabel = new WalletGui::WalletSmallGrayTextLabel(m_walletFrame);
        m_walletLabel->setObjectName(QStringLiteral("m_walletLabel"));
        m_walletLabel->setIndent(0);

        verticalLayout_3->addWidget(m_walletLabel);

        m_addressLabel = new QLabel(m_walletFrame);
        m_addressLabel->setObjectName(QStringLiteral("m_addressLabel"));
        sizePolicy.setHeightForWidth(m_addressLabel->sizePolicy().hasHeightForWidth());
        m_addressLabel->setSizePolicy(sizePolicy);
        m_addressLabel->setCursor(QCursor(Qt::PointingHandCursor));
        m_addressLabel->setIndent(0);

        verticalLayout_3->addWidget(m_addressLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        m_notEncryptedFrame = new QFrame(m_walletFrame);
        m_notEncryptedFrame->setObjectName(QStringLiteral("m_notEncryptedFrame"));
        m_notEncryptedFrame->setFrameShape(QFrame::NoFrame);
        m_notEncryptedFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(m_notEncryptedFrame);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        m_notEncryptedIconLabel = new QLabel(m_notEncryptedFrame);
        m_notEncryptedIconLabel->setObjectName(QStringLiteral("m_notEncryptedIconLabel"));
        m_notEncryptedIconLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/lock_open_gray")));
        m_notEncryptedIconLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(m_notEncryptedIconLabel, 0, Qt::AlignVCenter);

        horizontalSpacer_11 = new QSpacerItem(5, 3, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_11);

        m_notEncryptedTextLabel = new WalletGui::WalletTinyGrayTextLabel(m_notEncryptedFrame);
        m_notEncryptedTextLabel->setObjectName(QStringLiteral("m_notEncryptedTextLabel"));
        m_notEncryptedTextLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(m_notEncryptedTextLabel, 0, Qt::AlignVCenter);

        horizontalSpacer_9 = new QSpacerItem(3, 3, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_9);

        m_encryptButton = new WalletGui::WalletTinyLinkLikeButton(m_notEncryptedFrame);
        m_encryptButton->setObjectName(QStringLiteral("m_encryptButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_encryptButton->sizePolicy().hasHeightForWidth());
        m_encryptButton->setSizePolicy(sizePolicy1);
        m_encryptButton->setMinimumSize(QSize(0, 3));
        m_encryptButton->setCursor(QCursor(Qt::PointingHandCursor));
        m_encryptButton->setFlat(true);

        horizontalLayout_4->addWidget(m_encryptButton, 0, Qt::AlignVCenter);

        horizontalSpacer_10 = new QSpacerItem(20, 3, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_10);


        horizontalLayout_2->addWidget(m_notEncryptedFrame, 0, Qt::AlignTop);

        m_copyAddressButton = new WalletGui::WalletTinyLinkLikeButton(m_walletFrame);
        m_copyAddressButton->setObjectName(QStringLiteral("m_copyAddressButton"));
        sizePolicy1.setHeightForWidth(m_copyAddressButton->sizePolicy().hasHeightForWidth());
        m_copyAddressButton->setSizePolicy(sizePolicy1);
        m_copyAddressButton->setMinimumSize(QSize(0, 3));
        m_copyAddressButton->setCursor(QCursor(Qt::PointingHandCursor));
        m_copyAddressButton->setFocusPolicy(Qt::NoFocus);
        m_copyAddressButton->setAutoFillBackground(true);
        m_copyAddressButton->setFlat(true);

        horizontalLayout_2->addWidget(m_copyAddressButton, 0, Qt::AlignVCenter);

        horizontalSpacer_7 = new QSpacerItem(13, 3, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_7);

        m_copyAddressLabel = new WalletGui::CopyMagicLabel(m_walletFrame);
        m_copyAddressLabel->setObjectName(QStringLiteral("m_copyAddressLabel"));
        m_copyAddressLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(m_copyAddressLabel, 0, Qt::AlignVCenter);

        horizontalSpacer = new QSpacerItem(40, 3, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_3->addItem(verticalSpacer_2);


        horizontalLayout_3->addWidget(m_walletFrame);

        horizontalSpacer_3 = new QSpacerItem(34, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        horizontalSpacer_8 = new QSpacerItem(34, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_8);

        m_balanceFrame = new QFrame(m_headerFrame);
        m_balanceFrame->setObjectName(QStringLiteral("m_balanceFrame"));
        gridLayout = new QGridLayout(m_balanceFrame);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setVerticalSpacing(3);
        gridLayout->setContentsMargins(0, 30, 0, 0);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        m_balanceTextLabel = new QLabel(m_balanceFrame);
        m_balanceTextLabel->setObjectName(QStringLiteral("m_balanceTextLabel"));

        verticalLayout_4->addWidget(m_balanceTextLabel);

        m_balanceLabel = new QLabel(m_balanceFrame);
        m_balanceLabel->setObjectName(QStringLiteral("m_balanceLabel"));
        m_balanceLabel->setCursor(QCursor(Qt::PointingHandCursor));

        verticalLayout_4->addWidget(m_balanceLabel);


        gridLayout->addLayout(verticalLayout_4, 0, 2, 1, 1);

        m_balanceCopyLabel = new WalletGui::CopyMagicLabel(m_balanceFrame);
        m_balanceCopyLabel->setObjectName(QStringLiteral("m_balanceCopyLabel"));

        gridLayout->addWidget(m_balanceCopyLabel, 1, 2, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(2, 39, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 0, 1, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_5, 2, 2, 1, 1);

        m_balanceIconLabel = new QLabel(m_balanceFrame);
        m_balanceIconLabel->setObjectName(QStringLiteral("m_balanceIconLabel"));
        m_balanceIconLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/total_balance")));

        gridLayout->addWidget(m_balanceIconLabel, 0, 0, 1, 1);


        horizontalLayout_3->addWidget(m_balanceFrame, 0, Qt::AlignRight|Qt::AlignVCenter);


        verticalLayout_2->addWidget(m_headerFrame);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        m_toolFrame = new QFrame(centralwidget);
        m_toolFrame->setObjectName(QStringLiteral("m_toolFrame"));
        m_toolFrame->setFrameShape(QFrame::NoFrame);
        m_toolFrame->setFrameShadow(QFrame::Plain);
        m_toolFrame->setLineWidth(0);
        verticalLayout = new QVBoxLayout(m_toolFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        m_overviewButton = new QPushButton(m_toolFrame);
        m_toolButtonGroup = new QButtonGroup(MainWindow);
        m_toolButtonGroup->setObjectName(QStringLiteral("m_toolButtonGroup"));
        m_toolButtonGroup->setExclusive(true);
        m_toolButtonGroup->addButton(m_overviewButton);
        m_overviewButton->setObjectName(QStringLiteral("m_overviewButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_overviewButton->sizePolicy().hasHeightForWidth());
        m_overviewButton->setSizePolicy(sizePolicy2);
        m_overviewButton->setFocusPolicy(Qt::NoFocus);
        m_overviewButton->setCheckable(true);

        verticalLayout->addWidget(m_overviewButton);

        m_sendButton = new QPushButton(m_toolFrame);
        m_toolButtonGroup->addButton(m_sendButton);
        m_sendButton->setObjectName(QStringLiteral("m_sendButton"));
        sizePolicy2.setHeightForWidth(m_sendButton->sizePolicy().hasHeightForWidth());
        m_sendButton->setSizePolicy(sizePolicy2);
        m_sendButton->setFocusPolicy(Qt::NoFocus);
        m_sendButton->setCheckable(true);

        verticalLayout->addWidget(m_sendButton);

        m_transactionsButton = new QPushButton(m_toolFrame);
        m_toolButtonGroup->addButton(m_transactionsButton);
        m_transactionsButton->setObjectName(QStringLiteral("m_transactionsButton"));
        sizePolicy2.setHeightForWidth(m_transactionsButton->sizePolicy().hasHeightForWidth());
        m_transactionsButton->setSizePolicy(sizePolicy2);
        m_transactionsButton->setFocusPolicy(Qt::NoFocus);
        m_transactionsButton->setCheckable(true);

        verticalLayout->addWidget(m_transactionsButton);

        m_blockExplorerButton = new QPushButton(m_toolFrame);
        m_toolButtonGroup->addButton(m_blockExplorerButton);
        m_blockExplorerButton->setObjectName(QStringLiteral("m_blockExplorerButton"));
        m_blockExplorerButton->setFocusPolicy(Qt::NoFocus);
        m_blockExplorerButton->setCheckable(true);

        verticalLayout->addWidget(m_blockExplorerButton);

        m_addressBookButton = new QPushButton(m_toolFrame);
        m_toolButtonGroup->addButton(m_addressBookButton);
        m_addressBookButton->setObjectName(QStringLiteral("m_addressBookButton"));
        sizePolicy2.setHeightForWidth(m_addressBookButton->sizePolicy().hasHeightForWidth());
        m_addressBookButton->setSizePolicy(sizePolicy2);
        m_addressBookButton->setFocusPolicy(Qt::NoFocus);
        m_addressBookButton->setCheckable(true);

        verticalLayout->addWidget(m_addressBookButton);

        m_miningButton = new QPushButton(m_toolFrame);
        m_toolButtonGroup->addButton(m_miningButton);
        m_miningButton->setObjectName(QStringLiteral("m_miningButton"));
        m_miningButton->setFocusPolicy(Qt::NoFocus);
        m_miningButton->setCheckable(true);

        verticalLayout->addWidget(m_miningButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(m_toolFrame);

        m_overviewFrame = new WalletGui::OverviewFrame(centralwidget);
        m_overviewFrame->setObjectName(QStringLiteral("m_overviewFrame"));
        m_overviewFrame->setFrameShape(QFrame::NoFrame);
        m_overviewFrame->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(m_overviewFrame);

        m_sendFrame = new WalletGui::SendFrame(centralwidget);
        m_sendFrame->setObjectName(QStringLiteral("m_sendFrame"));
        m_sendFrame->setFrameShape(QFrame::NoFrame);
        m_sendFrame->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(m_sendFrame);

        m_transactionsFrame = new WalletGui::TransactionsFrame(centralwidget);
        m_transactionsFrame->setObjectName(QStringLiteral("m_transactionsFrame"));
        m_transactionsFrame->setFrameShape(QFrame::NoFrame);
        m_transactionsFrame->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(m_transactionsFrame);

        m_addressBookFrame = new WalletGui::AddressBookFrame(centralwidget);
        m_addressBookFrame->setObjectName(QStringLiteral("m_addressBookFrame"));
        m_addressBookFrame->setFrameShape(QFrame::NoFrame);
        m_addressBookFrame->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(m_addressBookFrame);

        m_noWalletFrame = new WalletGui::NoWalletFrame(centralwidget);
        m_noWalletFrame->setObjectName(QStringLiteral("m_noWalletFrame"));
        m_noWalletFrame->setFrameShape(QFrame::NoFrame);
        m_noWalletFrame->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(m_noWalletFrame);

        m_blockExplorerFrame = new WalletGui::BlockExplorerFrame(centralwidget);
        m_blockExplorerFrame->setObjectName(QStringLiteral("m_blockExplorerFrame"));
        m_blockExplorerFrame->setFrameShape(QFrame::NoFrame);
        m_blockExplorerFrame->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(m_blockExplorerFrame);

        m_miningFrame = new WalletGui::MiningFrame(centralwidget);
        m_miningFrame->setObjectName(QStringLiteral("m_miningFrame"));
        m_miningFrame->setFrameShape(QFrame::NoFrame);
        m_miningFrame->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(m_miningFrame);


        verticalLayout_2->addLayout(horizontalLayout);

        m_syncProgress = new QProgressBar(centralwidget);
        m_syncProgress->setObjectName(QStringLiteral("m_syncProgress"));
        m_syncProgress->setMaximum(100);
        m_syncProgress->setValue(0);
        m_syncProgress->setTextVisible(false);

        verticalLayout_2->addWidget(m_syncProgress);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1273, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName(QStringLiteral("menuSettings"));
        menuThemes = new QMenu(menuSettings);
        menuThemes->setObjectName(QStringLiteral("menuThemes"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menubar);
        statusBar = new WalletGui::WalletStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuSettings->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(m_createWalletAction);
        menuFile->addAction(m_openWalletAction);
        menuFile->addAction(m_recentWalletsAction);
        menuFile->addAction(m_backupWalletAction);
        menuFile->addAction(m_saveKeysAction);
        menuFile->addAction(m_resetAction);
        menuFile->addAction(m_importKeyAction);
		menuFile->addAction(m_exportPrivateKeyAction);
        menuFile->addAction(m_exportKeyAction);
        menuFile->addAction(m_exportTrackingKeyAction);
        menuFile->addAction(m_exitAction);
        menuSettings->addAction(m_encryptWalletAction);
        menuSettings->addAction(m_changePasswordAction);
        //menuSettings->addAction(m_removePendingTxAction);
        menuSettings->addSeparator();
        menuSettings->addAction(m_autostartAction);
        menuSettings->addAction(m_minimizeToTrayAction);
        menuSettings->addAction(m_closeToTrayAction);
        menuSettings->addSeparator();
        menuSettings->addAction(m_preferencesAction);
        menuSettings->addAction(menuThemes->menuAction());
        menuHelp->addAction(m_communityForumAction);
        menuHelp->addAction(m_reportIssueAction);
        menuHelp->addAction(m_aboutIntensecoinAction);
        menuHelp->addAction(m_aboutQtAction);

        retranslateUi(MainWindow);
        QObject::connect(m_createWalletAction, SIGNAL(triggered()), MainWindow, SLOT(createWallet()));
        QObject::connect(m_openWalletAction, SIGNAL(triggered()), MainWindow, SLOT(openWallet()));
        QObject::connect(m_encryptWalletAction, SIGNAL(triggered()), MainWindow, SLOT(encryptWallet()));
        QObject::connect(m_changePasswordAction, SIGNAL(triggered()), MainWindow, SLOT(encryptWallet()));
		QObject::connect(m_removePendingTxAction, SIGNAL(triggered()), MainWindow, SLOT(removePendingTx()));
        QObject::connect(m_aboutQtAction, SIGNAL(triggered()), MainWindow, SLOT(aboutQt()));
        QObject::connect(m_backupWalletAction, SIGNAL(triggered()), MainWindow, SLOT(backupWallet()));
        QObject::connect(m_aboutIntensecoinAction, SIGNAL(triggered()), MainWindow, SLOT(about()));
        QObject::connect(m_overviewButton, SIGNAL(toggled(bool)), m_overviewFrame, SLOT(setVisible(bool)));
        QObject::connect(m_transactionsButton, SIGNAL(toggled(bool)), m_transactionsFrame, SLOT(setVisible(bool)));
        QObject::connect(m_addressBookButton, SIGNAL(toggled(bool)), m_addressBookFrame, SLOT(setVisible(bool)));
        QObject::connect(m_sendButton, SIGNAL(toggled(bool)), m_sendFrame, SLOT(setVisible(bool)));
        QObject::connect(m_copyAddressButton, SIGNAL(clicked()), MainWindow, SLOT(copyAddress()));
        QObject::connect(m_autostartAction, SIGNAL(triggered(bool)), MainWindow, SLOT(setStartOnLoginEnabled(bool)));
        QObject::connect(m_minimizeToTrayAction, SIGNAL(triggered(bool)), MainWindow, SLOT(setMinimizeToTrayEnabled(bool)));
        QObject::connect(m_closeToTrayAction, SIGNAL(triggered(bool)), MainWindow, SLOT(setCloseToTrayEnabled(bool)));
        QObject::connect(m_encryptButton, SIGNAL(clicked()), m_encryptWalletAction, SLOT(trigger()));
        QObject::connect(m_preferencesAction, SIGNAL(triggered()), MainWindow, SLOT(showPreferences()));
        QObject::connect(m_blockExplorerButton, SIGNAL(toggled(bool)), m_blockExplorerFrame, SLOT(setVisible(bool)));
        QObject::connect(m_miningButton, SIGNAL(toggled(bool)), m_miningFrame, SLOT(setVisible(bool)));
        QObject::connect(m_exportTrackingKeyAction, SIGNAL(triggered()), MainWindow, SLOT(exportTrackingKey()));
        QObject::connect(m_importKeyAction, SIGNAL(triggered()), MainWindow, SLOT(importKey()));
        QObject::connect(m_communityForumAction, SIGNAL(triggered()), MainWindow, SLOT(communityForumTriggered()));
        QObject::connect(m_reportIssueAction, SIGNAL(triggered()), MainWindow, SLOT(reportIssueTriggered()));
        QObject::connect(m_resetAction, SIGNAL(triggered()), MainWindow, SLOT(resetWallet()));
        QObject::connect(m_saveKeysAction, SIGNAL(triggered()), MainWindow, SLOT(saveWalletKeys()));
        QObject::connect(m_exportKeyAction, SIGNAL(triggered()), MainWindow, SLOT(exportKey()));
		QObject::connect(m_exportPrivateKeyAction, SIGNAL(triggered()), MainWindow, SLOT(exportPrivateKeys()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        m_exitAction->setText(QApplication::translate("MainWindow", "Exit", Q_NULLPTR));
        m_exitAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", Q_NULLPTR));
        m_createWalletAction->setText(QApplication::translate("MainWindow", "Create wallet", Q_NULLPTR));
        m_openWalletAction->setText(QApplication::translate("MainWindow", "Open wallet", Q_NULLPTR));
        m_encryptWalletAction->setText(QApplication::translate("MainWindow", "Encrypt wallet", Q_NULLPTR));
        m_changePasswordAction->setText(QApplication::translate("MainWindow", "Change password", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        m_changePasswordAction->setToolTip(QApplication::translate("MainWindow", "Change password", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        m_aboutIntensecoinAction->setText(QApplication::translate("MainWindow", "About Intensecoin", Q_NULLPTR));
        m_aboutQtAction->setText(QApplication::translate("MainWindow", "About Qt", Q_NULLPTR));
        m_backupWalletAction->setText(QApplication::translate("MainWindow", "Backup wallet", Q_NULLPTR));
        m_autostartAction->setText(QApplication::translate("MainWindow", "Start on system login", Q_NULLPTR));
        m_minimizeToTrayAction->setText(QApplication::translate("MainWindow", "Minimize to tray", Q_NULLPTR));
        m_closeToTrayAction->setText(QApplication::translate("MainWindow", "Close to tray", Q_NULLPTR));
        m_preferencesAction->setText(QApplication::translate("MainWindow", "Preferences", Q_NULLPTR));
        m_recentWalletsAction->setText(QApplication::translate("MainWindow", "Recent wallets", Q_NULLPTR));
        m_exportTrackingKeyAction->setText(QApplication::translate("MainWindow", "Export tracking key", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        m_exportTrackingKeyAction->setToolTip(QApplication::translate("MainWindow", "Export tracking key", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        m_importKeyAction->setText(QApplication::translate("MainWindow", "Import key", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        m_importKeyAction->setToolTip(QApplication::translate("MainWindow", "Import key", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        m_communityForumAction->setText(QApplication::translate("MainWindow", "Join us on Slack", Q_NULLPTR));
        m_reportIssueAction->setText(QApplication::translate("MainWindow", "Report an issue", Q_NULLPTR));
        m_resetAction->setText(QApplication::translate("MainWindow", "Reset wallet", Q_NULLPTR));
        m_saveKeysAction->setText(QApplication::translate("MainWindow", "Save wallet keys", Q_NULLPTR));
        m_exportKeyAction->setText(QApplication::translate("MainWindow", "Export key", Q_NULLPTR));
		m_exportPrivateKeyAction->setText(QApplication::translate("MainWindow", "Export secret key", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        m_exportKeyAction->setToolTip(QApplication::translate("MainWindow", "Export key", Q_NULLPTR));
		m_exportPrivateKeyAction->setToolTip(QApplication::translate("MainWindow", "Export secret key", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        m_removePendingTxAction->setText(QApplication::translate("MainWindow", "Remove pending TXs", Q_NULLPTR));
        m_logoLabel->setText(QString());
        m_noWalletLabel->setText(QApplication::translate("MainWindow", "No active wallet", Q_NULLPTR));
        m_walletLabel->setText(QApplication::translate("MainWindow", "Your wallet:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        m_addressLabel->setToolTip(QApplication::translate("MainWindow", "Click to copy", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        m_addressLabel->setText(QApplication::translate("MainWindow", "ADDRESS", Q_NULLPTR));
        m_notEncryptedIconLabel->setText(QString());
        m_notEncryptedTextLabel->setText(QApplication::translate("MainWindow", "Wallet not encrypted!", Q_NULLPTR));
        m_encryptButton->setText(QApplication::translate("MainWindow", "Set password", Q_NULLPTR));
        m_copyAddressButton->setText(QApplication::translate("MainWindow", "Copy address", Q_NULLPTR));
        m_copyAddressLabel->setText(QApplication::translate("MainWindow", "Address copied to clipboard", Q_NULLPTR));
        m_balanceTextLabel->setText(QApplication::translate("MainWindow", "Total balance", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        m_balanceLabel->setToolTip(QApplication::translate("MainWindow", "Click to copy", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        m_balanceLabel->setText(QApplication::translate("MainWindow", "0.00", Q_NULLPTR));
        m_balanceCopyLabel->setText(QApplication::translate("MainWindow", "Copied!", Q_NULLPTR));
        m_balanceIconLabel->setText(QString());
        m_overviewButton->setText(QApplication::translate("MainWindow", "OVERVIEW", Q_NULLPTR));
        m_sendButton->setText(QApplication::translate("MainWindow", "SEND INTENSECOINS", Q_NULLPTR));
        m_transactionsButton->setText(QApplication::translate("MainWindow", "TRANSACTIONS", Q_NULLPTR));
        m_blockExplorerButton->setText(QApplication::translate("MainWindow", "BLOCK EXPLORER", Q_NULLPTR));
        m_addressBookButton->setText(QApplication::translate("MainWindow", "CONTACTS", Q_NULLPTR));
        m_miningButton->setText(QApplication::translate("MainWindow", "MINING", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuSettings->setTitle(QApplication::translate("MainWindow", "Settings", Q_NULLPTR));
        menuThemes->setTitle(QApplication::translate("MainWindow", "Themes", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
