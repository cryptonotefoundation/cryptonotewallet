// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <iostream>
#include <QRegExp>
#include <QHeaderView>
#include "ui_connectionsettingsdialog.h"
#include "ConnectionSettings.h"
#include "CurrencyAdapter.h"
#include "NewNodeDialog.h"
#include "MainWindow.h"
#include "NodeModel.h"

namespace Ui {
class ConnectionSettingsDialog;
}

namespace WalletGui {

void RemoteNodesView::showEvent(QShowEvent *e) {
  if (e->type() == QShowEvent::Show)
    this->selectRow(this->currentIndex().row());
}

void RemoteNodesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  QStyleOptionViewItem cellOption = option;
  cellOption.state &= QStyle::State_Active;
  QItemDelegate::paint(painter, cellOption, index);
}

QSize RemoteNodesDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
  return QItemDelegate::sizeHint(option, index);
}

ConnectionSettingsDialog::ConnectionSettingsDialog(QWidget *_parent) : QDialog(_parent),
                                                       m_nodeModel(new NodeModel(this)),
                                                       m_ui(new Ui::ConnectionSettingsDialog),
                                                       m_nodesCurrentIndex(0) {
  m_ui->setupUi(this);
  m_remoteNodesView = new RemoteNodesView(this);
  setupRemoteNodesView(m_remoteNodesView);
  m_ui->remoteNodesComboBox->setModel(m_nodeModel);
  m_ui->remoteNodesComboBox->setView(m_remoteNodesView);
  m_ui->remoteNodesComboBox->setModelColumn(1);
  m_ui->remoteNodesComboBox->view()->selectionModel()->hasSelection();
  m_ui->remoteNodesComboBox->setItemDelegate(new RemoteNodesDelegate);
  connect(m_ui->remoteNodesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(nodesCurrentIndex(int)));
}

ConnectionSettingsDialog::~ConnectionSettingsDialog() {
}

void ConnectionSettingsDialog::setupRemoteNodesView(QTableView *view) {
  view->setSelectionMode(QAbstractItemView::SingleSelection);
  view->setSelectionBehavior(QAbstractItemView::SelectRows); 
  view->setColumnWidth(0, 30);
  view->horizontalHeader()->setStretchLastSection(true);
  view->verticalHeader()->setStretchLastSection(true);
  view->horizontalHeader()->hide();
  view->verticalHeader()->hide();
  view->setShowGrid(false);
}

void ConnectionSettingsDialog::nodesCurrentIndex(int currentIndex) {
  m_nodesCurrentIndex = currentIndex;
  if (m_nodeModel) m_nodeModel->pushCurrentIndex(m_nodesCurrentIndex);
}

void ConnectionSettingsDialog::updateNodeSelect() {
  const NodeSetting currentRemoteNode = Settings::instance().getCurrentRemoteNode();
  int index = m_nodeModel->getIndexByData(currentRemoteNode);
  if ( index != -1 ) {
    m_ui->remoteNodesComboBox->setCurrentIndex(index);
  }
}

void ConnectionSettingsDialog::initConnectionSettings() {
  QString connection = Settings::instance().getConnection();
  if (connection.compare("auto") == 0) {
    m_ui->radioButton_1->setChecked(true);
  } else if (connection.compare("embedded") == 0) {
    m_ui->radioButton_2->setChecked(true);
  } else if (connection.compare("local") == 0) {
    m_ui->radioButton_3->setChecked(true);
  } else if (connection.compare("remote") == 0) {
    m_ui->radioButton_4->setChecked(true);
  }

  quint16 localDaemonPort = Settings::instance().getCurrentLocalDaemonPort();
  if (localDaemonPort == 0) localDaemonPort = CryptoNote::RPC_DEFAULT_PORT;
  m_ui->m_localDaemonPort->setValue(localDaemonPort);

  updateNodeSelect();
}

QString ConnectionSettingsDialog::setConnectionMode() const {
  QString connectionMode;
  if (m_ui->radioButton_1->isChecked()) {
    connectionMode = "auto";
  } else if (m_ui->radioButton_2->isChecked()) {
    connectionMode = "embedded";
  } else if (m_ui->radioButton_3->isChecked()) {
    connectionMode = "local";
  } else if(m_ui->radioButton_4->isChecked()) {
    connectionMode = "remote";
  }
  return connectionMode;
}

NodeSetting ConnectionSettingsDialog::setRemoteNode() const {
  return m_nodeModel->getDataByIndex(m_ui->remoteNodesComboBox->currentIndex());
}

quint16 ConnectionSettingsDialog::setLocalDaemonPort() const {
  quint16 localDaemonPort;
  localDaemonPort = m_ui->m_localDaemonPort->value();
  return localDaemonPort;
}

void ConnectionSettingsDialog::addNodeClicked() {
  NewNodeDialog dlg(&MainWindow::instance());
  NodeSetting nodeSetting;
  if (dlg.exec() == QDialog::Accepted) {
    nodeSetting.host = dlg.getHost();
    nodeSetting.port = dlg.getPort();
    nodeSetting.path = dlg.getPath();
    nodeSetting.ssl = dlg.getEnableSSL();
    if (QRegExp("^([a-z|A-Z|0-9]|[a-z|A-Z|0-9]-[a-z|A-Z|0-9]|[a-z|A-Z|0-9]\\.)+$").exactMatch(nodeSetting.host) &&
        (nodeSetting.port > 0 && nodeSetting.port < 65535) &&
        QRegExp("^(/([\\w|-]+/)+|/)$").exactMatch(nodeSetting.path)) {
      m_nodeModel->addNode(nodeSetting);
    }
  }
  updateNodeSelect();
}

void ConnectionSettingsDialog::removeNodeClicked() {
  m_nodeModel->delNode(m_ui->remoteNodesComboBox->currentIndex());
  updateNodeSelect();
}

}
