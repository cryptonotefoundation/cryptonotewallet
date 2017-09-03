/********************************************************************************
** Form generated from reading UI file 'AddressBookFrame.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDRESSBOOKFRAME_H
#define UI_ADDRESSBOOKFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "Gui/Common/WalletBlueButton.h"
#include "Gui/Common/WalletTextLabel.h"
#include "Gui/Common/WalletTreeView.h"

QT_BEGIN_NAMESPACE

class Ui_AddressBookFrame
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    WalletGui::WalletHeaderLabel *m_addressBookTextLabel;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    WalletGui::WalletTreeView *m_addressBookView;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    WalletGui::WalletLargeBlueButton *m_addAddressButton;

    void setupUi(QFrame *AddressBookFrame)
    {
        if (AddressBookFrame->objectName().isEmpty())
            AddressBookFrame->setObjectName(QStringLiteral("AddressBookFrame"));
        AddressBookFrame->resize(874, 585);
        AddressBookFrame->setFrameShape(QFrame::NoFrame);
        AddressBookFrame->setFrameShadow(QFrame::Plain);
        verticalLayout = new QVBoxLayout(AddressBookFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(25, 23, 25, 23);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        m_addressBookTextLabel = new WalletGui::WalletHeaderLabel(AddressBookFrame);
        m_addressBookTextLabel->setObjectName(QStringLiteral("m_addressBookTextLabel"));

        horizontalLayout_2->addWidget(m_addressBookTextLabel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        m_addressBookView = new WalletGui::WalletTreeView(AddressBookFrame);
        m_addressBookView->setObjectName(QStringLiteral("m_addressBookView"));
        m_addressBookView->setFocusPolicy(Qt::NoFocus);
        m_addressBookView->setContextMenuPolicy(Qt::CustomContextMenu);
        m_addressBookView->setFrameShape(QFrame::NoFrame);
        m_addressBookView->setAlternatingRowColors(true);
        m_addressBookView->setSelectionMode(QAbstractItemView::NoSelection);
        m_addressBookView->setTextElideMode(Qt::ElideMiddle);
        m_addressBookView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        m_addressBookView->setRootIsDecorated(false);
        m_addressBookView->setItemsExpandable(false);
        m_addressBookView->setExpandsOnDoubleClick(false);
        m_addressBookView->header()->setStretchLastSection(false);

        verticalLayout->addWidget(m_addressBookView);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_addAddressButton = new WalletGui::WalletLargeBlueButton(AddressBookFrame);
        m_addAddressButton->setObjectName(QStringLiteral("m_addAddressButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_addAddressButton->sizePolicy().hasHeightForWidth());
        m_addAddressButton->setSizePolicy(sizePolicy);
        m_addAddressButton->setMinimumSize(QSize(218, 40));
        m_addAddressButton->setMaximumSize(QSize(218, 40));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/add"), QSize(), QIcon::Normal, QIcon::Off);
        m_addAddressButton->setIcon(icon);

        horizontalLayout->addWidget(m_addAddressButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(AddressBookFrame);
        QObject::connect(m_addAddressButton, SIGNAL(clicked()), AddressBookFrame, SLOT(addClicked()));
        QObject::connect(m_addressBookView, SIGNAL(customContextMenuRequested(QPoint)), AddressBookFrame, SLOT(contextMenu(QPoint)));

        QMetaObject::connectSlotsByName(AddressBookFrame);
    } // setupUi

    void retranslateUi(QFrame *AddressBookFrame)
    {
        AddressBookFrame->setWindowTitle(QApplication::translate("AddressBookFrame", "Frame", 0));
        m_addressBookTextLabel->setText(QApplication::translate("AddressBookFrame", "Contacts", 0));
        m_addAddressButton->setText(QApplication::translate("AddressBookFrame", "Add contact", 0));
    } // retranslateUi

};

namespace Ui {
    class AddressBookFrame: public Ui_AddressBookFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDRESSBOOKFRAME_H
