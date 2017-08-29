/********************************************************************************
** Form generated from reading UI file 'BlockExplorerFrame.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLOCKEXPLORERFRAME_H
#define UI_BLOCKEXPLORERFRAME_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Gui/Common/WalletLinkLikeButton.h"
#include "Gui/Common/WalletNavigationButton.h"
#include "Gui/Common/WalletOkButton.h"
#include "Gui/Common/WalletTableView.h"
#include "Gui/Common/WalletTextLabel.h"

QT_BEGIN_NAMESPACE

class Ui_BlockExplorerFrame
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *m_blockExplorerTabBar;
    QWidget *m_blockchainTab;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    WalletGui::WalletTinyLinkLikeButton *m_resetFilterButton;
    WalletGui::WalletTinyLinkLikeButton *m_blockSearchButton;
    QSpacerItem *verticalSpacer_2;
    QFrame *m_filterFrame;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_7;
    QFrame *m_filterPeriodComboFrame;
    QHBoxLayout *m_filter_combo_frame;
    WalletGui::WalletNormalGrayTextLabel *m_filterTextLabel;
    QComboBox *m_filterCombo;
    QFrame *m_filterPeriodFrame;
    QHBoxLayout *m_filter_period_frame_layout;
    WalletGui::WalletNormalGrayTextLabel *m_filterBeginTextLabel;
    QDateTimeEdit *m_filterBeginDtedit;
    WalletGui::WalletNormalGrayTextLabel *m_filterEndTextLabel;
    QDateTimeEdit *m_filterEndDtedit;
    QFrame *m_filterHashFrame;
    QHBoxLayout *m_filer_hash_frame;
    WalletGui::WalletNormalGrayTextLabel *m_filterHashTextLabel;
    QLineEdit *m_filterHashEdit;
    WalletGui::WalletOkButton *m_applyFilterButton;
    QSpacerItem *verticalSpacer;
    WalletGui::WalletTableView *m_blockExplorerView;
    QHBoxLayout *horizontalLayout;
    WalletGui::WalletNavigationButton *m_prevPageButton;
    QSpacerItem *horizontalSpacer;
    WalletGui::WalletNavigationButton *m_nextPageButton;
    WalletGui::WalletNavigationButton *m_topButton;
    QWidget *m_transactionPoolTab;
    QVBoxLayout *verticalLayout_3;
    WalletGui::WalletTableView *m_transactionPoolView;

    void setupUi(QFrame *BlockExplorerFrame)
    {
        if (BlockExplorerFrame->objectName().isEmpty())
            BlockExplorerFrame->setObjectName(QStringLiteral("BlockExplorerFrame"));
        BlockExplorerFrame->resize(950, 627);
        BlockExplorerFrame->setFrameShape(QFrame::StyledPanel);
        BlockExplorerFrame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(BlockExplorerFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 12, 0, 0);
        m_blockExplorerTabBar = new QTabWidget(BlockExplorerFrame);
        m_blockExplorerTabBar->setObjectName(QStringLiteral("m_blockExplorerTabBar"));
        m_blockchainTab = new QWidget();
        m_blockchainTab->setObjectName(QStringLiteral("m_blockchainTab"));
        verticalLayout_4 = new QVBoxLayout(m_blockchainTab);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(25, 15, 25, 12);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, 0, -1);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        m_resetFilterButton = new WalletGui::WalletTinyLinkLikeButton(m_blockchainTab);
        m_resetFilterButton->setObjectName(QStringLiteral("m_resetFilterButton"));
        m_resetFilterButton->setCursor(QCursor(Qt::PointingHandCursor));
        m_resetFilterButton->setFlat(true);

        horizontalLayout_2->addWidget(m_resetFilterButton, 0, Qt::AlignBottom);

        m_blockSearchButton = new WalletGui::WalletTinyLinkLikeButton(m_blockchainTab);
        m_blockSearchButton->setObjectName(QStringLiteral("m_blockSearchButton"));
        m_blockSearchButton->setCursor(QCursor(Qt::PointingHandCursor));
        m_blockSearchButton->setCheckable(true);
        m_blockSearchButton->setFlat(true);

        horizontalLayout_2->addWidget(m_blockSearchButton, 0, Qt::AlignBottom);


        verticalLayout_4->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 7, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer_2);

        m_filterFrame = new QFrame(m_blockchainTab);
        m_filterFrame->setObjectName(QStringLiteral("m_filterFrame"));
        m_filterFrame->setMinimumSize(QSize(0, 0));
        m_filterFrame->setFrameShape(QFrame::NoFrame);
        m_filterFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(m_filterFrame);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 5, 0, 30);
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(0);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(-1, -1, 0, -1);
        m_filterPeriodComboFrame = new QFrame(m_filterFrame);
        m_filterPeriodComboFrame->setObjectName(QStringLiteral("m_filterPeriodComboFrame"));
        m_filterPeriodComboFrame->setFrameShape(QFrame::NoFrame);
        m_filterPeriodComboFrame->setFrameShadow(QFrame::Raised);
        m_filter_combo_frame = new QHBoxLayout(m_filterPeriodComboFrame);
        m_filter_combo_frame->setSpacing(8);
        m_filter_combo_frame->setObjectName(QStringLiteral("m_filter_combo_frame"));
        m_filter_combo_frame->setContentsMargins(0, 0, 25, 0);
        m_filterTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_filterPeriodComboFrame);
        m_filterTextLabel->setObjectName(QStringLiteral("m_filterTextLabel"));

        m_filter_combo_frame->addWidget(m_filterTextLabel);

        m_filterCombo = new QComboBox(m_filterPeriodComboFrame);
        m_filterCombo->setObjectName(QStringLiteral("m_filterCombo"));
        m_filterCombo->setFocusPolicy(Qt::NoFocus);

        m_filter_combo_frame->addWidget(m_filterCombo);


        horizontalLayout_7->addWidget(m_filterPeriodComboFrame);

        m_filterPeriodFrame = new QFrame(m_filterFrame);
        m_filterPeriodFrame->setObjectName(QStringLiteral("m_filterPeriodFrame"));
        m_filterPeriodFrame->setFrameShape(QFrame::NoFrame);
        m_filterPeriodFrame->setFrameShadow(QFrame::Raised);
        m_filter_period_frame_layout = new QHBoxLayout(m_filterPeriodFrame);
        m_filter_period_frame_layout->setSpacing(8);
        m_filter_period_frame_layout->setObjectName(QStringLiteral("m_filter_period_frame_layout"));
        m_filter_period_frame_layout->setContentsMargins(0, 0, 25, 0);
        m_filterBeginTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_filterPeriodFrame);
        m_filterBeginTextLabel->setObjectName(QStringLiteral("m_filterBeginTextLabel"));

        m_filter_period_frame_layout->addWidget(m_filterBeginTextLabel);

        m_filterBeginDtedit = new QDateTimeEdit(m_filterPeriodFrame);
        m_filterBeginDtedit->setObjectName(QStringLiteral("m_filterBeginDtedit"));
        m_filterBeginDtedit->setEnabled(true);
        m_filterBeginDtedit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        m_filterBeginDtedit->setCalendarPopup(true);
        m_filterBeginDtedit->setTimeSpec(Qt::UTC);

        m_filter_period_frame_layout->addWidget(m_filterBeginDtedit);

        m_filterEndTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_filterPeriodFrame);
        m_filterEndTextLabel->setObjectName(QStringLiteral("m_filterEndTextLabel"));

        m_filter_period_frame_layout->addWidget(m_filterEndTextLabel);

        m_filterEndDtedit = new QDateTimeEdit(m_filterPeriodFrame);
        m_filterEndDtedit->setObjectName(QStringLiteral("m_filterEndDtedit"));
        m_filterEndDtedit->setEnabled(true);
        m_filterEndDtedit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        m_filterEndDtedit->setCalendarPopup(true);
        m_filterEndDtedit->setTimeSpec(Qt::UTC);

        m_filter_period_frame_layout->addWidget(m_filterEndDtedit);


        horizontalLayout_7->addWidget(m_filterPeriodFrame);

        m_filterHashFrame = new QFrame(m_filterFrame);
        m_filterHashFrame->setObjectName(QStringLiteral("m_filterHashFrame"));
        m_filterHashFrame->setFrameShape(QFrame::NoFrame);
        m_filterHashFrame->setFrameShadow(QFrame::Raised);
        m_filer_hash_frame = new QHBoxLayout(m_filterHashFrame);
        m_filer_hash_frame->setSpacing(8);
        m_filer_hash_frame->setObjectName(QStringLiteral("m_filer_hash_frame"));
        m_filer_hash_frame->setContentsMargins(0, 0, 10, 0);
        m_filterHashTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_filterHashFrame);
        m_filterHashTextLabel->setObjectName(QStringLiteral("m_filterHashTextLabel"));

        m_filer_hash_frame->addWidget(m_filterHashTextLabel);

        m_filterHashEdit = new QLineEdit(m_filterHashFrame);
        m_filterHashEdit->setObjectName(QStringLiteral("m_filterHashEdit"));

        m_filer_hash_frame->addWidget(m_filterHashEdit);


        horizontalLayout_7->addWidget(m_filterHashFrame);

        m_applyFilterButton = new WalletGui::WalletOkButton(m_filterFrame);
        m_applyFilterButton->setObjectName(QStringLiteral("m_applyFilterButton"));

        horizontalLayout_7->addWidget(m_applyFilterButton);


        verticalLayout_2->addLayout(horizontalLayout_7);


        verticalLayout_4->addWidget(m_filterFrame);

        verticalSpacer = new QSpacerItem(20, 7, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer);

        m_blockExplorerView = new WalletGui::WalletTableView(m_blockchainTab);
        m_blockExplorerView->setObjectName(QStringLiteral("m_blockExplorerView"));
        m_blockExplorerView->setFocusPolicy(Qt::NoFocus);
        m_blockExplorerView->setFrameShape(QFrame::NoFrame);
        m_blockExplorerView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_blockExplorerView->setAlternatingRowColors(true);
        m_blockExplorerView->setSelectionMode(QAbstractItemView::NoSelection);
        m_blockExplorerView->setSelectionBehavior(QAbstractItemView::SelectItems);
        m_blockExplorerView->setTextElideMode(Qt::ElideMiddle);
        m_blockExplorerView->setShowGrid(false);
        m_blockExplorerView->setWordWrap(false);
        m_blockExplorerView->setCornerButtonEnabled(false);
        m_blockExplorerView->horizontalHeader()->setHighlightSections(false);
        m_blockExplorerView->verticalHeader()->setVisible(false);

        verticalLayout_4->addWidget(m_blockExplorerView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 5, 0, -1);
        m_prevPageButton = new WalletGui::WalletNavigationButton(m_blockchainTab);
        m_prevPageButton->setObjectName(QStringLiteral("m_prevPageButton"));

        horizontalLayout->addWidget(m_prevPageButton);

        horizontalSpacer = new QSpacerItem(40, 30, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_nextPageButton = new WalletGui::WalletNavigationButton(m_blockchainTab);
        m_nextPageButton->setObjectName(QStringLiteral("m_nextPageButton"));

        horizontalLayout->addWidget(m_nextPageButton);

        m_topButton = new WalletGui::WalletNavigationButton(m_blockchainTab);
        m_topButton->setObjectName(QStringLiteral("m_topButton"));

        horizontalLayout->addWidget(m_topButton);


        verticalLayout_4->addLayout(horizontalLayout);

        m_blockExplorerTabBar->addTab(m_blockchainTab, QString());
        m_transactionPoolTab = new QWidget();
        m_transactionPoolTab->setObjectName(QStringLiteral("m_transactionPoolTab"));
        verticalLayout_3 = new QVBoxLayout(m_transactionPoolTab);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(25, 23, 25, 12);
        m_transactionPoolView = new WalletGui::WalletTableView(m_transactionPoolTab);
        m_transactionPoolView->setObjectName(QStringLiteral("m_transactionPoolView"));
        m_transactionPoolView->setFocusPolicy(Qt::NoFocus);
        m_transactionPoolView->setFrameShape(QFrame::NoFrame);
        m_transactionPoolView->setSelectionMode(QAbstractItemView::NoSelection);
        m_transactionPoolView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        m_transactionPoolView->setShowGrid(false);
        m_transactionPoolView->setWordWrap(false);
        m_transactionPoolView->setCornerButtonEnabled(false);
        m_transactionPoolView->horizontalHeader()->setHighlightSections(false);
        m_transactionPoolView->verticalHeader()->setVisible(false);

        verticalLayout_3->addWidget(m_transactionPoolView);

        m_blockExplorerTabBar->addTab(m_transactionPoolTab, QString());

        verticalLayout->addWidget(m_blockExplorerTabBar);


        retranslateUi(BlockExplorerFrame);
        QObject::connect(m_nextPageButton, SIGNAL(clicked()), BlockExplorerFrame, SLOT(nextPage()));
        QObject::connect(m_prevPageButton, SIGNAL(clicked()), BlockExplorerFrame, SLOT(prevPage()));
        QObject::connect(m_blockSearchButton, SIGNAL(toggled(bool)), BlockExplorerFrame, SLOT(showSearch(bool)));
        QObject::connect(m_applyFilterButton, SIGNAL(clicked()), BlockExplorerFrame, SLOT(applyFilter()));
        QObject::connect(m_resetFilterButton, SIGNAL(clicked()), BlockExplorerFrame, SLOT(resetFilterClicked()));
        QObject::connect(m_filterCombo, SIGNAL(currentIndexChanged(int)), BlockExplorerFrame, SLOT(filterChanged(int)));
        QObject::connect(m_topButton, SIGNAL(clicked()), BlockExplorerFrame, SLOT(lastPage()));
        QObject::connect(m_filterHashEdit, SIGNAL(returnPressed()), m_applyFilterButton, SLOT(click()));
        QObject::connect(m_blockExplorerView, SIGNAL(doubleClicked(QModelIndex)), BlockExplorerFrame, SLOT(showBlockDetails(QModelIndex)));
        QObject::connect(m_transactionPoolView, SIGNAL(doubleClicked(QModelIndex)), BlockExplorerFrame, SLOT(showTransactionDetails(QModelIndex)));

        m_blockExplorerTabBar->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(BlockExplorerFrame);
    } // setupUi

    void retranslateUi(QFrame *BlockExplorerFrame)
    {
        BlockExplorerFrame->setWindowTitle(QApplication::translate("BlockExplorerFrame", "Frame", 0));
        m_resetFilterButton->setText(QApplication::translate("BlockExplorerFrame", "Reset", 0));
        m_blockSearchButton->setText(QApplication::translate("BlockExplorerFrame", "Show search", 0));
        m_filterTextLabel->setText(QApplication::translate("BlockExplorerFrame", "PERIOD", 0));
        m_filterCombo->clear();
        m_filterCombo->insertItems(0, QStringList()
         << QApplication::translate("BlockExplorerFrame", "All time", 0)
         << QApplication::translate("BlockExplorerFrame", "Last hour", 0)
         << QApplication::translate("BlockExplorerFrame", "Last day", 0)
         << QApplication::translate("BlockExplorerFrame", "Last week", 0)
         << QApplication::translate("BlockExplorerFrame", "Last month", 0)
         << QApplication::translate("BlockExplorerFrame", "Last year", 0)
         << QApplication::translate("BlockExplorerFrame", "Custom", 0)
        );
        m_filterBeginTextLabel->setText(QApplication::translate("BlockExplorerFrame", "FROM", 0));
        m_filterBeginDtedit->setDisplayFormat(QApplication::translate("BlockExplorerFrame", "dd MMM yyyy, hh:mm", 0));
        m_filterEndTextLabel->setText(QApplication::translate("BlockExplorerFrame", "TO", 0));
        m_filterEndDtedit->setDisplayFormat(QApplication::translate("BlockExplorerFrame", "dd MMM yyyy, hh:mm", 0));
        m_filterHashTextLabel->setText(QApplication::translate("BlockExplorerFrame", "HASH / HEIGHT", 0));
        m_applyFilterButton->setText(QApplication::translate("BlockExplorerFrame", "Apply", 0));
        m_prevPageButton->setText(QString());
        m_nextPageButton->setText(QString());
        m_topButton->setText(QApplication::translate("BlockExplorerFrame", "Back to Top", 0));
        m_blockExplorerTabBar->setTabText(m_blockExplorerTabBar->indexOf(m_blockchainTab), QApplication::translate("BlockExplorerFrame", "Blockchain", 0));
        m_blockExplorerTabBar->setTabText(m_blockExplorerTabBar->indexOf(m_transactionPoolTab), QApplication::translate("BlockExplorerFrame", "Transaction pool", 0));
    } // retranslateUi

};

namespace Ui {
    class BlockExplorerFrame: public Ui_BlockExplorerFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLOCKEXPLORERFRAME_H
