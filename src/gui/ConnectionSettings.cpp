// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "ui_connectionsettingsdialog.h"
#include "ConnectionSettings.h"
#include "CurrencyAdapter.h"
#include "NewNodeDialog.h"
#include "MainWindow.h"
#include "NodeModel.h"
#include "Settings.h"

namespace Ui {
class ConnectionSettingsDialog;
}

namespace WalletGui {

ConnectionSettingsDialog::ConnectionSettingsDialog(QWidget* _parent) : QDialog(_parent),
    m_nodeModel(new NodeModel(this)),
    m_ui(new Ui::ConnectionSettingsDialog) {
    m_ui->setupUi(this);
    m_ui->remoteNodesComboBox->setModel(m_nodeModel);
//  m_ui->remoteNodesComboBox->setValidator(new QRegExpValidator( QRegExp("[^\\:]+:[0-9]"), this ));
}

ConnectionSettingsDialog::~ConnectionSettingsDialog() {
}

void ConnectionSettingsDialog::initConnectionSettings() {
 QString connection = Settings::instance().getConnection();
    if(connection.compare("auto") == 0) {
        m_ui->radioButton_1->setChecked(true);
    }
    else if(connection.compare("embedded") == 0) {
        m_ui->radioButton_2->setChecked(true);
    }
    else if(connection.compare("local") == 0) {
       m_ui->radioButton_3->setChecked(true);
    }
    else if(connection.compare("remote") == 0) {
       m_ui->radioButton_4->setChecked(true);
    }

 quint16 localDaemonPort = Settings::instance().getCurrentLocalDaemonPort();
    if(localDaemonPort == 0) {
       localDaemonPort = CryptoNote::RPC_DEFAULT_PORT;
    }
    m_ui->m_localDaemonPort->setValue(localDaemonPort);

 QString currentRemoteNode = Settings::instance().getCurrentRemoteNode().split(":")[0];
 int index = m_ui->remoteNodesComboBox->findData(currentRemoteNode);
 if ( index != -1 ) {
    m_ui->remoteNodesComboBox->setCurrentIndex(index);
 }
}

QString ConnectionSettingsDialog::setConnectionMode() const {
    QString connectionMode;
    if(m_ui->radioButton_1->isChecked())
    {
        connectionMode = "auto";
    }
    else if(m_ui->radioButton_2->isChecked())
    {
        connectionMode = "embedded";
    }
    else if(m_ui->radioButton_3->isChecked())
    {
        connectionMode = "local";
    }
    else if(m_ui->radioButton_4->isChecked())
    {
        connectionMode = "remote";
    }
    return connectionMode;
}

QString ConnectionSettingsDialog::setRemoteNode() const {
    QString selectedRemoteNode;
            selectedRemoteNode = m_ui->remoteNodesComboBox->currentText();
            return selectedRemoteNode;
}

quint16 ConnectionSettingsDialog::setLocalDaemonPort() const {
    quint16 localDaemonPort;
            localDaemonPort = m_ui->m_localDaemonPort->value();
            return localDaemonPort;
}

void ConnectionSettingsDialog::addNodeClicked() {
/*
    QString host = m_ui->remoteNodesComboBox->currentText().split(":")[0];
    quint16 port = m_ui->remoteNodesComboBox->currentText().split(":")[1].toInt();
    if (host.isEmpty()) {
      return;
    }
    m_nodeModel->addNode(host, port);
*/
    NewNodeDialog dlg(&MainWindow::instance());
    if (dlg.exec() == QDialog::Accepted) {
    QString host = dlg.getHost();
    quint16 port = dlg.getPort();
    if (host.isEmpty()) {
      return;
    }
    m_nodeModel->addNode(host, port);
  }

}

void ConnectionSettingsDialog::removeNodeClicked() {
    m_nodeModel->removeRow(m_ui->remoteNodesComboBox->currentIndex());
    Settings::instance().setRpcNodesList(m_nodeModel->stringList());
}

}
