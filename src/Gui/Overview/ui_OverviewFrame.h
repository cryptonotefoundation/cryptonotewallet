/********************************************************************************
** Form generated from reading UI file 'OverviewFrame.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OVERVIEWFRAME_H
#define UI_OVERVIEWFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "Gui/Common/NewsFrame.h"
#include "Gui/Common/WalletLinkLikeButton.h"
#include "Gui/Common/WalletTableView.h"
#include "Gui/Common/WalletTextLabel.h"
#include "Gui/Overview/OverviewHeaderFrame.h"

QT_BEGIN_NAMESPACE

class Ui_OverviewFrame
{
public:
    QVBoxLayout *verticalLayout;
    WalletGui::OverviewHeaderFrame *m_overviewHeaderFrame;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    WalletGui::WalletHeaderLabel *m_overviewTransactionsTextLabel;
    WalletGui::WalletTinyLinkLikeButton *m_allTransactionsButton;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    WalletGui::WalletTableView *m_recentTransactionsView;
    QSpacerItem *horizontalSpacer_9;
    QFrame *m_newsFrame;
    QHBoxLayout *horizontalLayout_4;
    WalletGui::NewsFrame *m_blogFrame;

    void setupUi(QFrame *OverviewFrame)
    {
        if (OverviewFrame->objectName().isEmpty())
            OverviewFrame->setObjectName(QStringLiteral("OverviewFrame"));
        OverviewFrame->resize(857, 557);
        OverviewFrame->setFrameShape(QFrame::StyledPanel);
        OverviewFrame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(OverviewFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        m_overviewHeaderFrame = new WalletGui::OverviewHeaderFrame(OverviewFrame);
        m_overviewHeaderFrame->setObjectName(QStringLiteral("m_overviewHeaderFrame"));
        m_overviewHeaderFrame->setFrameShape(QFrame::NoFrame);
        m_overviewHeaderFrame->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(m_overviewHeaderFrame);

        verticalSpacer_2 = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(25, -1, -1, -1);
        m_overviewTransactionsTextLabel = new WalletGui::WalletHeaderLabel(OverviewFrame);
        m_overviewTransactionsTextLabel->setObjectName(QStringLiteral("m_overviewTransactionsTextLabel"));
        m_overviewTransactionsTextLabel->setIndent(0);

        horizontalLayout->addWidget(m_overviewTransactionsTextLabel);

        m_allTransactionsButton = new WalletGui::WalletTinyLinkLikeButton(OverviewFrame);
        m_allTransactionsButton->setObjectName(QStringLiteral("m_allTransactionsButton"));
        m_allTransactionsButton->setCursor(QCursor(Qt::PointingHandCursor));
        m_allTransactionsButton->setFocusPolicy(Qt::NoFocus);
        m_allTransactionsButton->setFlat(true);

        horizontalLayout->addWidget(m_allTransactionsButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(25, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        m_recentTransactionsView = new WalletGui::WalletTableView(OverviewFrame);
        m_recentTransactionsView->setObjectName(QStringLiteral("m_recentTransactionsView"));
        m_recentTransactionsView->setEnabled(true);
        m_recentTransactionsView->setFocusPolicy(Qt::NoFocus);
        m_recentTransactionsView->setFrameShape(QFrame::NoFrame);
        m_recentTransactionsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_recentTransactionsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_recentTransactionsView->setAlternatingRowColors(true);
        m_recentTransactionsView->setSelectionMode(QAbstractItemView::NoSelection);
        m_recentTransactionsView->setSelectionBehavior(QAbstractItemView::SelectItems);
        m_recentTransactionsView->setTextElideMode(Qt::ElideMiddle);
        m_recentTransactionsView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        m_recentTransactionsView->setShowGrid(false);
        m_recentTransactionsView->setWordWrap(false);
        m_recentTransactionsView->setCornerButtonEnabled(false);
        m_recentTransactionsView->horizontalHeader()->setHighlightSections(false);
        m_recentTransactionsView->verticalHeader()->setVisible(false);

        horizontalLayout_3->addWidget(m_recentTransactionsView);

        horizontalSpacer_9 = new QSpacerItem(25, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_9);


        verticalLayout->addLayout(horizontalLayout_3);

        m_newsFrame = new QFrame(OverviewFrame);
        m_newsFrame->setObjectName(QStringLiteral("m_newsFrame"));
        m_newsFrame->setMinimumSize(QSize(0, 100));
        m_newsFrame->setFrameShape(QFrame::StyledPanel);
        m_newsFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(m_newsFrame);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        m_blogFrame = new WalletGui::NewsFrame(m_newsFrame);
        m_blogFrame->setObjectName(QStringLiteral("m_blogFrame"));
        m_blogFrame->setFrameShape(QFrame::NoFrame);
        m_blogFrame->setFrameShadow(QFrame::Raised);

        horizontalLayout_4->addWidget(m_blogFrame);


        verticalLayout->addWidget(m_newsFrame);

        verticalLayout->setStretch(4, 1);

        retranslateUi(OverviewFrame);
        QObject::connect(m_recentTransactionsView, SIGNAL(doubleClicked(QModelIndex)), OverviewFrame, SLOT(showTransactionDetails(QModelIndex)));

        QMetaObject::connectSlotsByName(OverviewFrame);
    } // setupUi

    void retranslateUi(QFrame *OverviewFrame)
    {
        OverviewFrame->setWindowTitle(QApplication::translate("OverviewFrame", "Frame", 0));
        m_overviewTransactionsTextLabel->setText(QApplication::translate("OverviewFrame", "Recent transactions", 0));
        m_allTransactionsButton->setText(QApplication::translate("OverviewFrame", "See full history", 0));
    } // retranslateUi

};

namespace Ui {
    class OverviewFrame: public Ui_OverviewFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OVERVIEWFRAME_H
