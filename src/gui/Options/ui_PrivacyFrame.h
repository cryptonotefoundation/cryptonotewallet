/********************************************************************************
** Form generated from reading UI file 'PrivacyFrame.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRIVACYFRAME_H
#define UI_PRIVACYFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Gui/Common/WalletTextLabel.h"

QT_BEGIN_NAMESPACE

class Ui_PrivacyFrame
{
public:
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *m_newsCheck;
    WalletGui::WalletSmallGrayTextLabel *m_embeddedHelperLabel;
    QSpacerItem *verticalSpacer;

    void setupUi(QFrame *PrivacyFrame)
    {
        if (PrivacyFrame->objectName().isEmpty())
            PrivacyFrame->setObjectName(QStringLiteral("PrivacyFrame"));
        PrivacyFrame->resize(695, 368);
        PrivacyFrame->setFrameShape(QFrame::StyledPanel);
        PrivacyFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(PrivacyFrame);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(25, 40, 49, -1);
        widget_2 = new QWidget(PrivacyFrame);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        widget_2->setMinimumSize(QSize(0, 70));
        widget_2->setMaximumSize(QSize(16777215, 70));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        m_newsCheck = new QCheckBox(widget_2);
        m_newsCheck->setObjectName(QStringLiteral("m_newsCheck"));

        verticalLayout_2->addWidget(m_newsCheck);

        m_embeddedHelperLabel = new WalletGui::WalletSmallGrayTextLabel(widget_2);
        m_embeddedHelperLabel->setObjectName(QStringLiteral("m_embeddedHelperLabel"));
        m_embeddedHelperLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        m_embeddedHelperLabel->setWordWrap(true);
        m_embeddedHelperLabel->setIndent(0);

        verticalLayout_2->addWidget(m_embeddedHelperLabel, 0, Qt::AlignTop);

        verticalLayout_2->setStretch(1, 1);

        verticalLayout_3->addWidget(widget_2);

        verticalSpacer = new QSpacerItem(20, 171, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        retranslateUi(PrivacyFrame);

        QMetaObject::connectSlotsByName(PrivacyFrame);
    } // setupUi

    void retranslateUi(QFrame *PrivacyFrame)
    {
        PrivacyFrame->setWindowTitle(QApplication::translate("PrivacyFrame", "Frame", 0));
        m_newsCheck->setText(QApplication::translate("PrivacyFrame", "I want to see Intensecoin Blog feeds", 0));
        m_embeddedHelperLabel->setText(QApplication::translate("PrivacyFrame", "Checking this box will result in updated Overview screen. It will now show you the latest posts of Intensecoin Blog providing you with fresh Intensecoin news.", 0));
    } // retranslateUi

};

namespace Ui {
    class PrivacyFrame: public Ui_PrivacyFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVACYFRAME_H
