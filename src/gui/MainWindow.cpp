// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016 Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QCloseEvent>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QFontDatabase>
#include <Common/Base58.h>
#include <Common/Util.h>
#include <QToolButton>
#include <QDebug>
#include "AboutDialog.h"
#include "AnimatedLabel.h"
#include "ChangePasswordDialog.h"
#include "ChangeLanguageDialog.h"
#include "ConnectionSettings.h"
#include "PrivateKeysDialog.h"
#include "CurrencyAdapter.h"
#include "ExitWidget.h"
#include "ImportKeyDialog.h"
#include "MainWindow.h"
#include "NewPasswordDialog.h"
#include "NodeAdapter.h"
#include "PasswordDialog.h"
#include "Settings.h"
#include "WalletAdapter.h"
#include "WalletEvents.h"
#include "SendFrame.h"
#include "ui_mainwindow.h"

namespace WalletGui {

MainWindow* MainWindow::m_instance = nullptr;

MainWindow& MainWindow::instance() {
  if (m_instance == nullptr) {
    m_instance = new MainWindow;
  }

  return *m_instance;
}

MainWindow::MainWindow() : QMainWindow(), m_ui(new Ui::MainWindow), m_trayIcon(nullptr), m_tabActionGroup(new QActionGroup(this)),
  m_isAboutToQuit(false), paymentServer(0) {
  m_ui->setupUi(this);
  m_connectionStateIconLabel = new QLabel(this);
  m_encryptionStateIconLabel = new QLabel(this);
  m_synchronizationStateIconLabel = new AnimatedLabel(this);
  connectToSignals();
  initUi();
  walletClosed();
}

MainWindow::~MainWindow() {
    delete paymentServer;
    paymentServer = 0;
}

void MainWindow::connectToSignals() {
  connect(&WalletAdapter::instance(), &WalletAdapter::openWalletWithPasswordSignal, this, &MainWindow::askForWalletPassword, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::changeWalletPasswordSignal, this, &MainWindow::encryptWallet, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletSynchronizationProgressUpdatedSignal,
    this, &MainWindow::walletSynchronizationInProgress, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletSynchronizationCompletedSignal, this, &MainWindow::walletSynchronized
    , Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletStateChangedSignal, this, &MainWindow::setStatusBarText);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletInitCompletedSignal, this, &MainWindow::walletOpened);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &MainWindow::walletClosed);
  connect(&NodeAdapter::instance(), &NodeAdapter::peerCountUpdatedSignal, this, &MainWindow::peerCountUpdated, Qt::QueuedConnection);
  connect(m_ui->m_exitAction, &QAction::triggered, qApp, &QApplication::quit);
  connect(m_ui->m_accountFrame, &AccountFrame::showQRcodeSignal, this, &MainWindow::onShowQR, Qt::QueuedConnection);
  connect(m_ui->m_sendFrame, &SendFrame::uriOpenSignal, this, &MainWindow::onUriOpenSignal, Qt::QueuedConnection);
}

void MainWindow::initUi() {
  setWindowTitle(QString(tr("%1 Wallet %2")).arg(CurrencyAdapter::instance().getCurrencyDisplayName()).arg(Settings::instance().getVersion()));
#ifdef Q_OS_WIN32
  if (QSystemTrayIcon::isSystemTrayAvailable()) {
    m_trayIcon = new QSystemTrayIcon(QPixmap(":images/cryptonote"), this);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayActivated);
  }
#endif

  QToolBar *accountToolBar = addToolBar(tr("Account toolbar"));
  accountToolBar->setAllowedAreas(Qt::TopToolBarArea);
  accountToolBar->addWidget(m_ui->m_accountFrame);
  accountToolBar->setMovable(false);
  addToolBar(Qt::TopToolBarArea, accountToolBar);
  addToolBarBreak();
  addToolBar(Qt::LeftToolBarArea, m_ui->toolBar);
  accountToolBar->setStyleSheet("QToolBar {background-color: rgb(133, 167, 211); border: 0;}");
  m_ui->toolBar->setStyleSheet("QToolBar {background-color: rgba(32, 55, 92, 0.9); border: 0; width: 150px; min-width: 150px; spacing: 0; padding: 0; margin: 0;}");
  QToolButton button;
  button.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  button.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  button.setStyleSheet("QToolButton {spacing:0px; width: 150px; min-width: 150px;}");

  m_ui->m_aboutCryptonoteAction->setText(QString(tr("About %1 Wallet")).arg(CurrencyAdapter::instance().getCurrencyDisplayName()));
  m_ui->m_overviewFrame->hide();
  m_ui->m_sendFrame->hide();
  m_ui->m_accountFrame->hide();
  m_ui->m_receiveFrame->hide();
  m_ui->m_transactionsFrame->hide();
  m_ui->m_addressBookFrame->hide();
  m_ui->m_miningFrame->hide();

  m_tabActionGroup->addAction(m_ui->m_overviewAction);
  m_tabActionGroup->addAction(m_ui->m_sendAction);
  m_tabActionGroup->addAction(m_ui->m_receiveAction);
  m_tabActionGroup->addAction(m_ui->m_transactionsAction);
  m_tabActionGroup->addAction(m_ui->m_addressBookAction);
  m_tabActionGroup->addAction(m_ui->m_miningAction);

  m_ui->m_overviewAction->toggle();
  encryptedFlagChanged(false);
  statusBar()->addPermanentWidget(m_connectionStateIconLabel);
  statusBar()->addPermanentWidget(m_encryptionStateIconLabel);
  statusBar()->addPermanentWidget(m_synchronizationStateIconLabel);
  qobject_cast<AnimatedLabel*>(m_synchronizationStateIconLabel)->setSprite(QPixmap(":icons/sync_sprite"), QSize(16, 16), 5, 24);
  m_connectionStateIconLabel->setPixmap(QPixmap(":icons/disconnected").scaled(16, 16, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

#ifdef Q_OS_MAC
  installDockHandler();
#endif

#ifndef Q_OS_WIN
  m_ui->m_minimizeToTrayAction->deleteLater();
  m_ui->m_closeToTrayAction->deleteLater();
#endif
}

#ifdef Q_OS_WIN
void MainWindow::minimizeToTray(bool _on) {
  if (_on) {
    hide();
    m_trayIcon->show();
  } else {
    showNormal();
    activateWindow();
    m_trayIcon->hide();
  }
}
#endif

void MainWindow::scrollToTransaction(const QModelIndex& _index) {
  m_ui->m_transactionsAction->setChecked(true);
  m_ui->m_transactionsFrame->scrollToTransaction(_index);
}

void MainWindow::quit() {
  if (!m_isAboutToQuit) {
    ExitWidget* exitWidget = new ExitWidget(nullptr);
    exitWidget->show();
    m_isAboutToQuit = true;
    close();
  }
}

#ifdef Q_OS_MAC
void MainWindow::restoreFromDock() {
  if (m_isAboutToQuit) {
    return;
  }

  showNormal();
}
#endif

void MainWindow::closeEvent(QCloseEvent* _event) {
#ifdef Q_OS_WIN
  if (m_isAboutToQuit) {
    QMainWindow::closeEvent(_event);
    return;
  } else if (Settings::instance().isCloseToTrayEnabled()) {
    minimizeToTray(true);
    _event->ignore();
  } else {
    QApplication::quit();
    return;
  }
#elif defined(Q_OS_LINUX)
  if (!m_isAboutToQuit) {
    QApplication::quit();
    return;
  }
#endif
  QMainWindow::closeEvent(_event);

}

#ifdef Q_OS_WIN
void MainWindow::changeEvent(QEvent* _event) {
  QMainWindow::changeEvent(_event);
  if (!QSystemTrayIcon::isSystemTrayAvailable()) {
    QMainWindow::changeEvent(_event);
    return;
  }

  switch (_event->type()) {
  case QEvent::WindowStateChange:
    if(Settings::instance().isMinimizeToTrayEnabled()) {
      minimizeToTray(isMinimized());
    }
    break;
  default:
    break;
  }

  QMainWindow::changeEvent(_event);
}
#endif

bool MainWindow::event(QEvent* _event) {
  switch (static_cast<WalletEventType>(_event->type())) {
    case WalletEventType::ShowMessage:
    showMessage(static_cast<ShowMessageEvent*>(_event)->messageText(), static_cast<ShowMessageEvent*>(_event)->messageType());
    return true;
  }

  return QMainWindow::event(_event);
}

void MainWindow::createWallet() {
  QString filePath = QFileDialog::getSaveFileName(this, tr("New wallet file"),
  #ifdef Q_OS_WIN
      QApplication::applicationDirPath(),
  #else
      QDir::homePath(),
  #endif
      tr("Wallets (*.wallet)")
      );

    if (!filePath.isEmpty() && !filePath.endsWith(".wallet")) {
      filePath.append(".wallet");
    }

    if (!filePath.isEmpty() && !QFile::exists(filePath)) {
      if (WalletAdapter::instance().isOpen()) {
        WalletAdapter::instance().close();
      }

      WalletAdapter::instance().setWalletFile(filePath);
      WalletAdapter::instance().open("");
    }
}

void MainWindow::openWallet() {
  QString filePath = QFileDialog::getOpenFileName(this, tr("Open .wallet/.keys file"),
#ifdef Q_OS_WIN
    QApplication::applicationDirPath(),
#else
    QDir::homePath(),
#endif
    tr("Wallet (*.wallet *.keys)"));

  if (!filePath.isEmpty()) {
    if (WalletAdapter::instance().isOpen()) {
      WalletAdapter::instance().close();
    }

    WalletAdapter::instance().setWalletFile(filePath);
    WalletAdapter::instance().open("");
  }
}

void MainWindow::importKey() {
  ImportKeyDialog dlg(this);
  if (dlg.exec() == QDialog::Accepted) {
    QString keyString = dlg.getKeyString().trimmed();
    QString filePath = dlg.getFilePath();
    if (keyString.isEmpty() || filePath.isEmpty()) {
      return;
    }

    if (!filePath.endsWith(".wallet")) {
      filePath.append(".wallet");
    }

    uint64_t addressPrefix;
    std::string data;
    CryptoNote::AccountKeys keys;
    if (Tools::Base58::decode_addr(keyString.toStdString(), addressPrefix, data) && addressPrefix == CurrencyAdapter::instance().getAddressPrefix() &&
      data.size() == sizeof(keys)) {
      std::memcpy(&keys, data.data(), sizeof(keys));
      if (WalletAdapter::instance().isOpen()) {
        WalletAdapter::instance().close();
      }

      WalletAdapter::instance().setWalletFile(filePath);
      WalletAdapter::instance().createWithKeys(keys);
    }
  }
}

void MainWindow::ChangeLanguage() {
  ChangeLanguageDialog dlg(&MainWindow::instance());
  dlg.initLangList();
  if (dlg.exec() == QDialog::Accepted) {
    QString language = dlg.getLang();
    Settings::instance().setLanguage((language));
    QMessageBox::information(this, tr("Language was changed"), tr("The language will be changed after restarting the wallet."), QMessageBox::Ok);
  }
}

void MainWindow::DisplayCmdLineHelp() {
    CommandLineParser cmdLineParser(nullptr);
//  QMessageBox::information(nullptr, QObject::tr("Help"), cmdLineParser.getHelpText());
    QMessageBox *msg = new QMessageBox(QMessageBox::Information, QObject::tr("Help"),
                       cmdLineParser.getHelpText(),
                       QMessageBox::Ok, this);
    msg->setInformativeText(tr("More info can be found at www.karbowanec.com in Documentation section"));
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    msg->setFont(font);
    QSpacerItem* horizontalSpacer = new QSpacerItem(650, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)msg->layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    msg->exec();
}

void MainWindow::openConnectionSettings() {
    ConnectionSettingsDialog dlg(&MainWindow::instance());
    dlg.initConnectionSettings();
    dlg.setConnectionMode();
    dlg.setRemoteNode();
    dlg.setLocalDaemonPort();
    if (dlg.exec() == QDialog::Accepted) {

      QString connection = dlg.setConnectionMode();
      Settings::instance().setConnection(connection);

      QString remoteNode = dlg.setRemoteNode();
      Settings::instance().setCurrentRemoteNode(remoteNode);

      quint16 daemonPort = dlg.setLocalDaemonPort();
      Settings::instance().setCurrentLocalDaemonPort(daemonPort);

      QMessageBox::information(this, tr("Connection settings changed"), tr("Connection mode will be changed after restarting the wallet."), QMessageBox::Ok);
    }
}

void MainWindow::backupWallet() {
  QString filePath = QFileDialog::getSaveFileName(this, tr("Backup wallet to..."),
  #ifdef Q_OS_WIN
      QApplication::applicationDirPath(),
  #else
      QDir::homePath(),
  #endif
      tr("Wallets (*.wallet)")
      );
    if (!filePath.isEmpty() && !filePath.endsWith(".wallet")) {
      filePath.append(".wallet");
    }

    if (!filePath.isEmpty() && !QFile::exists(filePath)) {
      WalletAdapter::instance().backup(filePath);
    }
}

void MainWindow::resetWallet() {
  Q_ASSERT(WalletAdapter::instance().isOpen());
  if (QMessageBox::warning(this, tr("Warning"), tr("Your wallet will be reset and restored from blockchain.\n"
    "Are you sure?"), QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok) {
    WalletAdapter::instance().reset();
    WalletAdapter::instance().open("");
  }
}

void MainWindow::showPrivateKeys() {
  PrivateKeysDialog dlg(this);
  dlg.walletOpened();
  dlg.exec();
}

void MainWindow::onShowQR() {
  m_ui->m_receiveAction->trigger();
  m_ui->m_receiveFrame->closePaymentRequestForm();
}

void MainWindow::handlePaymentRequest(QString _request) {
  m_ui->m_sendAction->trigger();
  m_ui->m_sendFrame->parsePaymentRequest(_request);
  QWidget::activateWindow();
}

void MainWindow::onUriOpenSignal() {
  m_ui->m_sendAction->trigger();
}

void MainWindow::encryptWallet() {
  if (Settings::instance().isEncrypted()) {
    bool error = false;
    do {
      ChangePasswordDialog dlg(this);
      if (dlg.exec() == QDialog::Rejected) {
        return;
      }

      QString oldPassword = dlg.getOldPassword();
      QString newPassword = dlg.getNewPassword();
      error = !WalletAdapter::instance().changePassword(oldPassword, newPassword);
    } while (error);
  } else {
    NewPasswordDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
      QString password = dlg.getPassword();
      if (password.isEmpty()) {
        return;
      }

      encryptedFlagChanged(WalletAdapter::instance().changePassword("", password));
    }
  }
}

void MainWindow::aboutQt() {
  QMessageBox::aboutQt(this);
}

void MainWindow::setStartOnLogin(bool _on) {
  Settings::instance().setStartOnLoginEnabled(_on);
  m_ui->m_startOnLoginAction->setChecked(Settings::instance().isStartOnLoginEnabled());
}

void MainWindow::setMinimizeToTray(bool _on) {
#ifdef Q_OS_WIN
  Settings::instance().setMinimizeToTrayEnabled(_on);
#endif
}

void MainWindow::setCloseToTray(bool _on) {
#ifdef Q_OS_WIN
  Settings::instance().setCloseToTrayEnabled(_on);
#endif
}

void MainWindow::about() {
  AboutDialog dlg(this);
  dlg.exec();
}

void MainWindow::setStatusBarText(const QString& _text) {
  statusBar()->showMessage(_text);
}

void MainWindow::showMessage(const QString& _text, QtMsgType _type) {
  switch (_type) {
  case QtCriticalMsg:
    QMessageBox::critical(this, tr("Wallet error"), _text);
    break;
  case QtDebugMsg:
    QMessageBox::information(this, tr("Wallet"), _text);
    break;
  default:
    break;
  }
}

void MainWindow::askForWalletPassword(bool _error) {
  PasswordDialog dlg(_error, this);
  if (dlg.exec() == QDialog::Accepted) {
    QString password = dlg.getPassword();
    WalletAdapter::instance().open(password);
  }
}

void MainWindow::encryptedFlagChanged(bool _encrypted) {
  m_ui->m_encryptWalletAction->setEnabled(!_encrypted);
  m_ui->m_changePasswordAction->setEnabled(_encrypted);
  QString encryptionIconPath = _encrypted ? ":icons/encrypted" : ":icons/decrypted";
  QPixmap encryptionIcon = QPixmap(encryptionIconPath).scaled(16, 16, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  m_encryptionStateIconLabel->setPixmap(encryptionIcon);
  QString encryptionLabelTooltip = _encrypted ? tr("Encrypted") : tr("Not encrypted");
  m_encryptionStateIconLabel->setToolTip(encryptionLabelTooltip);
}

void MainWindow::peerCountUpdated(quint64 _peerCount) {
  QString connectionIconPath = _peerCount > 0 ? ":icons/connected" : ":icons/disconnected";
  QPixmap connectionIcon = QPixmap(connectionIconPath).scaled(16, 16, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  m_connectionStateIconLabel->setPixmap(connectionIcon);
  m_connectionStateIconLabel->setToolTip(QString(tr("%1 peers").arg(_peerCount)));
}

void MainWindow::walletSynchronizationInProgress() {
  qobject_cast<AnimatedLabel*>(m_synchronizationStateIconLabel)->startAnimation();
  m_synchronizationStateIconLabel->setToolTip(tr("Synchronization in progress"));
}

void MainWindow::walletSynchronized(int _error, const QString& _error_text) {
  QPixmap syncIcon = QPixmap(":icons/synced").scaled(16, 16, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  qobject_cast<AnimatedLabel*>(m_synchronizationStateIconLabel)->stopAnimation();
  m_synchronizationStateIconLabel->setPixmap(syncIcon);
  QString syncLabelTooltip = _error > 0 ? tr("Not synchronized") : tr("Synchronized");
  m_synchronizationStateIconLabel->setToolTip(syncLabelTooltip);
}

void MainWindow::walletOpened(bool _error, const QString& _error_text) {
  if (!_error) {
    m_encryptionStateIconLabel->show();
    m_synchronizationStateIconLabel->show();
    m_ui->m_backupWalletAction->setEnabled(true);
    m_ui->m_showPrivateKey->setEnabled(true);
	m_ui->m_resetAction->setEnabled(true);
    encryptedFlagChanged(Settings::instance().isEncrypted());

    QList<QAction*> tabActions = m_tabActionGroup->actions();
    Q_FOREACH(auto action, tabActions) {
      action->setEnabled(true);
    }

    m_ui->m_overviewAction->trigger();
    m_ui->m_accountFrame->show();
    m_ui->m_overviewFrame->show();
    m_ui->m_receiveFrame->closePaymentRequestForm();
  } else {
    walletClosed();
  }
}

void MainWindow::walletClosed() {
  m_ui->m_backupWalletAction->setEnabled(false);
  m_ui->m_encryptWalletAction->setEnabled(false);
  m_ui->m_changePasswordAction->setEnabled(false);
  m_ui->m_resetAction->setEnabled(false);
  m_ui->m_overviewFrame->hide();
  m_ui->m_accountFrame->hide();
  m_ui->m_receiveFrame->hide();
  m_ui->m_sendFrame->hide();
  m_ui->m_transactionsFrame->hide();
  m_ui->m_addressBookFrame->hide();
  m_ui->m_miningFrame->hide();
  m_encryptionStateIconLabel->hide();
  m_synchronizationStateIconLabel->hide();
  QList<QAction*> tabActions = m_tabActionGroup->actions();
  Q_FOREACH(auto action, tabActions) {
    action->setEnabled(false);
  }
}

#ifdef Q_OS_WIN
void MainWindow::trayActivated(QSystemTrayIcon::ActivationReason _reason) {
  showNormal();
  m_trayIcon->hide();
}
#endif

}
