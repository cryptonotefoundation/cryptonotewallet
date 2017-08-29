/********************************************************************************
** Form generated from reading UI file 'TransactionsFrame.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSACTIONSFRAME_H
#define UI_TRANSACTIONSFRAME_H

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
#include <QtWidgets/QVBoxLayout>
#include "Gui/Common/WalletBlueButton.h"
#include "Gui/Common/WalletLinkLikeButton.h"
#include "Gui/Common/WalletTableView.h"
#include "Gui/Common/WalletTextLabel.h"

QT_BEGIN_NAMESPACE

class Ui_TransactionsFrame
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    WalletGui::WalletHeaderLabel *m_transactionsTextLabel;
    QSpacerItem *horizontalSpacer_4;
    WalletGui::WalletTinyLinkLikeButton *m_resetFilterButton;
    WalletGui::WalletTinyLinkLikeButton *m_filterButton;
    QSpacerItem *verticalSpacer_3;
    QFrame *m_filterFrame;
    QHBoxLayout *horizontalLayout_3;
    QFrame *m_filterPeriodComboFrame;
    QHBoxLayout *horizontalLayout_5;
    WalletGui::WalletNormalGrayTextLabel *m_filterTextLabel;
    QComboBox *m_filterCombo;
    QFrame *m_filterPeriodFrame;
    QHBoxLayout *horizontalLayout_4;
    WalletGui::WalletNormalGrayTextLabel *m_filterBeginTextLabel;
    QDateTimeEdit *m_filterBeginDtedit;
    WalletGui::WalletNormalGrayTextLabel *m_filterEndTextLabel;
    QDateTimeEdit *m_filterEndDtedit;
    QFrame *m_filterHashFrame;
    QHBoxLayout *horizontalLayout_6;
    WalletGui::WalletNormalGrayTextLabel *m_filterHashTextLabel;
    QLineEdit *m_filterHashEdit;
    QFrame *m_filterAddressFrame;
    QHBoxLayout *horizontalLayout_7;
    WalletGui::WalletNormalGrayTextLabel *m_filterAddressTextLabel;
    QLineEdit *m_filterAddressEdit;
    QSpacerItem *verticalSpacer;
    WalletGui::WalletTableView *m_transactionsView;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    WalletGui::WalletNormalBlueButton *m_exportToCsvButton;

    void setupUi(QFrame *TransactionsFrame)
    {
        if (TransactionsFrame->objectName().isEmpty())
            TransactionsFrame->setObjectName(QStringLiteral("TransactionsFrame"));
        TransactionsFrame->resize(894, 638);
        TransactionsFrame->setMinimumSize(QSize(0, 0));
        TransactionsFrame->setFrameShape(QFrame::NoFrame);
        TransactionsFrame->setFrameShadow(QFrame::Plain);
        verticalLayout = new QVBoxLayout(TransactionsFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(25, 23, 25, -1);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, -1, 0, -1);
        m_transactionsTextLabel = new WalletGui::WalletHeaderLabel(TransactionsFrame);
        m_transactionsTextLabel->setObjectName(QStringLiteral("m_transactionsTextLabel"));
        m_transactionsTextLabel->setIndent(0);

        horizontalLayout_2->addWidget(m_transactionsTextLabel);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        m_resetFilterButton = new WalletGui::WalletTinyLinkLikeButton(TransactionsFrame);
        m_resetFilterButton->setObjectName(QStringLiteral("m_resetFilterButton"));
        m_resetFilterButton->setCursor(QCursor(Qt::PointingHandCursor));
        m_resetFilterButton->setFlat(true);

        horizontalLayout_2->addWidget(m_resetFilterButton);

        m_filterButton = new WalletGui::WalletTinyLinkLikeButton(TransactionsFrame);
        m_filterButton->setObjectName(QStringLiteral("m_filterButton"));
        m_filterButton->setCursor(QCursor(Qt::PointingHandCursor));
        m_filterButton->setFocusPolicy(Qt::NoFocus);
        m_filterButton->setCheckable(true);
        m_filterButton->setFlat(true);

        horizontalLayout_2->addWidget(m_filterButton);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_3);

        m_filterFrame = new QFrame(TransactionsFrame);
        m_filterFrame->setObjectName(QStringLiteral("m_filterFrame"));
        m_filterFrame->setMinimumSize(QSize(0, 0));
        m_filterFrame->setFrameShape(QFrame::NoFrame);
        m_filterFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(m_filterFrame);
        horizontalLayout_3->setSpacing(25);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 5, 0, 30);
        m_filterPeriodComboFrame = new QFrame(m_filterFrame);
        m_filterPeriodComboFrame->setObjectName(QStringLiteral("m_filterPeriodComboFrame"));
        m_filterPeriodComboFrame->setFrameShape(QFrame::NoFrame);
        m_filterPeriodComboFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(m_filterPeriodComboFrame);
        horizontalLayout_5->setSpacing(8);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        m_filterTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_filterPeriodComboFrame);
        m_filterTextLabel->setObjectName(QStringLiteral("m_filterTextLabel"));

        horizontalLayout_5->addWidget(m_filterTextLabel);

        m_filterCombo = new QComboBox(m_filterPeriodComboFrame);
        m_filterCombo->setObjectName(QStringLiteral("m_filterCombo"));
        m_filterCombo->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_5->addWidget(m_filterCombo);


        horizontalLayout_3->addWidget(m_filterPeriodComboFrame);

        m_filterPeriodFrame = new QFrame(m_filterFrame);
        m_filterPeriodFrame->setObjectName(QStringLiteral("m_filterPeriodFrame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_filterPeriodFrame->sizePolicy().hasHeightForWidth());
        m_filterPeriodFrame->setSizePolicy(sizePolicy);
        m_filterPeriodFrame->setFrameShape(QFrame::NoFrame);
        m_filterPeriodFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(m_filterPeriodFrame);
        horizontalLayout_4->setSpacing(8);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        m_filterBeginTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_filterPeriodFrame);
        m_filterBeginTextLabel->setObjectName(QStringLiteral("m_filterBeginTextLabel"));

        horizontalLayout_4->addWidget(m_filterBeginTextLabel);

        m_filterBeginDtedit = new QDateTimeEdit(m_filterPeriodFrame);
        m_filterBeginDtedit->setObjectName(QStringLiteral("m_filterBeginDtedit"));
        m_filterBeginDtedit->setEnabled(true);
        m_filterBeginDtedit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        m_filterBeginDtedit->setCalendarPopup(true);

        horizontalLayout_4->addWidget(m_filterBeginDtedit);

        m_filterEndTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_filterPeriodFrame);
        m_filterEndTextLabel->setObjectName(QStringLiteral("m_filterEndTextLabel"));

        horizontalLayout_4->addWidget(m_filterEndTextLabel);

        m_filterEndDtedit = new QDateTimeEdit(m_filterPeriodFrame);
        m_filterEndDtedit->setObjectName(QStringLiteral("m_filterEndDtedit"));
        m_filterEndDtedit->setEnabled(true);
        m_filterEndDtedit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        m_filterEndDtedit->setCalendarPopup(true);

        horizontalLayout_4->addWidget(m_filterEndDtedit);


        horizontalLayout_3->addWidget(m_filterPeriodFrame);

        m_filterHashFrame = new QFrame(m_filterFrame);
        m_filterHashFrame->setObjectName(QStringLiteral("m_filterHashFrame"));
        m_filterHashFrame->setFrameShape(QFrame::NoFrame);
        m_filterHashFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_6 = new QHBoxLayout(m_filterHashFrame);
        horizontalLayout_6->setSpacing(8);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        m_filterHashTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_filterHashFrame);
        m_filterHashTextLabel->setObjectName(QStringLiteral("m_filterHashTextLabel"));

        horizontalLayout_6->addWidget(m_filterHashTextLabel);

        m_filterHashEdit = new QLineEdit(m_filterHashFrame);
        m_filterHashEdit->setObjectName(QStringLiteral("m_filterHashEdit"));

        horizontalLayout_6->addWidget(m_filterHashEdit);


        horizontalLayout_3->addWidget(m_filterHashFrame);

        m_filterAddressFrame = new QFrame(m_filterFrame);
        m_filterAddressFrame->setObjectName(QStringLiteral("m_filterAddressFrame"));
        m_filterAddressFrame->setFrameShape(QFrame::NoFrame);
        m_filterAddressFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_7 = new QHBoxLayout(m_filterAddressFrame);
        horizontalLayout_7->setSpacing(8);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        m_filterAddressTextLabel = new WalletGui::WalletNormalGrayTextLabel(m_filterAddressFrame);
        m_filterAddressTextLabel->setObjectName(QStringLiteral("m_filterAddressTextLabel"));

        horizontalLayout_7->addWidget(m_filterAddressTextLabel);

        m_filterAddressEdit = new QLineEdit(m_filterAddressFrame);
        m_filterAddressEdit->setObjectName(QStringLiteral("m_filterAddressEdit"));

        horizontalLayout_7->addWidget(m_filterAddressEdit);


        horizontalLayout_3->addWidget(m_filterAddressFrame);


        verticalLayout->addWidget(m_filterFrame);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        m_transactionsView = new WalletGui::WalletTableView(TransactionsFrame);
        m_transactionsView->setObjectName(QStringLiteral("m_transactionsView"));
        m_transactionsView->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_transactionsView->sizePolicy().hasHeightForWidth());
        m_transactionsView->setSizePolicy(sizePolicy1);
        m_transactionsView->setFocusPolicy(Qt::NoFocus);
        m_transactionsView->setFrameShape(QFrame::NoFrame);
        m_transactionsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_transactionsView->setTabKeyNavigation(false);
        m_transactionsView->setDragDropOverwriteMode(false);
        m_transactionsView->setAlternatingRowColors(true);
        m_transactionsView->setSelectionMode(QAbstractItemView::NoSelection);
        m_transactionsView->setSelectionBehavior(QAbstractItemView::SelectItems);
        m_transactionsView->setTextElideMode(Qt::ElideMiddle);
        m_transactionsView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        m_transactionsView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        m_transactionsView->setShowGrid(false);
        m_transactionsView->setGridStyle(Qt::NoPen);
        m_transactionsView->setWordWrap(false);
        m_transactionsView->setCornerButtonEnabled(false);
        m_transactionsView->horizontalHeader()->setStretchLastSection(false);
        m_transactionsView->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(m_transactionsView);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_exportToCsvButton = new WalletGui::WalletNormalBlueButton(TransactionsFrame);
        m_exportToCsvButton->setObjectName(QStringLiteral("m_exportToCsvButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_exportToCsvButton->sizePolicy().hasHeightForWidth());
        m_exportToCsvButton->setSizePolicy(sizePolicy2);
        m_exportToCsvButton->setMinimumSize(QSize(110, 30));
        m_exportToCsvButton->setMaximumSize(QSize(110, 30));
        m_exportToCsvButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(m_exportToCsvButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(TransactionsFrame);
        QObject::connect(m_exportToCsvButton, SIGNAL(clicked()), TransactionsFrame, SLOT(exportToCsv()));
        QObject::connect(m_filterCombo, SIGNAL(currentIndexChanged(int)), TransactionsFrame, SLOT(filterChanged(int)));
        QObject::connect(m_filterEndDtedit, SIGNAL(dateTimeChanged(QDateTime)), TransactionsFrame, SLOT(filterPeriodChanged(QDateTime)));
        QObject::connect(m_filterHashEdit, SIGNAL(textChanged(QString)), TransactionsFrame, SLOT(filterHashChanged(QString)));
        QObject::connect(m_filterAddressEdit, SIGNAL(textChanged(QString)), TransactionsFrame, SLOT(filterAddressChanged(QString)));
        QObject::connect(m_filterBeginDtedit, SIGNAL(dateTimeChanged(QDateTime)), TransactionsFrame, SLOT(filterPeriodChanged(QDateTime)));
        QObject::connect(m_filterButton, SIGNAL(toggled(bool)), TransactionsFrame, SLOT(showFilter(bool)));
        QObject::connect(m_resetFilterButton, SIGNAL(clicked()), TransactionsFrame, SLOT(resetClicked()));
        QObject::connect(m_filterButton, SIGNAL(toggled(bool)), m_resetFilterButton, SLOT(setVisible(bool)));
        QObject::connect(m_transactionsView, SIGNAL(doubleClicked(QModelIndex)), TransactionsFrame, SLOT(transactionDoubleClicked(QModelIndex)));
        QObject::connect(m_transactionsView, SIGNAL(clicked(QModelIndex)), TransactionsFrame, SLOT(transactionClicked(QModelIndex)));

        QMetaObject::connectSlotsByName(TransactionsFrame);
    } // setupUi

    void retranslateUi(QFrame *TransactionsFrame)
    {
        TransactionsFrame->setWindowTitle(QApplication::translate("TransactionsFrame", "Frame", 0));
        m_transactionsTextLabel->setText(QApplication::translate("TransactionsFrame", "Transactions", 0));
        m_resetFilterButton->setText(QApplication::translate("TransactionsFrame", "Reset", 0));
        m_filterButton->setText(QApplication::translate("TransactionsFrame", "Show filter", 0));
        m_filterTextLabel->setText(QApplication::translate("TransactionsFrame", "PERIOD", 0));
        m_filterCombo->clear();
        m_filterCombo->insertItems(0, QStringList()
         << QApplication::translate("TransactionsFrame", "All time", 0)
         << QApplication::translate("TransactionsFrame", "Last hour", 0)
         << QApplication::translate("TransactionsFrame", "Last day", 0)
         << QApplication::translate("TransactionsFrame", "Last week", 0)
         << QApplication::translate("TransactionsFrame", "Last month", 0)
         << QApplication::translate("TransactionsFrame", "Last year", 0)
         << QApplication::translate("TransactionsFrame", "Custom", 0)
        );
        m_filterBeginTextLabel->setText(QApplication::translate("TransactionsFrame", "FROM", 0));
        m_filterBeginDtedit->setDisplayFormat(QApplication::translate("TransactionsFrame", "dd MMM yyyy, HH:mm", 0));
        m_filterEndTextLabel->setText(QApplication::translate("TransactionsFrame", "TO", 0));
        m_filterEndDtedit->setDisplayFormat(QApplication::translate("TransactionsFrame", "dd MMM yyyy, hh:mm", 0));
        m_filterHashTextLabel->setText(QApplication::translate("TransactionsFrame", "HASH", 0));
        m_filterAddressTextLabel->setText(QApplication::translate("TransactionsFrame", "ADDRESS", 0));
        m_exportToCsvButton->setText(QApplication::translate("TransactionsFrame", "Save as CSV", 0));
    } // retranslateUi

};

namespace Ui {
    class TransactionsFrame: public Ui_TransactionsFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSACTIONSFRAME_H
