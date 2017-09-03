/********************************************************************************
** Form generated from reading UI file 'ConnectionOptionsFrame.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTIONOPTIONSFRAME_H
#define UI_CONNECTIONOPTIONSFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Gui/Common/WalletTextLabel.h"

QT_BEGIN_NAMESPACE

class Ui_ConnectionOptionsFrame
{
public:
    QVBoxLayout *verticalLayout_5;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QRadioButton *m_autoRadio;
    WalletGui::WalletSmallGrayTextLabel *m_autoHelperLabel;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *m_embeddedRadio;
    WalletGui::WalletSmallGrayTextLabel *m_embeddedHelperLabel;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QRadioButton *m_localRadio;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpinBox *m_localPortSpin;
    QSpacerItem *horizontalSpacer_2;
    WalletGui::WalletSmallGrayTextLabel *m_localHelperLabel;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *m_remoteRadio;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_2;
    QLineEdit *m_remoteHostEdit;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_3;
    QSpinBox *m_remotePortSpin;
    QSpacerItem *horizontalSpacer_4;
    WalletGui::WalletSmallGrayTextLabel *m_remoteHelperLabel;
    QSpacerItem *verticalSpacer;
    QButtonGroup *m_connectionButtonGroup;

    void setupUi(QFrame *ConnectionOptionsFrame)
    {
        if (ConnectionOptionsFrame->objectName().isEmpty())
            ConnectionOptionsFrame->setObjectName(QStringLiteral("ConnectionOptionsFrame"));
        ConnectionOptionsFrame->resize(746, 396);
        ConnectionOptionsFrame->setFrameShape(QFrame::NoFrame);
        ConnectionOptionsFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_5 = new QVBoxLayout(ConnectionOptionsFrame);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(25, 40, 49, -1);
        widget = new QWidget(ConnectionOptionsFrame);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(0, 70));
        widget->setMaximumSize(QSize(16777215, 70));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        m_autoRadio = new QRadioButton(widget);
        m_connectionButtonGroup = new QButtonGroup(ConnectionOptionsFrame);
        m_connectionButtonGroup->setObjectName(QStringLiteral("m_connectionButtonGroup"));
        m_connectionButtonGroup->addButton(m_autoRadio);
        m_autoRadio->setObjectName(QStringLiteral("m_autoRadio"));

        verticalLayout->addWidget(m_autoRadio);

        m_autoHelperLabel = new WalletGui::WalletSmallGrayTextLabel(widget);
        m_autoHelperLabel->setObjectName(QStringLiteral("m_autoHelperLabel"));
        m_autoHelperLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        m_autoHelperLabel->setWordWrap(true);
        m_autoHelperLabel->setIndent(0);

        verticalLayout->addWidget(m_autoHelperLabel, 0, Qt::AlignTop);

        verticalLayout->setStretch(1, 1);

        verticalLayout_5->addWidget(widget);

        widget_2 = new QWidget(ConnectionOptionsFrame);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        widget_2->setMinimumSize(QSize(0, 70));
        widget_2->setMaximumSize(QSize(16777215, 70));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        m_embeddedRadio = new QRadioButton(widget_2);
        m_connectionButtonGroup->addButton(m_embeddedRadio);
        m_embeddedRadio->setObjectName(QStringLiteral("m_embeddedRadio"));

        verticalLayout_2->addWidget(m_embeddedRadio);

        m_embeddedHelperLabel = new WalletGui::WalletSmallGrayTextLabel(widget_2);
        m_embeddedHelperLabel->setObjectName(QStringLiteral("m_embeddedHelperLabel"));
        m_embeddedHelperLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        m_embeddedHelperLabel->setWordWrap(true);
        m_embeddedHelperLabel->setIndent(0);

        verticalLayout_2->addWidget(m_embeddedHelperLabel, 0, Qt::AlignTop);

        verticalLayout_2->setStretch(1, 1);

        verticalLayout_5->addWidget(widget_2);

        widget_3 = new QWidget(ConnectionOptionsFrame);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        sizePolicy.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy);
        widget_3->setMinimumSize(QSize(0, 70));
        widget_3->setMaximumSize(QSize(16777215, 70));
        verticalLayout_3 = new QVBoxLayout(widget_3);
        verticalLayout_3->setSpacing(3);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        m_localRadio = new QRadioButton(widget_3);
        m_connectionButtonGroup->addButton(m_localRadio);
        m_localRadio->setObjectName(QStringLiteral("m_localRadio"));

        horizontalLayout->addWidget(m_localRadio);

        horizontalSpacer = new QSpacerItem(37, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(widget_3);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        m_localPortSpin = new QSpinBox(widget_3);
        m_localPortSpin->setObjectName(QStringLiteral("m_localPortSpin"));
        m_localPortSpin->setEnabled(false);
        m_localPortSpin->setMinimum(1);
        m_localPortSpin->setMaximum(65535);
        m_localPortSpin->setValue(1);

        horizontalLayout->addWidget(m_localPortSpin, 0, Qt::AlignVCenter);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout);

        m_localHelperLabel = new WalletGui::WalletSmallGrayTextLabel(widget_3);
        m_localHelperLabel->setObjectName(QStringLiteral("m_localHelperLabel"));
        m_localHelperLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        m_localHelperLabel->setWordWrap(true);
        m_localHelperLabel->setIndent(0);

        verticalLayout_3->addWidget(m_localHelperLabel, 0, Qt::AlignTop);

        verticalLayout_3->setStretch(1, 1);

        verticalLayout_5->addWidget(widget_3);

        widget_4 = new QWidget(ConnectionOptionsFrame);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        sizePolicy.setHeightForWidth(widget_4->sizePolicy().hasHeightForWidth());
        widget_4->setSizePolicy(sizePolicy);
        widget_4->setMinimumSize(QSize(0, 70));
        widget_4->setMaximumSize(QSize(16777215, 70));
        verticalLayout_4 = new QVBoxLayout(widget_4);
        verticalLayout_4->setSpacing(3);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        m_remoteRadio = new QRadioButton(widget_4);
        m_connectionButtonGroup->addButton(m_remoteRadio);
        m_remoteRadio->setObjectName(QStringLiteral("m_remoteRadio"));

        horizontalLayout_2->addWidget(m_remoteRadio);

        horizontalSpacer_3 = new QSpacerItem(22, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        label_2 = new QLabel(widget_4);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        m_remoteHostEdit = new QLineEdit(widget_4);
        m_remoteHostEdit->setObjectName(QStringLiteral("m_remoteHostEdit"));
        m_remoteHostEdit->setEnabled(false);

        horizontalLayout_2->addWidget(m_remoteHostEdit);

        horizontalSpacer_5 = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        label_3 = new QLabel(widget_4);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_2->addWidget(label_3);

        m_remotePortSpin = new QSpinBox(widget_4);
        m_remotePortSpin->setObjectName(QStringLiteral("m_remotePortSpin"));
        m_remotePortSpin->setEnabled(false);
        m_remotePortSpin->setMinimum(1);
        m_remotePortSpin->setMaximum(65535);
        m_remotePortSpin->setValue(1);

        horizontalLayout_2->addWidget(m_remotePortSpin, 0, Qt::AlignVCenter);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout_4->addLayout(horizontalLayout_2);

        m_remoteHelperLabel = new WalletGui::WalletSmallGrayTextLabel(widget_4);
        m_remoteHelperLabel->setObjectName(QStringLiteral("m_remoteHelperLabel"));
        m_remoteHelperLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        m_remoteHelperLabel->setWordWrap(true);
        m_remoteHelperLabel->setIndent(0);

        verticalLayout_4->addWidget(m_remoteHelperLabel, 0, Qt::AlignTop);

        verticalLayout_4->setStretch(1, 1);

        verticalLayout_5->addWidget(widget_4);

        verticalSpacer = new QSpacerItem(665, 61, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);


        retranslateUi(ConnectionOptionsFrame);
        QObject::connect(m_remoteHostEdit, SIGNAL(textChanged(QString)), ConnectionOptionsFrame, SLOT(remoteHostNameChanged(QString)));
        QObject::connect(m_connectionButtonGroup, SIGNAL(buttonClicked(int)), ConnectionOptionsFrame, SLOT(connectionButtonClicked(int)));
        QObject::connect(m_remoteRadio, SIGNAL(toggled(bool)), m_remoteHostEdit, SLOT(setEnabled(bool)));
        QObject::connect(m_remoteRadio, SIGNAL(toggled(bool)), m_remotePortSpin, SLOT(setEnabled(bool)));
        QObject::connect(m_localRadio, SIGNAL(toggled(bool)), m_localPortSpin, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(ConnectionOptionsFrame);
    } // setupUi

    void retranslateUi(QFrame *ConnectionOptionsFrame)
    {
        ConnectionOptionsFrame->setWindowTitle(QApplication::translate("ConnectionOptionsFrame", "Frame", 0));
        m_autoRadio->setText(QApplication::translate("ConnectionOptionsFrame", "Auto selection", 0));
        m_autoHelperLabel->setText(QApplication::translate("ConnectionOptionsFrame", "Wallet will connect to local Intensecoin daemon process on port 48782. In case of no local daemon running it will use an in-wallet embedded node implementation.", 0));
        m_embeddedRadio->setText(QApplication::translate("ConnectionOptionsFrame", "Embedded", 0));
        m_embeddedHelperLabel->setText(QApplication::translate("ConnectionOptionsFrame", "An in-wallet embedded Intensecoin node will be used.", 0));
        m_localRadio->setText(QApplication::translate("ConnectionOptionsFrame", "Local daemon", 0));
        label->setText(QApplication::translate("ConnectionOptionsFrame", "Port:", 0));
        m_localHelperLabel->setText(QApplication::translate("ConnectionOptionsFrame", "Wallet will connect to local Intensecoin daemon process. Please specify daemon's port.", 0));
        m_remoteRadio->setText(QApplication::translate("ConnectionOptionsFrame", "Remote daemon", 0));
        label_2->setText(QApplication::translate("ConnectionOptionsFrame", "Host:", 0));
        label_3->setText(QApplication::translate("ConnectionOptionsFrame", "Port:", 0));
        m_remoteHelperLabel->setText(QApplication::translate("ConnectionOptionsFrame", "Wallet will connect to Intensecoin node running on another PC in the local or global network. Please specify IP address or domain name and the port.", 0));
    } // retranslateUi

};

namespace Ui {
    class ConnectionOptionsFrame: public Ui_ConnectionOptionsFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTIONOPTIONSFRAME_H
