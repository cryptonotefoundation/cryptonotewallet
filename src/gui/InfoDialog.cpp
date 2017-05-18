// Copyright (c) 2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDateTime>
#include <QLocale>
#include "InfoDialog.h"
#include "NodeAdapter.h"
#include "CryptoNoteWrapper.h"

#include "ui_infodialog.h"

namespace WalletGui {

InfoDialog::InfoDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::InfoDialog), m_refreshTimerId(-1) {
  m_ui->setupUi(this);
  m_refreshTimerId = startTimer(1000);
}

InfoDialog::~InfoDialog() {
  killTimer(m_refreshTimerId);
  m_refreshTimerId = -1;
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

    return;
  }

  QDialog::timerEvent(_event);
}

}
