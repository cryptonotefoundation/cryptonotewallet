// Copyright (c) 2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "InfoDialog.h"

#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QLocale>
#include <QTabWidget>

#include "NodeAdapter.h"
#include "CryptoNoteWrapper.h"
#include "CurrencyAdapter.h"
#include "ConnectionsModel.h"

#include "ui_infodialog.h"

namespace WalletGui {

InfoDialog::InfoDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::InfoDialog), m_refreshTimerId(-1) {
  m_ui->setupUi(this);
  m_refreshTimerId = startTimer(1000);
  m_ui->m_connectionsView->setModel(&ConnectionsModel::instance());
  m_ui->m_connectionsView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  m_ui->m_connectionsView->setSortingEnabled(true);
  m_ui->m_connectionsView->sortByColumn(0, Qt::AscendingOrder);
  m_ui->m_connectionsView->header()->resizeSection(ConnectionsModel::COLUMN_STATE, 80);
  m_ui->m_connectionsView->header()->resizeSection(ConnectionsModel::COLUMN_ID, 90);
  m_ui->m_connectionsView->header()->resizeSection(ConnectionsModel::COLUMN_PORT, 45);
  m_ui->m_connectionsView->header()->resizeSection(ConnectionsModel::COLUMN_IS_INCOMING, 70);
  m_ui->m_connectionsView->header()->resizeSection(ConnectionsModel::COLUMN_HEIGHT, 50);
  m_ui->m_connectionsView->header()->resizeSection(ConnectionsModel::COLUMN_LAST_RESPONSE_HEIGHT, 50);
  m_ui->m_connectionsView->header()->resizeSection(ConnectionsModel::COLUMN_VERSION, 45);
  m_ui->m_connectionsView->setRootIsDecorated(false);

  m_ui->m_connectionsView->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_ui->m_connectionsView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

  m_contextMenu = new QMenu();
  m_contextMenu->addAction(QString(tr("Copy &address")), this, SLOT(copyAddressClicked()));
  m_contextMenu->addAction(QString(tr("Copy &Id")), this, SLOT(copyIdClicked()));

  ConnectionsModel::instance().refreshConnections();
}

InfoDialog::~InfoDialog() {
  killTimer(m_refreshTimerId);
  m_refreshTimerId = -1;
}

void InfoDialog::onCustomContextMenu(const QPoint &point) {
  m_index = m_ui->m_connectionsView->indexAt(point);
  if (!m_index.isValid())
     return;
  m_contextMenu->exec(m_ui->m_connectionsView->mapToGlobal(point));
}

void InfoDialog::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_refreshTimerId) {

    quint64 Connections = NodeAdapter::instance().getPeerCount(); // NodeAdapter::instance().getConnectionsCount();
    quint64 Outgoing =    NodeAdapter::instance().getOutgoingConnectionsCount();
    quint64 Incoming =    NodeAdapter::instance().getIncomingConnectionsCount();
    m_ui->m_connections->setText(QString(tr("%1 (Outgoing: %2, Incoming: %3)")).arg(Connections).arg(Outgoing).arg(Incoming));

    quint64 whitePeerList = NodeAdapter::instance().getWhitePeerlistSize();
    quint64 greyPeerList = NodeAdapter::instance().getGreyPeerlistSize();
    m_ui->m_peerList->setText(QString(tr("White: %1, Grey: %2")).arg(whitePeerList).arg(greyPeerList));

    quint64 lastKnownBlockHeight = NodeAdapter::instance().getLastKnownBlockHeight();
    quint64 lastLocalBlockHeight = NodeAdapter::instance().getLastLocalBlockHeight();
    m_ui->m_height->setText(QString(tr("Known: %1, Local: %2")).arg(lastKnownBlockHeight).arg(lastLocalBlockHeight));

    const QDateTime blockTime = NodeAdapter::instance().getLastLocalBlockTimestamp();
    m_ui->m_blockTime->setText(QString(tr("%1")).arg(QLocale(QLocale::English).toString(blockTime, "dd.MM.yyyy, HH:mm:ss UTC")));

    quint64 difficulty = NodeAdapter::instance().getDifficulty();
    m_ui->m_difficulty->setText(QString(tr("%1")).arg(difficulty));

    quint64 txCount = NodeAdapter::instance().getTxCount();
    m_ui->m_txCount->setText(QString(tr("%1")).arg(txCount));

    quint64 txPoolSize = NodeAdapter::instance().getTxPoolSize();
    m_ui->m_txPoolSize->setText(QString(tr("%1")).arg(txPoolSize));

    quint64 altBlocks = NodeAdapter::instance().getAltBlocksCount();
    m_ui->m_altBlocksCount->setText(QString(tr("%1")).arg(altBlocks));

    quint64 coinsInCirculation = NodeAdapter::instance().getAlreadyGeneratedCoins();
    m_ui->m_alreadyGeneratedCoins->setText(QString(tr("%1 %2")).arg(CurrencyAdapter::instance().formatAmount(coinsInCirculation)).arg(CurrencyAdapter::instance().getCurrencyTicker()));

    return;
  }

  QDialog::timerEvent(_event);
}

void InfoDialog::copyAddressClicked() {
  QApplication::clipboard()->setText(QString("%1:%2").arg(m_ui->m_connectionsView->currentIndex().data(ConnectionsModel::ROLE_HOST).toString()).arg(m_ui->m_connectionsView->currentIndex().data(ConnectionsModel::ROLE_PORT).toString()));
}

void InfoDialog::copyIdClicked() {
  QApplication::clipboard()->setText(m_ui->m_connectionsView->currentIndex().data(ConnectionsModel::ROLE_ID).toString());
}

}
