/********************************************************************************
** Form generated from reading UI file 'TransactionTransfersRenderer.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSACTIONTRANSFERSRENDERER_H
#define UI_TRANSACTIONTRANSFERSRENDERER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TransactionTransfersRenderer
{
public:
    QVBoxLayout *verticalLayout;
    QTreeView *m_transfersView;

    void setupUi(QFrame *TransactionTransfersRenderer)
    {
        if (TransactionTransfersRenderer->objectName().isEmpty())
            TransactionTransfersRenderer->setObjectName(QStringLiteral("TransactionTransfersRenderer"));
        TransactionTransfersRenderer->resize(437, 25);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TransactionTransfersRenderer->sizePolicy().hasHeightForWidth());
        TransactionTransfersRenderer->setSizePolicy(sizePolicy);
        TransactionTransfersRenderer->setMinimumSize(QSize(0, 20));
        TransactionTransfersRenderer->setFrameShape(QFrame::NoFrame);
        TransactionTransfersRenderer->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(TransactionTransfersRenderer);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout->setContentsMargins(0, 5, 12, 5);
        m_transfersView = new QTreeView(TransactionTransfersRenderer);
        m_transfersView->setObjectName(QStringLiteral("m_transfersView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_transfersView->sizePolicy().hasHeightForWidth());
        m_transfersView->setSizePolicy(sizePolicy1);
        m_transfersView->setMaximumSize(QSize(16777215, 16777215));
        m_transfersView->setFrameShape(QFrame::NoFrame);
        m_transfersView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_transfersView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_transfersView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        m_transfersView->setAutoScroll(false);
        m_transfersView->setSelectionMode(QAbstractItemView::NoSelection);
        m_transfersView->setTextElideMode(Qt::ElideMiddle);
        m_transfersView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        m_transfersView->setRootIsDecorated(false);
        m_transfersView->setItemsExpandable(false);
        m_transfersView->header()->setVisible(false);
        m_transfersView->header()->setStretchLastSection(false);

        verticalLayout->addWidget(m_transfersView);


        retranslateUi(TransactionTransfersRenderer);

        QMetaObject::connectSlotsByName(TransactionTransfersRenderer);
    } // setupUi

    void retranslateUi(QFrame *TransactionTransfersRenderer)
    {
        TransactionTransfersRenderer->setWindowTitle(QApplication::translate("TransactionTransfersRenderer", "Frame", 0));
    } // retranslateUi

};

namespace Ui {
    class TransactionTransfersRenderer: public Ui_TransactionTransfersRenderer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSACTIONTRANSFERSRENDERER_H
