/********************************************************************************
** Form generated from reading UI file 'ExitWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXITWIDGET_H
#define UI_EXITWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExitWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;

    void setupUi(QWidget *ExitWidget)
    {
        if (ExitWidget->objectName().isEmpty())
            ExitWidget->setObjectName(QStringLiteral("ExitWidget"));
        ExitWidget->resize(385, 66);
        verticalLayout = new QVBoxLayout(ExitWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(ExitWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);


        retranslateUi(ExitWidget);

        QMetaObject::connectSlotsByName(ExitWidget);
    } // setupUi

    void retranslateUi(QWidget *ExitWidget)
    {
        ExitWidget->setWindowTitle(QApplication::translate("ExitWidget", "Saving data", 0));
        label->setText(QApplication::translate("ExitWidget", "Intensecoin wallet is saving data.\n"
"Please wait...", 0));
    } // retranslateUi

};

namespace Ui {
    class ExitWidget: public Ui_ExitWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXITWIDGET_H
