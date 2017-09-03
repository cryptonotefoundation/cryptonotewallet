/********************************************************************************
** Form generated from reading UI file 'NewsFrame.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWSFRAME_H
#define UI_NEWSFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Gui/Common/WalletTextLabel.h"

QT_BEGIN_NAMESPACE

class Ui_NewsFrame
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *m_newsWidget;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    WalletGui::WalletHeaderLabel *m_nameLabel;
    QLabel *m_linkLabel;
    QLabel *m_textLabel;
    QLabel *m_iconLabel;
    QFrame *m_scrollFrame;
    QVBoxLayout *verticalLayout;
    QPushButton *m_scrollUpButton;
    QSpacerItem *verticalSpacer;
    QPushButton *m_scrollDownButton;

    void setupUi(QFrame *NewsFrame)
    {
        if (NewsFrame->objectName().isEmpty())
            NewsFrame->setObjectName(QStringLiteral("NewsFrame"));
        NewsFrame->resize(618, 202);
        NewsFrame->setFrameShape(QFrame::StyledPanel);
        NewsFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(NewsFrame);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        m_newsWidget = new QWidget(NewsFrame);
        m_newsWidget->setObjectName(QStringLiteral("m_newsWidget"));
        gridLayout = new QGridLayout(m_newsWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(10);
        gridLayout->setContentsMargins(40, 25, 40, 25);
        horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        m_nameLabel = new WalletGui::WalletHeaderLabel(m_newsWidget);
        m_nameLabel->setObjectName(QStringLiteral("m_nameLabel"));
        m_nameLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        m_nameLabel->setIndent(0);

        gridLayout->addWidget(m_nameLabel, 0, 2, 1, 1);

        m_linkLabel = new QLabel(m_newsWidget);
        m_linkLabel->setObjectName(QStringLiteral("m_linkLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_linkLabel->sizePolicy().hasHeightForWidth());
        m_linkLabel->setSizePolicy(sizePolicy);
        m_linkLabel->setCursor(QCursor(Qt::PointingHandCursor));
        m_linkLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        m_linkLabel->setIndent(0);
        m_linkLabel->setOpenExternalLinks(true);

        gridLayout->addWidget(m_linkLabel, 0, 3, 1, 1);

        m_textLabel = new QLabel(m_newsWidget);
        m_textLabel->setObjectName(QStringLiteral("m_textLabel"));
        m_textLabel->setTextFormat(Qt::RichText);
        m_textLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        m_textLabel->setWordWrap(true);
        m_textLabel->setIndent(0);

        gridLayout->addWidget(m_textLabel, 1, 2, 1, 2);

        m_iconLabel = new QLabel(m_newsWidget);
        m_iconLabel->setObjectName(QStringLiteral("m_iconLabel"));
        m_iconLabel->setIndent(0);

        gridLayout->addWidget(m_iconLabel, 0, 0, 2, 1, Qt::AlignTop);

        gridLayout->setRowStretch(1, 1);
        gridLayout->setColumnStretch(3, 1);

        horizontalLayout->addWidget(m_newsWidget);

        m_scrollFrame = new QFrame(NewsFrame);
        m_scrollFrame->setObjectName(QStringLiteral("m_scrollFrame"));
        m_scrollFrame->setMinimumSize(QSize(45, 0));
        m_scrollFrame->setFrameShape(QFrame::StyledPanel);
        m_scrollFrame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(m_scrollFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        m_scrollUpButton = new QPushButton(m_scrollFrame);
        m_scrollUpButton->setObjectName(QStringLiteral("m_scrollUpButton"));
        m_scrollUpButton->setCursor(QCursor(Qt::PointingHandCursor));

        verticalLayout->addWidget(m_scrollUpButton, 0, Qt::AlignTop);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        m_scrollDownButton = new QPushButton(m_scrollFrame);
        m_scrollDownButton->setObjectName(QStringLiteral("m_scrollDownButton"));
        m_scrollDownButton->setCursor(QCursor(Qt::PointingHandCursor));

        verticalLayout->addWidget(m_scrollDownButton, 0, Qt::AlignBottom);


        horizontalLayout->addWidget(m_scrollFrame);

        horizontalLayout->setStretch(0, 1);

        retranslateUi(NewsFrame);
        QObject::connect(m_scrollUpButton, SIGNAL(clicked()), NewsFrame, SLOT(upClicked()));
        QObject::connect(m_scrollDownButton, SIGNAL(clicked()), NewsFrame, SLOT(downClicked()));

        QMetaObject::connectSlotsByName(NewsFrame);
    } // setupUi

    void retranslateUi(QFrame *NewsFrame)
    {
        NewsFrame->setWindowTitle(QApplication::translate("NewsFrame", "Frame", 0));
        m_nameLabel->setText(QString());
        m_linkLabel->setText(QString());
        m_iconLabel->setText(QString());
        m_scrollUpButton->setText(QString());
        m_scrollDownButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class NewsFrame: public Ui_NewsFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWSFRAME_H
