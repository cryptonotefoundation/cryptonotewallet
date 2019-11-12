// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2011-2013 The Bitcoin Core developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2018 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QCloseEvent>
#include <QFileDialog>
#include <QStandardPaths>
#include <QInputDialog>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QTimer>
#include <QLocale>
#include <QDateTime>
#include <QTranslator>
#include <QToolButton>
#include <QPushButton>
#include <QFontDatabase>
#include <Common/Base58.h>
#include <Common/StringTools.h>
#include <Common/Util.h>
#include "AboutDialog.h"
#include "AnimatedLabel.h"
#include "AddressBookModel.h"
#include "ChangePasswordDialog.h"
#include "ConnectionSettings.h"
#include "OptimizationSettings.h"
#include "PrivateKeysDialog.h"
#include "ImportKeyDialog.h"
#include "ImportKeysDialog.h"
#include "ExportTrackingKeyDialog.h"
#include "ImportTrackingKeyDialog.h"
#include "RestoreFromMnemonicSeedDialog.h"
#include "SignMessageDialog.h"
#include "CurrencyAdapter.h"
#include "ExitWidget.h"
#include "GetBalanceProofDialog.h"
#include "MainWindow.h"
#include "NewPasswordDialog.h"
#include "NodeAdapter.h"
#include "PasswordDialog.h"
#include "Settings.h"
#include "WalletAdapter.h"
#include "WalletEvents.h"
#include "SendFrame.h"
#include "InfoDialog.h"
#include "ui_mainwindow.h"
#include "MnemonicSeedDialog.h"
#include "ConfirmSendDialog.h"
#include "TranslatorManager.h"

#ifdef Q_OS_MAC
#include "macdockiconhandler.h"
#endif

#include "Mnemonics/electrum-words.h"

extern "C"
{
#include "crypto/keccak.h"
#include "crypto/crypto-ops.h"
}

namespace WalletGui {

MainWindow* MainWindow::m_instance = nullptr;

MainWindow& MainWindow::instance() {
  if (m_instance == nullptr) {
    m_instance = new MainWindow;
  }

  return *m_instance;
}

MainWindow::MainWindow() : QMainWindow(), m_ui(new Ui::MainWindow), m_trayIcon(nullptr), m_tabActionGroup(new QActionGroup(this)),
  m_isAboutToQuit(false), paymentServer(0), optimizationManager(nullptr), maxRecentFiles(10), trayIconMenu(0), toggleHideAction(0) {
  m_ui->setupUi(this);
  m_connectionStateIconLabel = new QPushButton();
  m_connectionStateIconLabel->setFlat(true); // Make the button look like a label, but clickable
  m_connectionStateIconLabel->setStyleSheet(".QPushButton { background-color: rgba(255, 255, 255, 0);}");
  m_connectionStateIconLabel->setMaximumSize(16, 16);
  m_encryptionStateIconLabel = new QLabel(this);
  m_trackingModeIconLabel = new QLabel(this);
  m_remoteModeIconLabel = new QLabel(this);
  m_synchronizationStateIconLabel = new AnimatedLabel(this);
  connectToSignals();
  createLanguageMenu();
  initUi();
  walletClosed();
}

MainWindow::~MainWindow() {
    delete paymentServer;
    paymentServer = 0;
    delete optimizationManager;
    optimizationManager = 0;
    //if(m_trayIcon) // Hide tray icon, as deleting will let it linger until quit (on Ubuntu)
    //  m_trayIcon->hide();
    #ifdef Q_OS_MAC
      MacDockIconHandler::cleanup();
    #endif
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
  connect(&WalletAdapter::instance(), &WalletAdapter::walletTransactionCreatedSignal, this, [this]() {
      QApplication::alert(this);
  });
  connect(&WalletAdapter::instance(), &WalletAdapter::walletUnmixableBalanceUpdatedSignal, this, &MainWindow::updateUnmixableBalance,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletSendTransactionCompletedSignal, this, [this](CryptoNote::TransactionId _transactionId, int _error, const QString& _errorString) {
    if (_error == 0) {
      m_ui->m_transactionsAction->setChecked(true);
    }
  });
  connect(&NodeAdapter::instance(), &NodeAdapter::peerCountUpdatedSignal, this, &MainWindow::peerCountUpdated, Qt::QueuedConnection);
  connect(m_ui->m_exitAction, &QAction::triggered, qApp, &QApplication::quit);
  connect(m_ui->m_accountFrame, &AccountFrame::showQRcodeSignal, this, &MainWindow::onShowQR, Qt::QueuedConnection);
  connect(m_ui->m_sendFrame, &SendFrame::uriOpenSignal, this, &MainWindow::onUriOpenSignal, Qt::QueuedConnection);
  connect(m_ui->m_noWalletFrame, &NoWalletFrame::createWalletClickedSignal, this, &MainWindow::createWallet, Qt::QueuedConnection);
  connect(m_ui->m_noWalletFrame, &NoWalletFrame::openWalletClickedSignal, this, &MainWindow::openWallet, Qt::QueuedConnection);
  connect(m_ui->m_addressBookFrame, &AddressBookFrame::payToSignal, this, &MainWindow::payTo);
  connect(m_connectionStateIconLabel, SIGNAL(clicked()), this, SLOT(showStatusInfo()));
}

void MainWindow::setMainWindowTitle() {
  setWindowTitle(QString(tr("Karbo Wallet %1")).arg(Settings::instance().getVersion()));
}
void MainWindow::initUi() {
  setMainWindowTitle();
#ifdef Q_OS_WIN32
  createTrayIcon();
#endif

  m_ui->accountToolBar->setAllowedAreas(Qt::TopToolBarArea);

  accountWidget = m_ui->accountToolBar->addWidget(m_ui->m_accountFrame);
  m_ui->accountToolBar->setMovable(false);
  addToolBar(Qt::TopToolBarArea, m_ui->accountToolBar);
  addToolBarBreak();
  addToolBar(Qt::LeftToolBarArea, m_ui->toolBar);
  QToolButton button;
  button.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  button.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

  m_ui->m_aboutCryptonoteAction->setText(QString(tr("About %1 Wallet")).arg(CurrencyAdapter::instance().getCurrencyDisplayName()));
  m_ui->m_overviewFrame->hide();
  m_ui->m_sendFrame->hide();
  accountWidget->setVisible(false);
  m_ui->m_receiveFrame->hide();
  m_ui->m_transactionsFrame->hide();
  m_ui->m_addressBookFrame->hide();

  m_tabActionGroup->addAction(m_ui->m_overviewAction);
  m_tabActionGroup->addAction(m_ui->m_sendAction);
  m_tabActionGroup->addAction(m_ui->m_receiveAction);
  m_tabActionGroup->addAction(m_ui->m_transactionsAction);
  m_tabActionGroup->addAction(m_ui->m_addressBookAction);

  m_ui->m_overviewAction->toggle();
  encryptedFlagChanged(false);
  statusBar()->addPermanentWidget(m_trackingModeIconLabel);
  statusBar()->addPermanentWidget(m_remoteModeIconLabel);
  statusBar()->addPermanentWidget(m_connectionStateIconLabel);
  statusBar()->addPermanentWidget(m_encryptionStateIconLabel);
  statusBar()->addPermanentWidget(m_synchronizationStateIconLabel);
  qobject_cast<AnimatedLabel*>(m_synchronizationStateIconLabel)->setSprite(QPixmap(":icons/sync_sprite"), QSize(16, 16), 5, 24);
  m_connectionStateIconLabel->setIcon(QPixmap(":icons/disconnected").scaled(16, 16, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
  m_trackingModeIconLabel->setPixmap(QPixmap(":icons/tracking").scaledToHeight(16, Qt::SmoothTransformation));
  m_remoteModeIconLabel->hide();
  m_trackingModeIconLabel->hide();
  m_trackingModeIconLabel->setToolTip(tr("Tracking wallet. Spending unawailable"));
  m_remoteModeIconLabel->setToolTip(tr("Connected through remote node"));

  QString connection = Settings::instance().getConnection();
  if(connection.compare("remote") == 0) {
    m_remoteModeIconLabel->show();
    m_remoteModeIconLabel->setPixmap(QPixmap(":icons/remote_mode").scaledToHeight(16, Qt::SmoothTransformation));
  }

  m_ui->m_showMnemonicSeedAction->setEnabled(false);

  m_ui->m_startOnLoginAction->setChecked(Settings::instance().isStartOnLoginEnabled());
  m_ui->m_hideFusionTransactions->setChecked(Settings::instance().skipFusionTransactions());

  m_ui->menuRecent_wallets->setVisible(false);
  QAction* recentWalletAction = 0;
  for(int i = 0; i < maxRecentFiles; ++i){
    recentWalletAction = new QAction(this);
    recentWalletAction->setVisible(false);
    QObject::connect(recentWalletAction, SIGNAL(triggered()), this, SLOT(openRecent()));
    recentFileActionList.append(recentWalletAction);
  }
  for(int i = 0; i < maxRecentFiles; ++i)
     m_ui->menuRecent_wallets->addAction(recentFileActionList.at(i));
  updateRecentActionList();

#ifdef Q_OS_MAC
  installDockHandler();
#endif

#ifdef Q_OS_WIN
  m_ui->m_minimizeToTrayAction->setVisible(true);
  m_ui->m_closeToTrayAction->setVisible(true);
  m_ui->m_minimizeToTrayAction->setChecked(Settings::instance().isMinimizeToTrayEnabled());
  m_ui->m_closeToTrayAction->setChecked(Settings::instance().isCloseToTrayEnabled());
  toggleHideAction = new QAction(tr("&Show / Hide"), this);
  toggleHideAction->setStatusTip(tr("Show or hide the main window"));
  connect(toggleHideAction, SIGNAL(triggered()), this, SLOT(toggleHidden()));
#endif

#ifndef Q_OS_WIN
  m_ui->m_minimizeToTrayAction->deleteLater();
  m_ui->m_closeToTrayAction->deleteLater();
#endif

  OptimizationManager* optimizationManager = new OptimizationManager(this);
  createTrayIconMenu();
}

void MainWindow::scrollToTransaction(const QModelIndex& _index) {
  m_ui->m_transactionsAction->setChecked(true);
  m_ui->m_transactionsFrame->scrollToTransaction(_index);
}

void MainWindow::quit() {
  if (!m_isAboutToQuit) {
    ExitWidget* exitWidget = new ExitWidget(nullptr);
    exitWidget->show();
    m_isAboutToQuit = true;
    if(m_trayIcon) // Hide tray icon, as deleting will let it linger until quit (on Ubuntu)
       m_trayIcon->hide();
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

void MainWindow::changeEvent(QEvent* _event) {
#ifdef Q_OS_WIN
  if (!QSystemTrayIcon::isSystemTrayAvailable()) {
    QMainWindow::changeEvent(_event);
    return;
  }
#endif
  switch (_event->type()) {
  // this event is send if a translator is loaded
  case QEvent::LanguageChange:
  {
    //m_ui->retranslateUi(this);
    setMainWindowTitle();
    break;
  }
  // this event is send, if the system language changes
  case QEvent::LocaleChange:
  {
    QString locale = QLocale::system().name();
    locale.truncate(locale.lastIndexOf('_'));
    loadLanguage(locale);
  }
  case QEvent::WindowStateChange:
  {
#ifdef Q_OS_WIN
    if(Settings::instance().isMinimizeToTrayEnabled()) {
      minimizeToTray(isMinimized());
    }
    break;
#endif
  }
  default:
    break;
  }
  QWidget::changeEvent(_event);
  QMainWindow::changeEvent(_event);
}

bool MainWindow::event(QEvent* _event) {
  switch (static_cast<WalletEventType>(_event->type())) {
    case WalletEventType::ShowMessage:
    {
      showMessage(static_cast<ShowMessageEvent*>(_event)->messageText(), static_cast<ShowMessageEvent*>(_event)->messageType());
      return true;
    }
  }

  return QMainWindow::event(_event);
}

void MainWindow::createWallet() {
  QString filePath = QFileDialog::getSaveFileName(this, tr("New wallet file"),
  #ifdef Q_OS_WIN
      //QApplication::applicationDirPath(),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),

  #else
      QDir::homePath(),
  #endif
      tr("Wallets (*.wallet)")
      );

    if (!filePath.isEmpty() && !filePath.endsWith(".wallet")) {
      filePath.append(".wallet");
    }

    if (QFile::exists(filePath)) {
        QFile::remove(filePath);
    }

    if (!filePath.isEmpty()) {
      if (WalletAdapter::instance().isOpen()) {
        WalletAdapter::instance().close();
      }

      WalletAdapter::instance().setWalletFile(filePath);
      WalletAdapter::instance().createWallet();
    }
}

void MainWindow::createNonDeterministicWallet() {
  QString filePath = QFileDialog::getSaveFileName(this, tr("New wallet file"),
  #ifdef Q_OS_WIN
      //QApplication::applicationDirPath(),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
  #else
      QDir::homePath(),
  #endif
      tr("Wallets (*.wallet)")
      );

    if (!filePath.isEmpty() && !filePath.endsWith(".wallet")) {
      filePath.append(".wallet");
    }

    if (QFile::exists(filePath)) {
        QFile::remove(filePath);
    }

    if (!filePath.isEmpty()) {
      if (WalletAdapter::instance().isOpen()) {
        WalletAdapter::instance().close();
      }

      WalletAdapter::instance().setWalletFile(filePath);
      WalletAdapter::instance().createNonDeterministic();
    }
}

void MainWindow::openWallet() {
  QString walletDirectory = "";
  QString lastwalletDir = QFileInfo(Settings::instance().getWalletFile()).absolutePath();
  if (!lastwalletDir.isEmpty()) {
    walletDirectory = lastwalletDir;
  } else {
#ifdef Q_OS_WIN
    walletDirectory = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
#else
    walletDirectory = QDir::homePath();
#endif
  }
  QString filePath = QFileDialog::getOpenFileName(this, tr("Open .wallet/.keys file"),
    walletDirectory,
    tr("Wallet (*.wallet *.keys)"));

  if (!filePath.isEmpty()) {

    if (QFile::exists(filePath) && (!filePath.endsWith(".keys") && !filePath.endsWith(".wallet") && !filePath.endsWith(".trackingwallet"))) {
      QMessageBox::warning(this, tr("Wrong wallet file extension"),
                                 tr("Wrong wallet file extension, wallet file should have \".wallet\", \".keys\" or \".trackingwallet\" extension."), QMessageBox::Ok);
      return;
    }

    if (WalletAdapter::instance().isOpen()) {
      WalletAdapter::instance().close();
    }

    WalletAdapter::instance().setWalletFile(filePath);
    WalletAdapter::instance().open("");
  }
}

void MainWindow::openRecent(){
  QAction *action = qobject_cast<QAction *>(sender());
  if (action) {
    QString filePath = action->data().toString();
    if (!filePath.isEmpty() && QFile::exists(filePath)) {
      if (WalletAdapter::instance().isOpen()) {
          WalletAdapter::instance().close();
      }
      WalletAdapter::instance().setWalletFile(filePath);
      WalletAdapter::instance().open("");
    } else {
       QMessageBox::warning(this, tr("Recent wallet file not found"), tr("The recent wallet file is missing. Probably it was removed."), QMessageBox::Ok);
    }
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
    } else {
      QMessageBox::warning(this, tr("Wallet keys are not valid"), tr("The private keys you entered are not valid."), QMessageBox::Ok);
    }
  }
}

void MainWindow::importKeys() {
  ImportKeysDialog dlg(this);
  if (dlg.exec() == QDialog::Accepted) {
    QString viewKeyString = dlg.getViewKeyString().trimmed();
    QString spendKeyString = dlg.getSpendKeyString().trimmed();
    QString filePath = dlg.getFilePath();
    if (viewKeyString.isEmpty() || spendKeyString.isEmpty() || filePath.isEmpty()) {
      return;
    }

    if (!filePath.endsWith(".wallet")) {
      filePath.append(".wallet");
    }

    uint64_t addressPrefix;
    std::string data;
    CryptoNote::AccountKeys keys;

    std::string private_spend_key_string = spendKeyString.toStdString();
    std::string private_view_key_string = viewKeyString.toStdString();

    Crypto::Hash private_spend_key_hash;
    Crypto::Hash private_view_key_hash;

    size_t size;
    if (!Common::fromHex(private_spend_key_string, &private_spend_key_hash, sizeof(private_spend_key_hash), size) || size != sizeof(private_spend_key_hash)) {
      QMessageBox::warning(this, tr("Key is not valid"), tr("The private spend key you entered is not valid."), QMessageBox::Ok);
      return;
    }
    if (!Common::fromHex(private_view_key_string, &private_view_key_hash, sizeof(private_view_key_hash), size) || size != sizeof(private_view_key_hash)) {
      QMessageBox::warning(this, tr("Key is not valid"), tr("The private view key you entered is not valid."), QMessageBox::Ok);
      return;
    }

    keys.spendSecretKey = *(struct Crypto::SecretKey *) &private_spend_key_hash;
    keys.viewSecretKey = *(struct Crypto::SecretKey *) &private_view_key_hash;

    Crypto::secret_key_to_public_key(keys.spendSecretKey, keys.address.spendPublicKey);
    Crypto::secret_key_to_public_key(keys.viewSecretKey, keys.address.viewPublicKey);

    if (WalletAdapter::instance().isOpen()) {
        WalletAdapter::instance().close();
    }
    WalletAdapter::instance().setWalletFile(filePath);
    WalletAdapter::instance().createWithKeys(keys);

  }
}

void MainWindow::importTrackingKey() {
  ImportTrackingKeyDialog dlg(this);
  if (dlg.exec() == QDialog::Accepted) {
    QString keyString = dlg.getKeyString().trimmed();
    QString filePath = dlg.getFilePath();
    if (keyString.isEmpty() || filePath.isEmpty()) {
      return;
    }
    if (keyString.size() != 256) {
      QMessageBox::warning(this, tr("Tracking key is not valid"), tr("The tracking key you entered is not valid."), QMessageBox::Ok);
      return;
    }

    if (!filePath.endsWith(".wallet")) {
      filePath.append(".wallet");
    }

    CryptoNote::AccountKeys keys;

    //  XDN style tracking key import
    //  uint64_t addressPrefix;
    //  std::string data;

    //  if (Tools::Base58::decode_addr(keyString.toStdString(), addressPrefix, data) && addressPrefix == CurrencyAdapter::instance().getAddressPrefix() &&
    //    data.size() == sizeof(keys)) {
    //    std::memcpy(&keys, data.data(), sizeof(keys));

    // To prevent confusing with import of private key / paperwallet lets use Bytecoin style tracking keys, they look different
    std::string public_spend_key_string = keyString.mid(0,64).toStdString();
    std::string public_view_key_string = keyString.mid(64,64).toStdString();
    std::string private_spend_key_string = keyString.mid(128,64).toStdString();
    std::string private_view_key_string = keyString.mid(192,64).toStdString();

    Crypto::Hash public_spend_key_hash;
    Crypto::Hash public_view_key_hash;
    Crypto::Hash private_spend_key_hash;
    Crypto::Hash private_view_key_hash;

    size_t size;
    if (!Common::fromHex(public_spend_key_string, &public_spend_key_hash, sizeof(public_spend_key_hash), size) || size != sizeof(public_spend_key_hash)) {
      QMessageBox::warning(this, tr("Key is not valid"), tr("The public spend key you entered is not valid."), QMessageBox::Ok);
      return;
    }
    if (!Common::fromHex(public_view_key_string, &public_view_key_hash, sizeof(public_view_key_hash), size) || size != sizeof(public_view_key_hash)) {
      QMessageBox::warning(this, tr("Key is not valid"), tr("The public view key you entered is not valid."), QMessageBox::Ok);
      return;
    }
    if (!Common::fromHex(private_spend_key_string, &private_spend_key_hash, sizeof(private_spend_key_hash), size) || size != sizeof(private_spend_key_hash)) {
      QMessageBox::warning(this, tr("Key is not valid"), tr("The private spend key you entered is not valid."), QMessageBox::Ok);
      return;
    }
    if (!Common::fromHex(private_view_key_string, &private_view_key_hash, sizeof(private_view_key_hash), size) || size != sizeof(private_view_key_hash)) {
      QMessageBox::warning(this, tr("Key is not valid"), tr("The private view key you entered is not valid."), QMessageBox::Ok);
      return;
    }

    Crypto::PublicKey public_spend_key = *(struct Crypto::PublicKey *) &public_spend_key_hash;
    Crypto::PublicKey public_view_key = *(struct Crypto::PublicKey *) &public_view_key_hash;
    Crypto::SecretKey private_spend_key = *(struct Crypto::SecretKey *) &private_spend_key_hash;
    Crypto::SecretKey private_view_key = *(struct Crypto::SecretKey *) &private_view_key_hash;

    keys.address.spendPublicKey = public_spend_key;
    keys.address.viewPublicKey = public_view_key;
    keys.spendSecretKey = private_spend_key;
    keys.viewSecretKey = private_view_key;

      if (WalletAdapter::instance().isOpen()) {
        WalletAdapter::instance().close();
      }
      Settings::instance().setTrackingMode(true);
      WalletAdapter::instance().setWalletFile(filePath);
      WalletAdapter::instance().createWithKeys(keys);
   // }
  }
}

void MainWindow::isTrackingMode() {
  m_ui->m_sendFrame->hide();
  m_ui->m_overviewAction->trigger();
  m_ui->m_sendAction->setEnabled(false);
  m_ui->m_openUriAction->setEnabled(false);
  m_ui->m_showMnemonicSeedAction->setEnabled(false);
  m_ui->m_sweepUnmixableAction->setEnabled(false);
  m_ui->m_optimizationAction->setEnabled(false);
  m_ui->m_proofBalanceAction->setEnabled(false);
  m_trackingModeIconLabel->show();
}

void MainWindow::restoreFromMnemonicSeed() {
  RestoreFromMnemonicSeedDialog dlg(this);
  if (dlg.exec() == QDialog::Accepted) {
    QString mnemonicString = dlg.getSeedString().trimmed();
    QString filePath = dlg.getFilePath();
    if (mnemonicString.isEmpty() || filePath.isEmpty()) {
      return;
    }

    if (!filePath.endsWith(".wallet")) {
      filePath.append(".wallet");
    }

    CryptoNote::AccountKeys keys;
    std::string seed_language = "English";
    if(Crypto::ElectrumWords::words_to_bytes(mnemonicString.toStdString(), keys.spendSecretKey, seed_language)) {
      Crypto::secret_key_to_public_key(keys.spendSecretKey,keys.address.spendPublicKey);
      Crypto::SecretKey second;
      keccak((uint8_t *)&keys.spendSecretKey, sizeof(Crypto::SecretKey), (uint8_t *)&second, sizeof(Crypto::SecretKey));
      Crypto::generate_deterministic_keys(keys.address.viewPublicKey,keys.viewSecretKey,second);

      if (WalletAdapter::instance().isOpen()) {
        WalletAdapter::instance().close();
      }
      WalletAdapter::instance().setWalletFile(filePath);
      WalletAdapter::instance().createWithKeys(keys);
    } else {
      QMessageBox::critical(nullptr, tr("Mnemonic seed is not correct"), tr("There must be an error in mnemonic seed. Make sure you entered it correctly."), QMessageBox::Ok);
      return;
    }
  }
}

void MainWindow::sweepUnmixable() {
  quint64 dust = WalletAdapter::instance().getUnmixableBalance();
  ConfirmSendDialog dlg(&MainWindow::instance());
  dlg.showPasymentDetails(dust);
  if (dlg.exec() == QDialog::Accepted) {
    quint64 fee = CurrencyAdapter::instance().getMinimumFee();
    std::vector<CryptoNote::WalletLegacyTransfer> walletTransfers;
    CryptoNote::WalletLegacyTransfer walletTransfer;
    walletTransfer.address = WalletAdapter::instance().getAddress().toStdString();
    walletTransfer.amount = dust;
    walletTransfers.push_back(walletTransfer);

    WalletAdapter::instance().sweepDust(walletTransfers, fee, "", 0);
  }
}

void MainWindow::createLanguageMenu(void)
{
  QActionGroup* langGroup = new QActionGroup(m_ui->menuLanguage);
  langGroup->setExclusive(true);
  connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));
  QString defaultLocale = Settings::instance().getLanguage();
  if (defaultLocale.isEmpty()){
    defaultLocale = QLocale::system().name();
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));
  }
#if defined(_MSC_VER)
  m_langPath = QApplication::applicationDirPath();
  m_langPath.append("/languages");
#elif defined(Q_OS_MAC)
  m_langPath = QApplication::applicationDirPath();
  m_langPath = m_langPath + "/../Resources/languages/";
#else
  m_langPath = "/opt/karbo/languages";
#endif
  QDir dir(m_langPath);
  QStringList fileNames = dir.entryList(QStringList("??.qm"));
  for (int i = 0; i < fileNames.size(); ++i) {
    QString locale;
    locale = fileNames[i];
    locale.truncate(locale.lastIndexOf('.'));
    QString lang = QLocale(locale).nativeLanguageName();
    QAction *action = new QAction(lang, this);
    action->setCheckable(true);
    action->setData(locale);
    m_ui->menuLanguage->addAction(action);
    langGroup->addAction(action);

    // set default translators and language checked
    if (defaultLocale == locale)
    {
      action->setChecked(true);
    }
  }
}

void MainWindow::slotLanguageChanged(QAction* action)
{
  if(0 != action) {
    // load the language dependant on the action content
    QString lang = action->data().toString();
    loadLanguage(lang);
    // save is in settings
    Settings::instance().setLanguage((lang));
  }
}

void MainWindow::loadLanguage(const QString& rLanguage)
{
  if(m_currLang != rLanguage) {
    m_currLang = rLanguage;
    QLocale locale = QLocale(m_currLang);
    QLocale::setDefault(locale);
    QString languageName = QLocale::languageToString(locale.language());
    //TranslatorManager::instance()->switchTranslator(m_translator, QString("%1.qm").arg(rLanguage));
    //TranslatorManager::instance()->switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));
    Settings::instance().setLanguage((m_currLang));
    m_ui->statusBar->showMessage(tr("Language changed to %1").arg(languageName));
    QMessageBox::information(this, tr("Language was changed"),
       tr("Language changed to %1. The change will take effect after restarting the wallet.").arg(languageName), QMessageBox::Ok);
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

void MainWindow::openOptimizationSettings() {
  OptimizationSettingsDialog dlg(&MainWindow::instance());
  dlg.exec();
}

void MainWindow::getBalanceProof() {
  GetBalanceProofDialog dlg(&MainWindow::instance());
  dlg.exec();
}

void MainWindow::showStatusInfo() {
  InfoDialog dlg(this);
  dlg.exec();
}

void MainWindow::backupWallet() {
  QString filePath = QFileDialog::getSaveFileName(this, tr("Backup wallet to..."),
  #ifdef Q_OS_WIN
      //QApplication::applicationDirPath(),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
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

void MainWindow::openLogFile() {
  QString pathLog = Settings::instance().getDataDir().absoluteFilePath(QApplication::applicationName() + ".log");
  if (!pathLog.isEmpty()) {
    QDesktopServices::openUrl(QUrl::fromLocalFile(pathLog));
  }
}

void MainWindow::showPrivateKeys() {
  PrivateKeysDialog dlg(this);
  dlg.walletOpened();
  dlg.exec();
}

void MainWindow::showMnemonicSeed() {
  MnemonicSeedDialog dlg(this);
  dlg.walletOpened();
  dlg.exec();
}

void MainWindow::exportTrackingKey() {
  ExportTrackingKeyDialog dlg(this);
  dlg.walletOpened();
  dlg.exec();
}

void MainWindow::signMessage() {
  SignMessageDialog dlg(this);
  dlg.walletOpened();
  dlg.sign();
  dlg.exec();
}

void MainWindow::verifyMessage() {
  SignMessageDialog dlg(this);
  dlg.walletOpened();
  dlg.verify();
  dlg.exec();
}

void MainWindow::onShowQR() {
  m_ui->m_receiveAction->trigger();
  m_ui->m_receiveFrame->closePaymentRequestForm();
}

void MainWindow::handlePaymentRequest(QString _request) {
  if (Settings::instance().isTrackingMode()) {
      isTrackingMode();
      return;
  }
  m_ui->m_sendAction->trigger();
  m_ui->m_sendFrame->parsePaymentRequest(_request);
  QWidget::activateWindow();
}

void MainWindow::onUriOpenSignal() {
  if (Settings::instance().isTrackingMode()) {
      isTrackingMode();
      return;
  }
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

void MainWindow::closeWallet() {
  if (WalletAdapter::instance().isOpen()) {
    WalletAdapter::instance().close();
  }
}

void MainWindow::updateRecentActionList(){
  QStringList recentFilePaths = Settings::instance().getRecentWalletsList();
  if(recentFilePaths.isEmpty())
    m_ui->menuRecent_wallets->setVisible(false);

  if(recentFilePaths.size() != 0) {
    int itEnd = 0;
    if(recentFilePaths.size() <= maxRecentFiles)
        itEnd = recentFilePaths.size();
    else
        itEnd = maxRecentFiles;

    for (int i = 0; i < itEnd; ++i) {
        QString strippedName = QFileInfo(recentFilePaths.at(i)).absoluteFilePath();
        recentFileActionList.at(i)->setText(strippedName);
        recentFileActionList.at(i)->setData(recentFilePaths.at(i));
        recentFileActionList.at(i)->setVisible(true);
    }
    for (int i = itEnd; i < maxRecentFiles; ++i)
        recentFileActionList.at(i)->setVisible(false);
  } else {
      m_ui->menuRecent_wallets->setVisible(false);
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
  m_ui->m_minimizeToTrayAction->setChecked(Settings::instance().isMinimizeToTrayEnabled());
#endif
}

void MainWindow::setCloseToTray(bool _on) {
#ifdef Q_OS_WIN
  Settings::instance().setCloseToTrayEnabled(_on);
  m_ui->m_closeToTrayAction->setChecked(Settings::instance().isCloseToTrayEnabled());
#endif
}

void MainWindow::setHideFusionTransactions(bool _on) {
  Settings::instance().setSkipFusionTransactions(_on);
  m_ui->m_hideFusionTransactions->setChecked(Settings::instance().skipFusionTransactions());
  m_ui->m_transactionsFrame->reloadTransactions();
  m_ui->m_overviewFrame->reloadTransactions();
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
  m_connectionStateIconLabel->setIcon(connectionIcon);
  m_connectionStateIconLabel->setToolTip(QString(tr("%n active connection(s)", "", _peerCount)));
}

void MainWindow::walletSynchronizationInProgress() {
  qobject_cast<AnimatedLabel*>(m_synchronizationStateIconLabel)->startAnimation();
  m_synchronizationStateIconLabel->setToolTip(tr("Synchronization in progress"));
  m_ui->m_proofBalanceAction->setEnabled(false);
}

void MainWindow::walletSynchronized(int _error, const QString& _error_text) {
  QPixmap syncIcon = QPixmap(":icons/synced").scaled(16, 16, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  qobject_cast<AnimatedLabel*>(m_synchronizationStateIconLabel)->stopAnimation();
  m_synchronizationStateIconLabel->setPixmap(syncIcon);
  QString syncLabelTooltip = _error > 0 ? tr("Not synchronized") : tr("Synchronized");
  m_synchronizationStateIconLabel->setToolTip(syncLabelTooltip);
  if (WalletAdapter::instance().getActualBalance() > 0 && !(Settings::instance().isTrackingMode())) {
    m_ui->m_proofBalanceAction->setEnabled(true);
  }
}

void MainWindow::walletOpened(bool _error, const QString& _error_text) {
  if (!_error) {
    m_ui->m_noWalletFrame->hide();
	m_ui->accountToolBar->show();
    m_ui->m_closeWalletAction->setEnabled(true);
    m_ui->m_exportTrackingKeyAction->setEnabled(true);
    m_encryptionStateIconLabel->show();
    m_synchronizationStateIconLabel->show();
    m_ui->m_backupWalletAction->setEnabled(true);
    m_ui->m_showPrivateKey->setEnabled(true);
    m_ui->m_resetAction->setEnabled(true);
    m_ui->m_openUriAction->setEnabled(true);
    m_ui->m_sweepUnmixableAction->setEnabled(true);
    m_ui->m_optimizationAction->setEnabled(true);
    m_ui->m_signMessageAction->setEnabled(true);
    m_ui->m_verifySignedMessageAction->setEnabled(true);
    if(WalletAdapter::instance().isDeterministic()) {
       m_ui->m_showMnemonicSeedAction->setEnabled(true);
    }
    encryptedFlagChanged(Settings::instance().isEncrypted());

    QList<QAction*> tabActions = m_tabActionGroup->actions();
    Q_FOREACH(auto action, tabActions) {
      action->setEnabled(true);
    }

    setWindowTitle(QString(tr("%1 - Karbo Wallet %2")).arg(Settings::instance().getWalletFile()).arg(Settings::instance().getVersion()));

    m_ui->m_overviewAction->trigger();
    accountWidget->setVisible(true);
    m_ui->m_overviewFrame->show();
    m_ui->m_receiveFrame->closePaymentRequestForm();

    checkTrackingMode();
    updateRecentActionList();

    if (Settings::instance().isTrackingMode()) {
      isTrackingMode();
    }

    WalletAdapter::instance().autoBackup();

  } else {
    walletClosed();
  }
}

void MainWindow::walletClosed() {
  m_ui->m_backupWalletAction->setEnabled(false);
  m_ui->m_encryptWalletAction->setEnabled(false);
  m_ui->m_changePasswordAction->setEnabled(false);
  m_ui->m_closeWalletAction->setEnabled(false);
  m_ui->m_openUriAction->setEnabled(false);
  m_ui->m_exportTrackingKeyAction->setEnabled(false);
  m_ui->m_showPrivateKey->setEnabled(false);
  m_ui->m_resetAction->setEnabled(false);
  m_ui->m_showMnemonicSeedAction->setEnabled(false);
  m_ui->m_sweepUnmixableAction->setEnabled(false);
  m_ui->m_optimizationAction->setEnabled(false);
  m_ui->m_signMessageAction->setEnabled(false);
  m_ui->m_verifySignedMessageAction->setEnabled(false);
  m_ui->m_proofBalanceAction->setEnabled(false);
  m_ui->m_overviewFrame->hide();
  accountWidget->setVisible(false);
  m_ui->m_receiveFrame->hide();
  m_ui->m_sendFrame->hide();
  m_ui->m_transactionsFrame->hide();
  m_ui->m_addressBookFrame->hide();
  m_ui->m_noWalletFrame->show();
  m_encryptionStateIconLabel->hide();
  m_trackingModeIconLabel->hide();
  m_synchronizationStateIconLabel->hide();

  setWindowTitle(QString(tr("Karbo Wallet %2")).arg(Settings::instance().getVersion()));

  QList<QAction*> tabActions = m_tabActionGroup->actions();
  Q_FOREACH(auto action, tabActions) {
    action->setEnabled(false);
  }
  Settings::instance().setTrackingMode(false);
  updateRecentActionList();
}

void MainWindow::checkTrackingMode() {
  CryptoNote::AccountKeys keys;
  WalletAdapter::instance().getAccountKeys(keys);
  if (keys.spendSecretKey == boost::value_initialized<Crypto::SecretKey>()) {
    Settings::instance().setTrackingMode(true);
  } else {
    Settings::instance().setTrackingMode(false);
  }
}

void MainWindow::updateUnmixableBalance(quint64 _balance) {
  if (_balance != 0) {
      m_ui->m_sweepUnmixableAction->setEnabled(true);
  } else {
      m_ui->m_sweepUnmixableAction->setEnabled(false);
  }
}

void MainWindow::createTrayIcon()
{
#ifdef Q_OS_WIN
    m_trayIcon = new QSystemTrayIcon(QPixmap(":images/cryptonote"), this);
    QString toolTip = QString(tr("Karbo Wallet %1")).arg(Settings::instance().getVersion());
    m_trayIcon->setToolTip(toolTip);
    m_trayIcon->show();
#endif
}

void MainWindow::createTrayIconMenu()
{
#ifndef Q_OS_MAC
    // return if trayIcon is unset (only on non-Mac OSes)
    if (!m_trayIcon)
        return;

    trayIconMenu = new QMenu(this);
    m_trayIcon->setContextMenu(trayIconMenu);

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
#endif
#ifdef Q_OS_MAC
    // Note: On Mac, the dock icon is used to provide the tray's functionality.
    MacDockIconHandler *dockIconHandler = MacDockIconHandler::instance();
    dockIconHandler->setMainWindow((QMainWindow *)this);
    trayIconMenu = dockIconHandler->dockMenu();
#endif

    // Configuration of the tray icon (or dock icon) icon menu

#ifndef Q_OS_MAC // This is built-in on Mac
    trayIconMenu->addAction(toggleHideAction);
    trayIconMenu->addSeparator();
#endif
    trayIconMenu->addAction(m_ui->m_sendAction);
    trayIconMenu->addAction(m_ui->m_receiveAction);
    trayIconMenu->addAction(m_ui->m_transactionsAction);
    trayIconMenu->addAction(m_ui->m_addressBookAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(m_ui->m_openWalletAction);
    trayIconMenu->addAction(m_ui->m_closeWalletAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(m_ui->actionHelp);
#ifndef Q_OS_MAC // This is built-in on Mac
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(m_ui->m_exitAction);
#endif
}

void MainWindow::payTo(const QModelIndex& _index) {
  m_ui->m_sendFrame->setAddress(_index.data(AddressBookModel::ROLE_ADDRESS).toString());
  m_ui->m_sendAction->trigger();
}

#ifdef Q_OS_WIN
void MainWindow::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
    {
        // Click on system tray icon triggers show/hide of the main window
        toggleHidden();
    }
}
#endif

#ifdef Q_OS_WIN
void MainWindow::minimizeToTray(bool _on) {
  if (_on) {
    hide();
  } else {
    showNormal();
    activateWindow();
  }
}
#endif

void MainWindow::showNormalIfMinimized(bool fToggleHidden)
{
    if (isHidden() || isMinimized())
    {
        showNormal();
        activateWindow();
    }
    else if (isObscured(this))
    {
        raise();
        activateWindow();
    }
    else if (fToggleHidden) {
        hide();
    }
    else {
        showNormal();
        activateWindow();
    }
}

void MainWindow::toggleHidden()
{
    showNormalIfMinimized(true);
}

bool MainWindow::checkPoint(const QPoint &p, const QWidget *w)
{
    QWidget *atW = QApplication::widgetAt(w->mapToGlobal(p));
    if (!atW) return false;
    return atW->topLevelWidget() == w;
}

bool MainWindow::isObscured(QWidget *w)
{
    return !(checkPoint(QPoint(0, 0), w)
        && checkPoint(QPoint(w->width() - 1, 0), w)
        && checkPoint(QPoint(0, w->height() - 1), w)
        && checkPoint(QPoint(w->width() - 1, w->height() - 1), w)
        && checkPoint(QPoint(w->width() / 2, w->height() / 2), w));
}

}
