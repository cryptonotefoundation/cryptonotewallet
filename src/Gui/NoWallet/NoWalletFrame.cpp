// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Intensecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Intensecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Intensecoin.  If not, see <http://www.gnu.org/licenses/>.

#include <QFileInfo>
#include <qtimer.h>
#include "NoWalletFrame.h"
#include "Settings/Settings.h"
#include "ICryptoNoteAdapter.h"
#include "INodeAdapter.h"

#include "ui_NoWalletFrame.h"

namespace WalletGui {

NoWalletFrame::NoWalletFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::NoWalletFrame),
  m_walletFile(), m_legacyKeysFile() {
  m_ui->setupUi(this);
  m_ui->m_passwordFrame->hide();
}

NoWalletFrame::~NoWalletFrame() {
}

void NoWalletFrame::openWallet(const QString& _walletFile, const QString& _legacyKeysFile) {
  m_ui->m_passwordFrame->hide();
  setPasswordError(false);
  m_walletFile = _walletFile;
  m_legacyKeysFile = _legacyKeysFile;
  m_ui->m_walletNameLabel->setText(QFileInfo(_walletFile).completeBaseName().split(".wallet")[0]);
  if (m_legacyKeysFile.isEmpty()) {
    QMetaObject::invokeMethod(this, "loadWallet", Qt::QueuedConnection, Q_ARG(QString, ""));
  } else {
    QMetaObject::invokeMethod(this, "loadLegacyKeys", Qt::QueuedConnection, Q_ARG(QString, ""));
  }
}

void NoWalletFrame::setPasswordError(bool _isErrorOccured) {
  m_ui->m_passwordEdit->setProperty("errorState", _isErrorOccured);
  m_ui->m_passwordTextLabel->setProperty("errorState", _isErrorOccured);
  m_ui->m_passwordTextLabel->setText(_isErrorOccured ? tr("INCORRECT PASSWORD") : tr("PASSWORD FOR "));
  m_ui->m_walletTextLabel->setVisible(_isErrorOccured ? false : true);
  m_ui->m_walletNameLabel->setVisible(_isErrorOccured ? false : true);

  m_ui->m_passwordEdit->style()->unpolish(m_ui->m_passwordEdit);
  m_ui->m_passwordEdit->style()->polish(m_ui->m_passwordEdit);
  m_ui->m_passwordEdit->update();

  m_ui->m_passwordTextLabel->style()->unpolish(m_ui->m_passwordTextLabel);
  m_ui->m_passwordTextLabel->style()->polish(m_ui->m_passwordTextLabel);
  m_ui->m_passwordTextLabel->update();
}

void NoWalletFrame::setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) {
  m_cryptoNoteAdapter = _cryptoNoteAdapter;
  m_cryptoNoteAdapter->addObserver(this);
  m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
}

void NoWalletFrame::setMainWindow(QWidget* _mainWindow) {
  QList<QAction*> actions = _mainWindow->actions();
  for (const auto& action : actions) {
    if (!action->objectName().compare("m_createWalletAction")) {
      connect(m_ui->m_createWalletButton, &WalletLargeBlueButton::clicked, action, &QAction::trigger, Qt::UniqueConnection);
    } else if (!action->objectName().compare("m_openWalletAction")) {
      connect(m_ui->m_openWalletButton, &WalletLargeBlueButton::clicked, action, &QAction::trigger, Qt::UniqueConnection);
    }
  }
}

void NoWalletFrame::walletOpened() {
  setPasswordError(false);
  m_ui->m_passwordFrame->hide();
  setEnabled(true);
}

void NoWalletFrame::walletOpenError(int _initStatus) {
  setEnabled(true);
  m_ui->m_passwordEdit->setFocus();
}

void NoWalletFrame::walletClosed() {
  setEnabled(true);
  m_ui->m_passwordEdit->setFocus();
}

void NoWalletFrame::passwordChanged() {
  // Do nothing
}

void NoWalletFrame::synchronizationProgressUpdated(quint32 _current, quint32 _total) {
  // Do nothing
}

void NoWalletFrame::synchronizationCompleted() {
  // Do nothing
}

void NoWalletFrame::balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) {
  // Do nothing
}

void NoWalletFrame::externalTransactionCreated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void NoWalletFrame::transactionUpdated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void NoWalletFrame::cryptoNoteAdapterInitCompleted(int _status) {
  if (_status == 0) {
    m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
  }
}

void NoWalletFrame::cryptoNoteAdapterDeinitCompleted() {
  // Do nothing
}

void NoWalletFrame::loadWallet(const QString& _password) {
  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  IWalletAdapter::WalletInitStatus status = walletAdapter->load(m_walletFile, _password);
  if (status == IWalletAdapter::INIT_WRONG_PASSWORD) {
    if (m_ui->m_passwordFrame->isVisible()) {
      setPasswordError(true);
    } else {
      m_ui->m_passwordFrame->show();
    }
  } else if (status == IWalletAdapter::INIT_SUCCESS) {
    Settings::instance().setWalletFile(m_walletFile);
  }
}

void NoWalletFrame::loadLegacyKeys(const QString& _password) {
  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  IWalletAdapter::WalletInitStatus status = walletAdapter->loadLegacyKeys(m_legacyKeysFile, m_walletFile, _password);
  if (status == IWalletAdapter::INIT_WRONG_PASSWORD) {
    if (m_ui->m_passwordFrame->isVisible()) {
      setPasswordError(true);
    } else {
      m_ui->m_passwordFrame->show();
    }
  } else if (status == IWalletAdapter::INIT_SUCCESS) {
    Settings::instance().setWalletFile(m_walletFile);
  }
}

void NoWalletFrame::attemptToProceedWithOk() {
	auto peer_count = m_cryptoNoteAdapter->getNodeAdapter()->getPeerCount();
	if (peer_count >= 1) {
		m_timer->stop();
		m_ui->m_passwordEdit->setDisabled(false);
		m_ui->m_okButton->setDisabled(false);
		setPasswordError(false);
		okClicked();

		if (m_timer) {
			delete m_timer;
			m_timer = NULL;
		}
	}
}

void NoWalletFrame::okClicked() {
	auto peer_count = m_cryptoNoteAdapter->getNodeAdapter()->getPeerCount();
	if (peer_count >= 1) {
		setEnabled(false);
		QString password = m_ui->m_passwordEdit->text();
		m_ui->m_passwordEdit->clear();
		if (m_legacyKeysFile.isEmpty()) {
			loadWallet(password);
		}
		else {
			loadLegacyKeys(password);
		}
	}
	else {
		//update ui
		m_ui->m_passwordTextLabel->setText(tr("WAITING FOR PEER CONNECTIONS BEFORE OPENING WALLET..."));
		m_ui->m_walletTextLabel->setVisible(false);
		m_ui->m_walletNameLabel->setVisible(false);

		m_ui->m_passwordEdit->setDisabled(true);
		m_ui->m_okButton->setDisabled(true);

		m_ui->m_passwordTextLabel->style()->unpolish(m_ui->m_passwordTextLabel);
		m_ui->m_passwordTextLabel->style()->polish(m_ui->m_passwordTextLabel);
		m_ui->m_passwordTextLabel->update();
		//start checking timer
		if (!m_timer)
			m_timer = new QTimer(this);

		connect(m_timer, SIGNAL(timeout()), this, SLOT(attemptToProceedWithOk()));
		m_timer->start(1000);
	}
}

void NoWalletFrame::passwordChanged(const QString& _password) {
    setPasswordError(false);
}

}
