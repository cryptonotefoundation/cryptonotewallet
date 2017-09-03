/********************************************************************************
** Form generated from reading UI file 'MiningFrame.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MININGFRAME_H
#define UI_MININGFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include "Gui/Common/WalletBlueButton.h"
#include "Gui/Common/WalletTextLabel.h"
#include "Gui/Mining/PoolTreeView.h"

QT_BEGIN_NAMESPACE

class Ui_MiningFrame
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *m_miningManageFrame;
    QHBoxLayout *horizontalLayout;
    WalletGui::WalletNormalGrayTextLabel *label;
    QComboBox *m_cpuCoresCombo;
    QSpacerItem *horizontalSpacer_2;
    WalletGui::WalletNormalGrayTextLabel *label_2;
    QRadioButton *m_failoverStrategyRadio;
    QRadioButton *m_randomStrategyRadio;
    QSpacerItem *horizontalSpacer;
    QCheckBox *m_lockedScreenMiningCheck;
    QSpacerItem *horizontalSpacer_4;
    QFrame *m_hashRateFrame;
    QGridLayout *gridLayout_2;
    WalletGui::WalletNormalBlackTextLabel *m_donationHashRateTextLabel;
    QSpacerItem *verticalSpacer;
    WalletGui::WalletNormalBlackTextLabel *m_donationHashRateLabel;
    WalletGui::WalletHeaderLabel *m_hashrateLabel;
    WalletGui::WalletHeaderLabel *label_7;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    WalletGui::WalletNormalBlueButton *m_startMiningButton;
    QHBoxLayout *horizontalLayout_2;
    WalletGui::WalletHeaderLabel *label_6;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_3;
    WalletGui::PoolTreeView *m_poolView;
    QFrame *m_addPoolFrame;
    QGridLayout *gridLayout;
    WalletGui::WalletNormalGrayTextLabel *label_3;
    WalletGui::WalletNormalGrayTextLabel *label_4;
    WalletGui::WalletNormalGrayTextLabel *label_5;
    QLineEdit *m_newPoolHostEdit;
    QSpinBox *m_newPoolPortSpin;
    QSpinBox *m_difficultySpin;
    WalletGui::WalletNormalBlueButton *m_addPoolButton;
    QButtonGroup *m_poolSelectionStrategyButtonGroup;

    void setupUi(QFrame *MiningFrame)
    {
        if (MiningFrame->objectName().isEmpty())
            MiningFrame->setObjectName(QStringLiteral("MiningFrame"));
        MiningFrame->resize(1195, 547);
        MiningFrame->setFrameShape(QFrame::NoFrame);
        MiningFrame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(MiningFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        m_miningManageFrame = new QFrame(MiningFrame);
        m_miningManageFrame->setObjectName(QStringLiteral("m_miningManageFrame"));
        m_miningManageFrame->setMinimumSize(QSize(0, 100));
        m_miningManageFrame->setMaximumSize(QSize(16777215, 100));
        m_miningManageFrame->setFrameShape(QFrame::NoFrame);
        m_miningManageFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(m_miningManageFrame);
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(25, 20, 25, 20);
        label = new WalletGui::WalletNormalGrayTextLabel(m_miningManageFrame);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        m_cpuCoresCombo = new QComboBox(m_miningManageFrame);
        m_cpuCoresCombo->setObjectName(QStringLiteral("m_cpuCoresCombo"));

        horizontalLayout->addWidget(m_cpuCoresCombo);

        horizontalSpacer_2 = new QSpacerItem(13, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label_2 = new WalletGui::WalletNormalGrayTextLabel(m_miningManageFrame);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        m_failoverStrategyRadio = new QRadioButton(m_miningManageFrame);
        m_poolSelectionStrategyButtonGroup = new QButtonGroup(MiningFrame);
        m_poolSelectionStrategyButtonGroup->setObjectName(QStringLiteral("m_poolSelectionStrategyButtonGroup"));
        m_poolSelectionStrategyButtonGroup->addButton(m_failoverStrategyRadio);
        m_failoverStrategyRadio->setObjectName(QStringLiteral("m_failoverStrategyRadio"));
        m_failoverStrategyRadio->setChecked(true);

        horizontalLayout->addWidget(m_failoverStrategyRadio);

        m_randomStrategyRadio = new QRadioButton(m_miningManageFrame);
        m_poolSelectionStrategyButtonGroup->addButton(m_randomStrategyRadio);
        m_randomStrategyRadio->setObjectName(QStringLiteral("m_randomStrategyRadio"));

        horizontalLayout->addWidget(m_randomStrategyRadio);

        horizontalSpacer = new QSpacerItem(13, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_lockedScreenMiningCheck = new QCheckBox(m_miningManageFrame);
        m_lockedScreenMiningCheck->setObjectName(QStringLiteral("m_lockedScreenMiningCheck"));

        horizontalLayout->addWidget(m_lockedScreenMiningCheck);

        horizontalSpacer_4 = new QSpacerItem(31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        m_hashRateFrame = new QFrame(m_miningManageFrame);
        m_hashRateFrame->setObjectName(QStringLiteral("m_hashRateFrame"));
        gridLayout_2 = new QGridLayout(m_hashRateFrame);
        gridLayout_2->setSpacing(5);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        m_donationHashRateTextLabel = new WalletGui::WalletNormalBlackTextLabel(m_hashRateFrame);
        m_donationHashRateTextLabel->setObjectName(QStringLiteral("m_donationHashRateTextLabel"));
        m_donationHashRateTextLabel->setIndent(0);

        gridLayout_2->addWidget(m_donationHashRateTextLabel, 2, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 0, 0, 1, 1);

        m_donationHashRateLabel = new WalletGui::WalletNormalBlackTextLabel(m_hashRateFrame);
        m_donationHashRateLabel->setObjectName(QStringLiteral("m_donationHashRateLabel"));
        m_donationHashRateLabel->setIndent(0);

        gridLayout_2->addWidget(m_donationHashRateLabel, 2, 1, 1, 1, Qt::AlignLeft);

        m_hashrateLabel = new WalletGui::WalletHeaderLabel(m_hashRateFrame);
        m_hashrateLabel->setObjectName(QStringLiteral("m_hashrateLabel"));
        m_hashrateLabel->setMinimumSize(QSize(0, 0));

        gridLayout_2->addWidget(m_hashrateLabel, 1, 1, 1, 1, Qt::AlignLeft);

        label_7 = new WalletGui::WalletHeaderLabel(m_hashRateFrame);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setIndent(0);

        gridLayout_2->addWidget(label_7, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 3, 0, 1, 1);


        horizontalLayout->addWidget(m_hashRateFrame);

        horizontalSpacer_3 = new QSpacerItem(13, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        m_startMiningButton = new WalletGui::WalletNormalBlueButton(m_miningManageFrame);
        m_startMiningButton->setObjectName(QStringLiteral("m_startMiningButton"));
        m_startMiningButton->setMinimumSize(QSize(190, 0));
        m_startMiningButton->setMaximumSize(QSize(190, 16777215));
        m_startMiningButton->setCheckable(true);

        horizontalLayout->addWidget(m_startMiningButton);


        verticalLayout->addWidget(m_miningManageFrame);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(25, 20, 25, 20);
        label_6 = new WalletGui::WalletHeaderLabel(MiningFrame);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_2->addWidget(label_6);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(25, 0, 25, 20);
        m_poolView = new WalletGui::PoolTreeView(MiningFrame);
        m_poolView->setObjectName(QStringLiteral("m_poolView"));
        m_poolView->setFocusPolicy(Qt::NoFocus);
        m_poolView->setFrameShape(QFrame::NoFrame);
        m_poolView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        m_poolView->setDragEnabled(true);
        m_poolView->setDragDropOverwriteMode(false);
        m_poolView->setDragDropMode(QAbstractItemView::InternalMove);
        m_poolView->setDefaultDropAction(Qt::MoveAction);
        m_poolView->setSelectionMode(QAbstractItemView::SingleSelection);
        m_poolView->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_poolView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        m_poolView->setRootIsDecorated(false);
        m_poolView->setUniformRowHeights(true);
        m_poolView->setItemsExpandable(false);
        m_poolView->setAllColumnsShowFocus(true);
        m_poolView->header()->setStretchLastSection(false);

        horizontalLayout_3->addWidget(m_poolView);


        verticalLayout->addLayout(horizontalLayout_3);

        m_addPoolFrame = new QFrame(MiningFrame);
        m_addPoolFrame->setObjectName(QStringLiteral("m_addPoolFrame"));
        m_addPoolFrame->setFrameShape(QFrame::NoFrame);
        m_addPoolFrame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(m_addPoolFrame);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(25, 23, 25, 23);
        label_3 = new WalletGui::WalletNormalGrayTextLabel(m_addPoolFrame);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setIndent(0);

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        label_4 = new WalletGui::WalletNormalGrayTextLabel(m_addPoolFrame);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setIndent(0);

        gridLayout->addWidget(label_4, 0, 1, 1, 1);

        label_5 = new WalletGui::WalletNormalGrayTextLabel(m_addPoolFrame);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setIndent(0);

        gridLayout->addWidget(label_5, 0, 2, 1, 1);

        m_newPoolHostEdit = new QLineEdit(m_addPoolFrame);
        m_newPoolHostEdit->setObjectName(QStringLiteral("m_newPoolHostEdit"));

        gridLayout->addWidget(m_newPoolHostEdit, 1, 0, 1, 1);

        m_newPoolPortSpin = new QSpinBox(m_addPoolFrame);
        m_newPoolPortSpin->setObjectName(QStringLiteral("m_newPoolPortSpin"));
        m_newPoolPortSpin->setMinimum(1);
        m_newPoolPortSpin->setMaximum(65535);
        m_newPoolPortSpin->setValue(1);

        gridLayout->addWidget(m_newPoolPortSpin, 1, 1, 1, 1);

        m_difficultySpin = new QSpinBox(m_addPoolFrame);
        m_difficultySpin->setObjectName(QStringLiteral("m_difficultySpin"));
        m_difficultySpin->setMaximum(999999999);
        m_difficultySpin->setSingleStep(1000);

        gridLayout->addWidget(m_difficultySpin, 1, 2, 1, 1);

        m_addPoolButton = new WalletGui::WalletNormalBlueButton(m_addPoolFrame);
        m_addPoolButton->setObjectName(QStringLiteral("m_addPoolButton"));
        m_addPoolButton->setEnabled(false);
        m_addPoolButton->setMinimumSize(QSize(80, 0));
        m_addPoolButton->setMaximumSize(QSize(80, 16777215));

        gridLayout->addWidget(m_addPoolButton, 1, 3, 1, 1);


        verticalLayout->addWidget(m_addPoolFrame);

        verticalLayout->setStretch(2, 1);

        retranslateUi(MiningFrame);
        QObject::connect(m_poolSelectionStrategyButtonGroup, SIGNAL(buttonToggled(QAbstractButton*,bool)), MiningFrame, SLOT(schedulePolicyChanged(QAbstractButton*,bool)));
        QObject::connect(m_startMiningButton, SIGNAL(clicked(bool)), MiningFrame, SLOT(startMiningClicked(bool)));
        QObject::connect(m_newPoolHostEdit, SIGNAL(textChanged(QString)), MiningFrame, SLOT(newPoolUrlChanged()));
        QObject::connect(m_newPoolPortSpin, SIGNAL(valueChanged(int)), MiningFrame, SLOT(newPoolUrlChanged()));
        QObject::connect(m_addPoolButton, SIGNAL(clicked()), MiningFrame, SLOT(addPoolClicked()));
        QObject::connect(m_poolView, SIGNAL(doubleClicked(QModelIndex)), MiningFrame, SLOT(deleteClicked(QModelIndex)));
        QObject::connect(m_cpuCoresCombo, SIGNAL(activated(int)), MiningFrame, SLOT(cpuCoreCountChangedByUser(int)));
        QObject::connect(m_lockedScreenMiningCheck, SIGNAL(clicked(bool)), MiningFrame, SLOT(miningOnLockedScreenChecked(bool)));

        QMetaObject::connectSlotsByName(MiningFrame);
    } // setupUi

    void retranslateUi(QFrame *MiningFrame)
    {
        MiningFrame->setWindowTitle(QApplication::translate("MiningFrame", "Frame", 0));
        label->setText(QApplication::translate("MiningFrame", "CPU CORES:", 0));
        label_2->setText(QApplication::translate("MiningFrame", "POOL SELECTION STRATEGY:", 0));
#ifndef QT_NO_TOOLTIP
        m_failoverStrategyRadio->setToolTip(QApplication::translate("MiningFrame", "Starts mining on the first pool. If it fails chooses the next one.", 0));
#endif // QT_NO_TOOLTIP
        m_failoverStrategyRadio->setText(QApplication::translate("MiningFrame", "Failover", 0));
#ifndef QT_NO_TOOLTIP
        m_randomStrategyRadio->setToolTip(QApplication::translate("MiningFrame", "Chooses random pool from the pool list.", 0));
#endif // QT_NO_TOOLTIP
        m_randomStrategyRadio->setText(QApplication::translate("MiningFrame", "Random", 0));
        m_lockedScreenMiningCheck->setText(QApplication::translate("MiningFrame", "Start mining when locked", 0));
        m_donationHashRateTextLabel->setText(QApplication::translate("MiningFrame", "Donation:", 0));
        m_donationHashRateLabel->setText(QString());
        m_hashrateLabel->setText(QString());
        label_7->setText(QApplication::translate("MiningFrame", "Hashrate:", 0));
        m_startMiningButton->setText(QApplication::translate("MiningFrame", "Start mining", 0));
        label_6->setText(QApplication::translate("MiningFrame", "Mining pool list", 0));
        label_3->setText(QApplication::translate("MiningFrame", "HOST", 0));
        label_4->setText(QApplication::translate("MiningFrame", "PORT", 0));
        label_5->setText(QApplication::translate("MiningFrame", "DIFFICULTY", 0));
        m_newPoolHostEdit->setPlaceholderText(QString());
        m_newPoolPortSpin->setSpecialValueText(QString());
        m_difficultySpin->setSpecialValueText(QString());
        m_addPoolButton->setText(QApplication::translate("MiningFrame", "Add pool", 0));
    } // retranslateUi

};

namespace Ui {
    class MiningFrame: public Ui_MiningFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MININGFRAME_H
