/********************************************************************************
** Form generated from reading UI file 'OptimizationOptionsFrame.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIMIZATIONOPTIONSFRAME_H
#define UI_OPTIMIZATIONOPTIONSFRAME_H

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
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Gui/Common/WalletTextLabel.h"

QT_BEGIN_NAMESPACE

class Ui_OptimizationOptionsFrame
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *m_enableOptimizationCheck;
    WalletGui::WalletNormalGrayTextLabel *label_8;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QWidget *m_optimizationOptionsWidget;
    QHBoxLayout *horizontalLayout;
    QWidget *widget1;
    QGridLayout *gridLayout;
    WalletGui::WalletNormalBlackTextLabel *label_5;
    QSpacerItem *verticalSpacer_3;
    QSlider *m_thresholdSlider;
    QComboBox *m_thresholdCombo;
    QFrame *line;
    QSpacerItem *verticalSpacer_4;
    QSpinBox *m_mixinSpin;
    WalletGui::WalletNormalBlackTextLabel *label_7;
    QSlider *m_mixinSlider;
    QSpacerItem *verticalSpacer_12;
    QHBoxLayout *horizontalLayout_3;
    QLabel *m_nonOptimizedOutputsLabel;
    WalletGui::WalletNormalBlackTextLabel *m_nonOptimizedOutputsTextLabel;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget2;
    QGridLayout *gridLayout_2;
    QComboBox *m_periodCombo;
    WalletGui::WalletNormalBlackTextLabel *label_2;
    QTimeEdit *m_startTimeEdit;
    WalletGui::WalletNormalBlackTextLabel *label_3;
    QTimeEdit *m_stopTimeEdit;
    QCheckBox *m_optimizationTimeCheck;
    QFrame *line_2;
    WalletGui::WalletNormalBlackTextLabel *label_4;
    QSpacerItem *verticalSpacer_8;
    QSpacerItem *verticalSpacer_6;
    QFrame *line_3;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *verticalSpacer_7;
    QSpacerItem *verticalSpacer_10;
    QCheckBox *m_showFusionTransactionsCheck;
    QSpacerItem *verticalSpacer_11;
    WalletGui::WalletExtraNormalGrayTextLabel *m_helpLabel;
    QSpacerItem *verticalSpacer;

    void setupUi(QFrame *OptimizationOptionsFrame)
    {
        if (OptimizationOptionsFrame->objectName().isEmpty())
            OptimizationOptionsFrame->setObjectName(QStringLiteral("OptimizationOptionsFrame"));
        OptimizationOptionsFrame->resize(735, 474);
        OptimizationOptionsFrame->setFrameShape(QFrame::NoFrame);
        OptimizationOptionsFrame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(OptimizationOptionsFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(25, 35, 25, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        m_enableOptimizationCheck = new QCheckBox(OptimizationOptionsFrame);
        m_enableOptimizationCheck->setObjectName(QStringLiteral("m_enableOptimizationCheck"));

        horizontalLayout_2->addWidget(m_enableOptimizationCheck);

        label_8 = new WalletGui::WalletNormalGrayTextLabel(OptimizationOptionsFrame);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_2->addWidget(label_8);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        m_optimizationOptionsWidget = new QWidget(OptimizationOptionsFrame);
        m_optimizationOptionsWidget->setObjectName(QStringLiteral("m_optimizationOptionsWidget"));
        m_optimizationOptionsWidget->setEnabled(true);
        horizontalLayout = new QHBoxLayout(m_optimizationOptionsWidget);
        horizontalLayout->setSpacing(75);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        widget1 = new QWidget(m_optimizationOptionsWidget);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setEnabled(false);
        gridLayout = new QGridLayout(widget1);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setVerticalSpacing(5);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_5 = new WalletGui::WalletNormalBlackTextLabel(widget1);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setIndent(0);

        gridLayout->addWidget(label_5, 0, 0, 1, 4);

        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_3, 4, 0, 1, 4);

        m_thresholdSlider = new QSlider(widget1);
        m_thresholdSlider->setObjectName(QStringLiteral("m_thresholdSlider"));
        m_thresholdSlider->setMaximum(6);
        m_thresholdSlider->setPageStep(1);
        m_thresholdSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(m_thresholdSlider, 1, 0, 1, 3);

        m_thresholdCombo = new QComboBox(widget1);
        m_thresholdCombo->setObjectName(QStringLiteral("m_thresholdCombo"));

        gridLayout->addWidget(m_thresholdCombo, 1, 3, 1, 1);

        line = new QFrame(widget1);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 5, 0, 1, 4);

        verticalSpacer_4 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_4, 6, 0, 1, 4);

        m_mixinSpin = new QSpinBox(widget1);
        m_mixinSpin->setObjectName(QStringLiteral("m_mixinSpin"));
        m_mixinSpin->setButtonSymbols(QAbstractSpinBox::UpDownArrows);

        gridLayout->addWidget(m_mixinSpin, 8, 3, 1, 1);

        label_7 = new WalletGui::WalletNormalBlackTextLabel(widget1);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setIndent(0);

        gridLayout->addWidget(label_7, 7, 0, 1, 4);

        m_mixinSlider = new QSlider(widget1);
        m_mixinSlider->setObjectName(QStringLiteral("m_mixinSlider"));
        m_mixinSlider->setPageStep(1);
        m_mixinSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(m_mixinSlider, 8, 0, 1, 3);

        verticalSpacer_12 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_12, 2, 0, 1, 4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        m_nonOptimizedOutputsLabel = new QLabel(widget1);
        m_nonOptimizedOutputsLabel->setObjectName(QStringLiteral("m_nonOptimizedOutputsLabel"));

        horizontalLayout_3->addWidget(m_nonOptimizedOutputsLabel);

        m_nonOptimizedOutputsTextLabel = new WalletGui::WalletNormalBlackTextLabel(widget1);
        m_nonOptimizedOutputsTextLabel->setObjectName(QStringLiteral("m_nonOptimizedOutputsTextLabel"));

        horizontalLayout_3->addWidget(m_nonOptimizedOutputsTextLabel);

        horizontalLayout_3->setStretch(1, 1);

        gridLayout->addLayout(horizontalLayout_3, 3, 0, 1, 4);

        gridLayout->setColumnStretch(0, 50);

        horizontalLayout->addWidget(widget1, 0, Qt::AlignTop);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(10);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, -1, 0, 0);
        widget2 = new QWidget(m_optimizationOptionsWidget);
        widget2->setObjectName(QStringLiteral("widget2"));
        widget2->setEnabled(false);
        gridLayout_2 = new QGridLayout(widget2);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setVerticalSpacing(5);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        m_periodCombo = new QComboBox(widget2);
        m_periodCombo->setObjectName(QStringLiteral("m_periodCombo"));

        gridLayout_2->addWidget(m_periodCombo, 2, 0, 1, 2, Qt::AlignLeft);

        label_2 = new WalletGui::WalletNormalBlackTextLabel(widget2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 8, 0, 1, 1);

        m_startTimeEdit = new QTimeEdit(widget2);
        m_startTimeEdit->setObjectName(QStringLiteral("m_startTimeEdit"));
        m_startTimeEdit->setEnabled(false);

        gridLayout_2->addWidget(m_startTimeEdit, 8, 1, 1, 1);

        label_3 = new WalletGui::WalletNormalBlackTextLabel(widget2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 8, 2, 1, 1);

        m_stopTimeEdit = new QTimeEdit(widget2);
        m_stopTimeEdit->setObjectName(QStringLiteral("m_stopTimeEdit"));
        m_stopTimeEdit->setEnabled(false);

        gridLayout_2->addWidget(m_stopTimeEdit, 8, 3, 1, 1);

        m_optimizationTimeCheck = new QCheckBox(widget2);
        m_optimizationTimeCheck->setObjectName(QStringLiteral("m_optimizationTimeCheck"));

        gridLayout_2->addWidget(m_optimizationTimeCheck, 6, 0, 1, 4);

        line_2 = new QFrame(widget2);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_2, 4, 0, 1, 4);

        label_4 = new WalletGui::WalletNormalBlackTextLabel(widget2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setIndent(0);

        gridLayout_2->addWidget(label_4, 0, 0, 1, 4);

        verticalSpacer_8 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_8, 7, 0, 1, 4);

        verticalSpacer_6 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_6, 3, 0, 1, 4);

        line_3 = new QFrame(widget2);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_3, 10, 0, 1, 4);

        verticalSpacer_5 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_5, 1, 0, 1, 4);

        verticalSpacer_7 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_7, 5, 0, 1, 4);

        verticalSpacer_10 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_10, 9, 0, 1, 4);


        verticalLayout_3->addWidget(widget2);

        m_showFusionTransactionsCheck = new QCheckBox(m_optimizationOptionsWidget);
        m_showFusionTransactionsCheck->setObjectName(QStringLiteral("m_showFusionTransactionsCheck"));
        m_showFusionTransactionsCheck->setEnabled(true);

        verticalLayout_3->addWidget(m_showFusionTransactionsCheck);


        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout->addWidget(m_optimizationOptionsWidget);

        verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_11);

        m_helpLabel = new WalletGui::WalletExtraNormalGrayTextLabel(OptimizationOptionsFrame);
        m_helpLabel->setObjectName(QStringLiteral("m_helpLabel"));
        m_helpLabel->setWordWrap(true);

        verticalLayout->addWidget(m_helpLabel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(OptimizationOptionsFrame);
        QObject::connect(m_enableOptimizationCheck, SIGNAL(toggled(bool)), widget1, SLOT(setEnabled(bool)));
        QObject::connect(m_thresholdSlider, SIGNAL(valueChanged(int)), OptimizationOptionsFrame, SLOT(thresholdChanged(int)));
        QObject::connect(m_mixinSlider, SIGNAL(valueChanged(int)), OptimizationOptionsFrame, SLOT(mixinChanged(int)));
        QObject::connect(m_optimizationTimeCheck, SIGNAL(toggled(bool)), m_startTimeEdit, SLOT(setEnabled(bool)));
        QObject::connect(m_optimizationTimeCheck, SIGNAL(toggled(bool)), m_stopTimeEdit, SLOT(setEnabled(bool)));
        QObject::connect(m_mixinSpin, SIGNAL(valueChanged(int)), OptimizationOptionsFrame, SLOT(mixinChanged(int)));
        QObject::connect(m_thresholdCombo, SIGNAL(currentIndexChanged(int)), OptimizationOptionsFrame, SLOT(thresholdChanged(int)));
        QObject::connect(m_enableOptimizationCheck, SIGNAL(toggled(bool)), widget2, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(OptimizationOptionsFrame);
    } // setupUi

    void retranslateUi(QFrame *OptimizationOptionsFrame)
    {
        OptimizationOptionsFrame->setWindowTitle(QApplication::translate("OptimizationOptionsFrame", "Frame", 0));
        m_enableOptimizationCheck->setText(QApplication::translate("OptimizationOptionsFrame", "Run background wallet optimization", 0));
        label_8->setText(QApplication::translate("OptimizationOptionsFrame", "(Recommended)", 0));
        label_5->setText(QApplication::translate("OptimizationOptionsFrame", "Optimization target", 0));
        label_7->setText(QApplication::translate("OptimizationOptionsFrame", "Anonymity level", 0));
        m_nonOptimizedOutputsLabel->setText(QApplication::translate("OptimizationOptionsFrame", "0", 0));
        m_nonOptimizedOutputsTextLabel->setText(QApplication::translate("OptimizationOptionsFrame", "outputs below selected target", 0));
        label_2->setText(QApplication::translate("OptimizationOptionsFrame", "From", 0));
        m_startTimeEdit->setDisplayFormat(QApplication::translate("OptimizationOptionsFrame", "HH:mm", 0));
        label_3->setText(QApplication::translate("OptimizationOptionsFrame", "To", 0));
        m_stopTimeEdit->setDisplayFormat(QApplication::translate("OptimizationOptionsFrame", "HH:mm", 0));
        m_optimizationTimeCheck->setText(QApplication::translate("OptimizationOptionsFrame", "Specific time for optimization transactions", 0));
        label_4->setText(QApplication::translate("OptimizationOptionsFrame", "Send optimization transactions each", 0));
        m_showFusionTransactionsCheck->setText(QApplication::translate("OptimizationOptionsFrame", "Show optimization transactions", 0));
        m_helpLabel->setText(QApplication::translate("OptimizationOptionsFrame", "Wallet optimization is a background task performed in order to aggregate transactions outputs that are below optimization target into bigger ones. It makes it cheaper to send larger amounts of Intensecoins within one transaction.\n"
"Wallet optimization is free for all users but takes some time.", 0));
    } // retranslateUi

};

namespace Ui {
    class OptimizationOptionsFrame: public Ui_OptimizationOptionsFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIMIZATIONOPTIONSFRAME_H
