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

#include <cstring>

#include <QGuiApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QDataWidgetMapper>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaMethod>
#include <QMovie>
#include <QSessionManager>
#include <QSystemTrayIcon>
#include <QUrlQuery>

#include "MainWindow.h"
#include "Settings/Settings.h"
#include "WalletLogger/WalletLogger.h"
#include "Gui/Common/AboutDialog.h"
#include "Gui/Common/ChangePasswordDialog.h"
#include "Gui/Common/NewPasswordDialog.h"
#include "Gui/Common/KeyDialog.h"
#include "Gui/Common/QuestionDialog.h"
#include "ICryptoNoteAdapter.h"
#include "INodeAdapter.h"
#include "Models/AddressBookModel.h"
#include "Models/BlockchainModel.h"
#include "Models/FusionTransactionsFilterModel.h"
#include "Models/MinerModel.h"
#include "Models/NodeStateModel.h"
#include "Models/SortedAddressBookModel.h"
#include "Models/SortedTransactionsModel.h"
#include "Models/TransactionsModel.h"
#include "Models/TransactionPoolModel.h"
#include "Models/WalletStateModel.h"
#include "Gui/Options/OptionsDialog.h"
#include "Style/Style.h"

#include "ui_MainWindow.h"

namespace WalletGui {

namespace {

const int MAX_RECENT_WALLET_COUNT = 10;
const char COMMUNITY_FORUM_URL[] = "https://intense-coin.herokuapp.com";
const char REPORT_ISSUE_URL[] = "https://intensecoin.com/#contact";

const char DONATION_URL_DONATION_TAG[] = "donation";
const char DONATION_URL_LABEL_TAG[] = "label";

QByteArray convertAccountKeysToByteArray(const AccountKeys& _accountKeys) {
  QByteArray spendPublicKey(reinterpret_cast<const char*>(&_accountKeys.spendKeys.publicKey), sizeof(Crypto::PublicKey));
  QByteArray viewPublicKey(reinterpret_cast<const char*>(&_accountKeys.viewKeys.publicKey), sizeof(Crypto::PublicKey));
  QByteArray spendPrivateKey(reinterpret_cast<const char*>(&_accountKeys.spendKeys.secretKey), sizeof(Crypto::SecretKey));
  QByteArray viewPrivateKey(reinterpret_cast<const char*>(&_accountKeys.viewKeys.secretKey), sizeof(Crypto::SecretKey));
  QByteArray trackingKeys;
  trackingKeys.append(spendPublicKey).append(viewPublicKey).append(spendPrivateKey).append(viewPrivateKey);
  return trackingKeys;
}

QByteArray convertAccountPrivateKeysToByteArray(const AccountKeys& _accountKeys) {
	QByteArray spendPrivateKey(reinterpret_cast<const char*>(&_accountKeys.spendKeys.secretKey), sizeof(Crypto::SecretKey));
	QByteArray viewPrivateKey(reinterpret_cast<const char*>(&_accountKeys.viewKeys.secretKey), sizeof(Crypto::SecretKey));
	QByteArray trackingKeys;
	trackingKeys.append(spendPrivateKey).append(viewPrivateKey);
	return trackingKeys;
}

AccountKeys convertByteArrayToAccountKeys(const QByteArray& _array) {
  AccountKeys accountKeys;
  QDataStream trackingKeysDataStream(_array);
  trackingKeysDataStream.readRawData(reinterpret_cast<char*>(&accountKeys.spendKeys.publicKey), sizeof(Crypto::PublicKey));
  trackingKeysDataStream.readRawData(reinterpret_cast<char*>(&accountKeys.viewKeys.publicKey), sizeof(Crypto::PublicKey));
  trackingKeysDataStream.readRawData(reinterpret_cast<char*>(&accountKeys.spendKeys.secretKey), sizeof(Crypto::SecretKey));
  trackingKeysDataStream.readRawData(reinterpret_cast<char*>(&accountKeys.viewKeys.secretKey), sizeof(Crypto::SecretKey));
  return accountKeys;
}

bool isDonationUrl(const QUrl& _url) {
  QUrlQuery urlQuery(_url);
  if(!urlQuery.hasQueryItem(DONATION_URL_DONATION_TAG)) {
    return false;
  }

  return (urlQuery.queryItemValue(DONATION_URL_DONATION_TAG).compare("true", Qt::CaseInsensitive) == 0);
}

}

MainWindow::MainWindow(ICryptoNoteAdapter* _cryptoNoteAdapter, IAddressBookManager* _addressBookManager,
  IDonationManager* _donationManager, IOptimizationManager* _optimizationManager, IMiningManager* _miningManager,
  IApplicationEventHandler* _applicationEventHandler, INewsReader* _blogReader, const QString& _styleSheetTemplate, QWidget* _parent) :
  QMainWindow(_parent), m_ui(new Ui::MainWindow), m_cryptoNoteAdapter(_cryptoNoteAdapter),
  m_addressBookManager(_addressBookManager), m_donationManager(_donationManager),
  m_optimizationManager(_optimizationManager), m_miningManager(_miningManager), m_applicationEventHandler(_applicationEventHandler),
  m_blogReader(_blogReader), m_blockChainModel(nullptr), m_transactionPoolModel(nullptr), m_recentWalletsMenu(new QMenu(this)),
  m_addRecipientAction(new QAction(this)), m_styleSheetTemplate(_styleSheetTemplate), m_walletStateMapper(new QDataWidgetMapper(this)),
  m_syncMovie(new QMovie(Settings::instance().getCurrentStyle().getWalletSyncGifFile(), QByteArray(), this)) {
  m_ui->setupUi(this);
  setWindowTitle(tr("Intensecoin Wallet %1").arg(Settings::instance().getVersion()));
  m_addRecipientAction->setObjectName("m_addRecipientAction");
  m_cryptoNoteAdapter->addObserver(this);
  m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
  m_applicationEventHandler->addObserver(this);
  addActions(QList<QAction*>() << m_ui->m_createWalletAction << m_ui->m_openWalletAction);

  m_nodeStateModel = new NodeStateModel(m_cryptoNoteAdapter, this);
  m_walletStateModel = new WalletStateModel(m_cryptoNoteAdapter, this);
  m_transactionsModel = new TransactionsModel(m_cryptoNoteAdapter, m_optimizationManager, m_nodeStateModel, this);
  m_sortedTranactionsModel = new SortedTransactionsModel(m_transactionsModel, this);
  m_fusionTranactionsFilterModel = new FusionTransactionsFilterModel(m_sortedTranactionsModel, m_optimizationManager, this);
  m_addressBookModel = new AddressBookModel(m_addressBookManager, this);
  m_sortedAddressBookModel = new SortedAddressBookModel(m_addressBookModel, this);
  m_blockChainModel = new BlockchainModel(m_cryptoNoteAdapter, m_nodeStateModel, this);
  m_transactionPoolModel = new TransactionPoolModel(m_cryptoNoteAdapter, this);
  m_minerModel = new MinerModel(m_miningManager, this);

  QList<IWalletUiItem*> uiItems;
  uiItems << m_ui->m_noWalletFrame << m_ui->m_overviewFrame << m_ui->m_sendFrame << m_ui->m_transactionsFrame <<
    m_ui->m_blockExplorerFrame << m_ui->m_addressBookFrame << m_ui->m_miningFrame << m_ui->statusBar;
  for (auto& uiItem : uiItems) {
    uiItem->setCryptoNoteAdapter(m_cryptoNoteAdapter);
    uiItem->setAddressBookManager(m_addressBookManager);
    uiItem->setDonationManager(m_donationManager);
    uiItem->setMiningManager(m_miningManager);
    uiItem->setApplicationEventHandler(m_applicationEventHandler);
    uiItem->setBlogReader(m_blogReader);
    uiItem->setMainWindow(this);
    uiItem->setNodeStateModel(m_nodeStateModel);
    uiItem->setWalletStateModel(m_walletStateModel);
    uiItem->setTransactionsModel(m_transactionsModel);
    uiItem->setSortedTransactionsModel(m_fusionTranactionsFilterModel);
    uiItem->setAddressBookModel(m_addressBookModel);
    uiItem->setSortedAddressBookModel(m_sortedAddressBookModel);
    uiItem->setBlockChainModel(m_blockChainModel);
    uiItem->setTransactionPoolModel(m_transactionPoolModel);
    uiItem->setMinerModel(m_minerModel);
  }

  if (!Settings::instance().isSystemTrayAvailable() && QSystemTrayIcon::isSystemTrayAvailable()) {
    m_ui->m_minimizeToTrayAction->deleteLater();
    m_ui->m_closeToTrayAction->deleteLater();
  } else {
    m_ui->m_minimizeToTrayAction->setChecked(Settings::instance().isMinimizeToTrayEnabled());
    m_ui->m_closeToTrayAction->setChecked(Settings::instance().isCloseToTrayEnabled());
  }

  createRecentWalletMenu();
  m_ui->m_addressLabel->installEventFilter(this);
  m_ui->m_balanceLabel->installEventFilter(this);
  m_walletStateMapper->setModel(m_walletStateModel);
  m_walletStateMapper->addMapping(m_ui->m_balanceFrame, WalletStateModel::COLUMN_IS_OPEN, "visible");
  m_walletStateMapper->addMapping(m_ui->m_walletFrame, WalletStateModel::COLUMN_IS_OPEN, "visible");
  m_walletStateMapper->addMapping(m_ui->m_noWalletLabel, WalletStateModel::COLUMN_IS_CLOSED, "visible");
  m_walletStateMapper->addMapping(m_ui->m_notEncryptedFrame, WalletStateModel::COLUMN_IS_NOT_ENCRYPTED, "visible");
  m_walletStateMapper->addMapping(m_ui->m_addressLabel, WalletStateModel::COLUMN_ADDRESS, "text");
  m_walletStateMapper->addMapping(m_ui->m_balanceLabel, WalletStateModel::COLUMN_TOTAL_BALANCE, "text");
  m_walletStateMapper->setCurrentIndex(0);

  setClosedState();
  if (QFile::exists(Settings::instance().getWalletFile())) {
    m_ui->m_noWalletFrame->openWallet(Settings::instance().getWalletFile(), QString());
  } else {
    m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->create(Settings::instance().getWalletFile(), "");
  }

  m_ui->m_balanceIconLabel->setPixmap(Settings::instance().getCurrentStyle().getBalanceIcon());
  m_ui->m_logoLabel->setPixmap(Settings::instance().getCurrentStyle().getLogoPixmap());
  QActionGroup* themeActionGroup = new QActionGroup(this);
  quintptr styleCount = Settings::instance().getStyleCount();
  for (quintptr i = 0; i < styleCount; ++i) {
    const Style& style = Settings::instance().getStyle(i);
    QAction* styleAction = m_ui->menuThemes->addAction(style.getStyleName());
    styleAction->setData(style.getStyleId());
    styleAction->setCheckable(true);
    if (style.getStyleId() == Settings::instance().getCurrentTheme()) {
      styleAction->setChecked(true);
    }

    themeActionGroup->addAction(styleAction);
    connect(styleAction, &QAction::triggered, this, &MainWindow::themeChanged);
  }

  connect(m_walletStateModel, &QAbstractItemModel::dataChanged, this, &MainWindow::walletStateModelDataChanged);
  connect(m_addRecipientAction, &QAction::triggered, this, &MainWindow::addRecipientTriggered);
  connect(m_ui->m_exitAction, &QAction::triggered, qApp, &QApplication::quit);
  connect(qApp, &QGuiApplication::commitDataRequest, this, &MainWindow::commitData);
}

MainWindow::~MainWindow() {
}

bool MainWindow::eventFilter(QObject* _object, QEvent* _event) {
  if (_object == m_ui->m_addressLabel && _event->type() == QEvent::MouseButtonRelease) {
    copyAddress();
  } else if (_object == m_ui->m_balanceLabel && _event->type() == QEvent::MouseButtonRelease) {
    copyBalance();
  }

  return false;
}

void MainWindow::walletOpened() {
  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  setOpenedState();
  QStringList recentWalletList = Settings::instance().getRecentWalletList();
  recentWalletList.removeAll(Settings::instance().getWalletFile());
  recentWalletList.prepend(Settings::instance().getWalletFile());
  while (recentWalletList.size() > MAX_RECENT_WALLET_COUNT) {
    recentWalletList.removeLast();
  }

  Settings::instance().setRecentWalletList(recentWalletList);
  updateRecentWalletActions();
  if (walletAdapter->isTrackingWallet()) {
    m_ui->m_sendButton->setEnabled(false);
    m_ui->m_addressBookButton->setEnabled(false);
  }

  QUrl url = m_applicationEventHandler->getLastReceivedUrl();
  if (url.isValid()) {
    urlReceived(url);
  }
}

void MainWindow::walletOpenError(int _initStatus) {
  if (_initStatus != IWalletAdapter::INIT_SUCCESS) {
    setClosedState();
  }
}

void MainWindow::walletClosed() {
  setClosedState();
}

void MainWindow::passwordChanged() {
  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  if (walletAdapter->isEncrypted()) {
    m_ui->m_notEncryptedFrame->hide();
  }

  m_ui->m_changePasswordAction->setEnabled(walletAdapter->isEncrypted());
  m_ui->m_encryptWalletAction->setEnabled(!walletAdapter->isEncrypted());
  walletAdapter->save(CryptoNote::WalletSaveLevel::SAVE_ALL, true);
}

void MainWindow::synchronizationProgressUpdated(quint32 _current, quint32 _total) {
  if (_total < _current) {
    return;
  }

  if (m_ui->m_removePendingTxAction->isEnabled())
	  m_ui->m_removePendingTxAction->setEnabled(false);

  qreal value = static_cast<qreal>(_current) / _total;
  m_ui->m_syncProgress->setValue(value * m_ui->m_syncProgress->maximum());
}

void MainWindow::synchronizationCompleted() {
  m_ui->m_syncProgress->setValue(m_ui->m_syncProgress->maximum());
  m_ui->m_removePendingTxAction->setEnabled(true);
}

void MainWindow::balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) {
  // Do nothing
}

void MainWindow::externalTransactionCreated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void MainWindow::transactionUpdated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void MainWindow::urlReceived(const QUrl& _url) {
  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  if (!walletAdapter->isOpen()) {
    return;
  }

  showNormal();
  activateWindow();
  raise();
  if (isDonationUrl(_url)) {
    QUrlQuery urlQuery(_url);
    QString address = _url.path();
    QString label = urlQuery.queryItemValue(DONATION_URL_LABEL_TAG);
    m_addressBookManager->addAddress(label, address, true);

    OptionsDialog dlg(m_cryptoNoteAdapter, m_donationManager, m_optimizationManager, m_addressBookModel, this);
    dlg.setDonationAddress(label, address);
    dlg.exec();
  } else if (_url.isValid()) {
    m_ui->m_sendButton->click();
  }
}

void MainWindow::screenLocked() {
  // Do nothing
}

void MainWindow::screenUnlocked() {
  // Do nothing
}

void MainWindow::cryptoNoteAdapterInitCompleted(int _status) {
  setEnabled(true);
  if (_status == 0) {
    m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
    if (QFile::exists(Settings::instance().getWalletFile())) {
      m_ui->m_noWalletFrame->openWallet(Settings::instance().getWalletFile(), QString());
    }
  }
}

void MainWindow::cryptoNoteAdapterDeinitCompleted() {

}

void MainWindow::changeEvent(QEvent* _event) {
  QMainWindow::changeEvent(_event);
  switch (_event->type()) {
  case QEvent::WindowStateChange:
    if(isMinimized() && Settings::instance().isMinimizeToTrayEnabled()) {
      hide();
    }

    break;
  default:
    break;
  }

  QMainWindow::changeEvent(_event);
}

void MainWindow::closeEvent(QCloseEvent* _event) {
#ifndef Q_OS_MAC
  if (!Settings::instance().isCloseToTrayEnabled()) {
    m_ui->m_exitAction->trigger();
  }
#endif
  QMainWindow::closeEvent(_event);
}

void MainWindow::setOpenedState() {

  QList<QAbstractButton*> toolButtons = m_ui->m_toolButtonGroup->buttons();
  for (const auto& button : toolButtons) {
    button->setChecked(false);
    button->setEnabled(true);
  }

  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  m_ui->m_backupWalletAction->setEnabled(true);
  m_ui->m_resetAction->setEnabled(true);
  m_ui->m_exportTrackingKeyAction->setEnabled(true);
  m_ui->m_encryptWalletAction->setEnabled(!walletAdapter->isEncrypted());
  m_ui->m_changePasswordAction->setEnabled(walletAdapter->isEncrypted());

  m_ui->m_noWalletFrame->hide();
  m_ui->m_overviewFrame->show();

  m_ui->m_overviewButton->setChecked(true);
  m_ui->m_blockExplorerButton->setEnabled(m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter() != nullptr);
}

void MainWindow::setClosedState() {
  QList<QAbstractButton*> toolButtons = m_ui->m_toolButtonGroup->buttons();
  for (const auto& button : toolButtons) {
    button->setChecked(false);
    button->setEnabled(false);
  }

  m_ui->m_backupWalletAction->setEnabled(false);
  m_ui->m_resetAction->setEnabled(false);
  m_ui->m_exportTrackingKeyAction->setEnabled(false);
  m_ui->m_encryptWalletAction->setEnabled(false);
  m_ui->m_changePasswordAction->setEnabled(false);
  m_ui->m_removePendingTxAction->setEnabled(false);

  m_ui->m_overviewFrame->hide();
  m_ui->m_sendFrame->hide();
  m_ui->m_transactionsFrame->hide();
  m_ui->m_addressBookFrame->hide();
  m_ui->m_blockExplorerFrame->hide();
  m_ui->m_miningFrame->hide();
  m_ui->m_noWalletFrame->show();
  m_ui->m_syncProgress->setValue(0);
}

void MainWindow::addRecipientTriggered() {
  m_ui->m_sendFrame->addRecipient(m_addRecipientAction->data().toString());
  m_ui->m_sendButton->click();
}

void MainWindow::commitData(QSessionManager& _manager) {
  WalletLogger::debug(tr("[Main window] Commit data request"));
  if (m_cryptoNoteAdapter->getNodeAdapter() != nullptr) {
    IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
    if (walletAdapter->isOpen()) {
      walletAdapter->save(CryptoNote::WalletSaveLevel::SAVE_ALL, true);
    }
  }
}

void MainWindow::walletStateModelDataChanged(const QModelIndex& _topLeft, const QModelIndex& _bottomRight, const QVector<int>& _roles) {
  if (_topLeft.column() == WalletStateModel::COLUMN_ABOUT_TO_BE_SYNCHRONIZED) {
    bool walletAboutToBeSynchronized = _topLeft.data().toBool();
    if (!walletAboutToBeSynchronized) {
      m_walletStateMapper->removeMapping(m_ui->m_balanceLabel);
      m_ui->m_balanceLabel->setMovie(m_syncMovie);
      m_syncMovie->start();
      m_ui->m_balanceLabel->setCursor(Qt::ArrowCursor);
      m_ui->m_balanceLabel->removeEventFilter(this);
      m_ui->m_balanceLabel->setToolTip(QString());
  } else {
      m_syncMovie->stop();
      m_ui->m_balanceLabel->setMovie(nullptr);
      m_walletStateMapper->addMapping(m_ui->m_balanceLabel, WalletStateModel::COLUMN_TOTAL_BALANCE, "text");
      m_walletStateMapper->revert();
      m_ui->m_balanceLabel->setCursor(Qt::PointingHandCursor);
      m_ui->m_balanceLabel->installEventFilter(this);
      m_ui->m_balanceLabel->setToolTip(tr("Click to copy"));
    }
  }
}

void MainWindow::createRecentWalletMenu() {
  m_ui->m_recentWalletsAction->setMenu(m_recentWalletsMenu);
  for (quint32 i = 0; i < MAX_RECENT_WALLET_COUNT; ++i) {
    QAction* action = new QAction(this);
    action->setVisible(false);
    m_recentWalletsActionList.append(action);
    m_recentWalletsMenu->addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::openRecentWallet);
  }

  updateRecentWalletActions();
}

void MainWindow::updateRecentWalletActions() {
  QStringList recentWallets = Settings::instance().getRecentWalletList();
  int recentWalletCount = qMin(recentWallets.size(), MAX_RECENT_WALLET_COUNT);
  for (int i = 0; i < recentWalletCount; ++i) {
    m_recentWalletsActionList[i]->setText(recentWallets[i]);
    m_recentWalletsActionList[i]->setData(recentWallets[i]);
    m_recentWalletsActionList[i]->setVisible(true);
  }

  for (int i = recentWalletCount; i < MAX_RECENT_WALLET_COUNT; ++i) {
    m_recentWalletsActionList[i]->setVisible(false);
  }
}

void MainWindow::openRecentWallet() {
  QAction* action = qobject_cast<QAction*>(sender());
  if (action == nullptr) {
    return;
  }

  QString filePath = action->data().toString();
  if (!filePath.isEmpty()) {
    IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
    if (walletAdapter->isOpen()) {
      walletAdapter->save(CryptoNote::WalletSaveLevel::SAVE_ALL, true);
      walletAdapter->removeObserver(this);
      walletAdapter->close();
      walletAdapter->addObserver(this);
    }

    m_ui->m_noWalletFrame->openWallet(filePath, QString());
  }
}

void MainWindow::themeChanged() {
  QAction* styleAction = qobject_cast<QAction*>(sender());
  Settings::instance().setCurrentTheme(styleAction->data().toString());
  qApp->setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(m_styleSheetTemplate));
  m_ui->m_balanceIconLabel->setPixmap(Settings::instance().getCurrentStyle().getBalanceIcon());
  m_ui->m_logoLabel->setPixmap(Settings::instance().getCurrentStyle().getLogoPixmap());
  m_syncMovie->stop();
  m_syncMovie->setFileName(Settings::instance().getCurrentStyle().getWalletSyncGifFile());
  m_syncMovie->start();
  QList<IWalletUiItem*> uiItems;
  uiItems << m_ui->m_noWalletFrame << m_ui->m_overviewFrame << m_ui->m_sendFrame << m_ui->m_transactionsFrame <<
    m_ui->m_blockExplorerFrame << m_ui->m_addressBookFrame << m_ui->m_miningFrame << m_ui->statusBar;
  for (auto& uiItem : uiItems) {
    uiItem->updateStyle();
  }
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

  if (QFile::exists(filePath)) {
    QMessageBox::warning(this, tr("Warning"),
      tr("Can't overwrite existing %1 because it may lead to loss of private keys").arg(QFileInfo(filePath).fileName()));
    return;
  }

  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  if (!filePath.isEmpty()) {
    if (walletAdapter->isOpen()) {
      walletAdapter->save(CryptoNote::WalletSaveLevel::SAVE_ALL, true);
      walletAdapter->removeObserver(this);
      walletAdapter->close();
      walletAdapter->addObserver(this);
    }

    QString oldWalletFile = Settings::instance().getWalletFile();
    Settings::instance().setWalletFile(filePath);
    if (walletAdapter->create(filePath, "") == IWalletAdapter::INIT_SUCCESS) {
      walletAdapter->save(CryptoNote::WalletSaveLevel::SAVE_ALL, true);
    } else {
      Settings::instance().setWalletFile(oldWalletFile);
    }
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

  QString walletFilePath = filePath;
  if (!filePath.isEmpty()) {
    if (filePath.endsWith(".keys")) {
      walletFilePath.replace(filePath.lastIndexOf(".keys"), 5, ".wallet");
      if (QFile::exists(walletFilePath)) {
        QMessageBox::warning(this, tr("Warning"),
          tr("Can't overwrite existing %1 because it may lead to loss of private keys").arg(QFileInfo(walletFilePath).fileName()));
        return;
      }
    } else {
      filePath.clear();
    }
  } else {
    return;
  }

  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  if (walletAdapter->isOpen()) {
    walletAdapter->save(CryptoNote::WalletSaveLevel::SAVE_ALL, true);
    walletAdapter->removeObserver(this);
    walletAdapter->close();
    walletAdapter->addObserver(this);
  }

  m_ui->m_noWalletFrame->openWallet(walletFilePath, filePath);
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

  if (QFile::exists(filePath)) {
    QMessageBox::warning(this, tr("Warning"),
      tr("Can't overwrite existing %1 because it may lead to loss of private keys").arg(QFileInfo(filePath).fileName()));
    return;
  }

  if (!filePath.isEmpty()) {
    IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
    walletAdapter->exportWallet(filePath, true, CryptoNote::WalletSaveLevel::SAVE_KEYS_AND_TRANSACTIONS, false);
  }
}

void MainWindow::saveWalletKeys() {
  QString filePath = QFileDialog::getSaveFileName(this, tr("Save keys to..."),
#ifdef Q_OS_WIN
    QApplication::applicationDirPath(),
#else
    QDir::homePath(),
#endif
    tr("Wallet (*.wallet)")
    );
  if (!filePath.isEmpty() && !filePath.endsWith(".wallet")) {
    filePath.append(".wallet");
  }

  if (QFile::exists(filePath)) {
    QMessageBox::warning(this, tr("Warning"),
      tr("Can't overwrite existing %1 because it may lead to loss of private keys").arg(QFileInfo(filePath).fileName()));
    return;
  }

  if (!filePath.isEmpty()) {
    IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
    walletAdapter->exportWallet(filePath, false, CryptoNote::WalletSaveLevel::SAVE_KEYS_ONLY, false);
  }
}

void MainWindow::resetWallet() {
  QuestionDialog dlg(tr("Reset wallet?"), tr("Reset wallet to resynchronize its transactions and balance based\n"
    "on the blockchain data. This operation can take some time.\n"
    "Are you sure you would like to reset this wallet?"), this);
  if (dlg.exec() != QDialog::Accepted) {
    return;
  }

  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  Q_ASSERT(walletAdapter->isOpen());
  QString fileName = Settings::instance().getWalletFile();
  QDateTime currenctDateTime = QDateTime::currentDateTime();
  fileName.append(QString(".%1.backup").arg(currenctDateTime.toString("yyyyMMddHHMMss")));

  walletAdapter->save(CryptoNote::WalletSaveLevel::SAVE_KEYS_ONLY, true);
  walletAdapter->removeObserver(this);
  walletAdapter->close();
  walletAdapter->addObserver(this);
  m_ui->m_noWalletFrame->openWallet(Settings::instance().getWalletFile(), QString());
}

void MainWindow::removePendingTx() {
	IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
	if (walletAdapter->resetPendingTransactions())
		QMessageBox::information(this, "Transactions reset", "Transactions reset successfully.", QMessageBox::Ok);
	else
		QMessageBox::warning(this, "Transaction reset failed", "Failed to reset transactions. Check debug log.", QMessageBox::Ok);
}

void MainWindow::encryptWallet() {
  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  if (walletAdapter->isEncrypted()) {
    IWalletAdapter::PasswordStatus status = IWalletAdapter::PASSWORD_SUCCESS;
    do {
      ChangePasswordDialog dlg(status == IWalletAdapter::PASSWORD_ERROR, this);
      if (dlg.exec() == QDialog::Rejected) {
        return;
      }

      QString oldPassword = dlg.getOldPassword();
      QString newPassword = dlg.getNewPassword();
      status = walletAdapter->changePassword(oldPassword, newPassword);
    } while (status != IWalletAdapter::PASSWORD_SUCCESS);
  } else {
    NewPasswordDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
      QString password = dlg.getPassword();
      if (password.isEmpty()) {
        return;
      }

      walletAdapter->changePassword("", password);
    }
  }
}

void MainWindow::exportKey() {
	if (m_cryptoNoteAdapter->getNodeAdapter() == nullptr ||
		m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter() == nullptr ||
		!m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->isOpen())
		return;
  AccountKeys accountKeys = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->getAccountKeys(0);
  QByteArray keys = convertAccountKeysToByteArray(accountKeys);
  KeyDialog dlg(keys, false, this);
  dlg.exec();
}

void MainWindow::exportPrivateKeys() {
	if (m_cryptoNoteAdapter->getNodeAdapter() == nullptr ||
		m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter() == nullptr ||
		!m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->isOpen())
		return;
	AccountKeys accountKeys = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->getAccountKeys(0);
	QByteArray keys = convertAccountPrivateKeysToByteArray(accountKeys);
	KeyDialog dlg(keys, false, true, this);
	dlg.exec();
}

void MainWindow::exportTrackingKey() {
  AccountKeys accountKeys = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->getAccountKeys(0);
  std::memset(&accountKeys.spendKeys.secretKey, 0, sizeof(Crypto::SecretKey));
  QByteArray trackingKeys = convertAccountKeysToByteArray(accountKeys);
  KeyDialog dlg(trackingKeys, true, this);
  dlg.exec();
}

void MainWindow::importKey() {
  KeyDialog dlg(this);
  if (dlg.exec() == QDialog::Accepted) {
    QByteArray key = dlg.getKey();
    if (key.size() != sizeof(CryptoNote::AccountKeys)) {
      QMessageBox::warning(this, tr("Warning"), tr("Incorrect tracking key size"));
      return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, tr("Save tracking wallet to..."),
#ifdef Q_OS_WIN
    QApplication::applicationDirPath(),
#else
    QDir::homePath(),
#endif
    tr("Wallets (*.wallet)"));
    if (filePath.isEmpty()) {
      return;
    }

    if (!filePath.endsWith(".wallet")) {
      filePath.append(".wallet");
    }

    if (QFile::exists(filePath)) {
      QMessageBox::warning(this, tr("Warning"),
        tr("Can't overwrite existing %1 because it may lead to loss of private keys").arg(QFileInfo(filePath).fileName()));
      return;
    }

    IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
    if (walletAdapter->isOpen()) {
      walletAdapter->removeObserver(this);
      walletAdapter->close();
      walletAdapter->addObserver(this);
    }

    AccountKeys accountKeys = convertByteArrayToAccountKeys(key);
    QString oldWalletFile = Settings::instance().getWalletFile();
    Settings::instance().setWalletFile(filePath);
    if (walletAdapter->createWithKeys(filePath, accountKeys) == IWalletAdapter::INIT_SUCCESS) {
      walletAdapter->save(CryptoNote::WalletSaveLevel::SAVE_ALL, true);
    } else {
      Settings::instance().setWalletFile(oldWalletFile);
    }
  }
}

void MainWindow::aboutQt() {
  QMessageBox::aboutQt(this);
}

void MainWindow::about() {
  AboutDialog dlg(this);
  dlg.exec();
}

void MainWindow::copyAddress() {
  QApplication::clipboard()->setText(m_walletStateModel->index(0, WalletStateModel::COLUMN_ADDRESS).data().toString());
  m_ui->m_copyAddressLabel->start();
}

void MainWindow::copyBalance() {
  QString balanceString = m_walletStateModel->index(0, WalletStateModel::COLUMN_TOTAL_BALANCE).data().toString();
  balanceString.remove(',');
  QApplication::clipboard()->setText(balanceString);
  m_ui->m_balanceCopyLabel->start();
}

void MainWindow::setStartOnLoginEnabled(bool _enable) {
  Settings::instance().setStartOnLoginEnabled(_enable);
  m_ui->m_autostartAction->setChecked(Settings::instance().isStartOnLoginEnabled());
}

void MainWindow::setMinimizeToTrayEnabled(bool _enable) {
  Settings::instance().setMinimizeToTrayEnabled(_enable);
}

void MainWindow::setCloseToTrayEnabled(bool _enable) {
  Settings::instance().setCloseToTrayEnabled(_enable);
}

void MainWindow::showPreferences() {
  OptionsDialog dlg(m_cryptoNoteAdapter, m_donationManager, m_optimizationManager, m_addressBookModel, this);
  ConnectionMethod currentConnectionMethod = Settings::instance().getConnectionMethod();
  quint16 currentLocalRpcPort = Settings::instance().getLocalRpcPort();
  QUrl currentRemoteRpcUrl = Settings::instance().getRemoteRpcUrl();
  if (dlg.exec() == QDialog::Accepted) {
    ConnectionMethod newConnectionMethod = Settings::instance().getConnectionMethod();
    quint16 newLocalRpcPort = Settings::instance().getLocalRpcPort();
    QUrl newRemoteRpcUrl = Settings::instance().getRemoteRpcUrl();

    if(newConnectionMethod != currentConnectionMethod ||
      (newConnectionMethod == ConnectionMethod::LOCAL && newLocalRpcPort != currentLocalRpcPort) ||
      (newConnectionMethod == ConnectionMethod::REMOTE && newRemoteRpcUrl != currentRemoteRpcUrl)) {
      setEnabled(false);
      Q_EMIT reinitCryptoNoteAdapterSignal();
    }
  }
}

void MainWindow::communityForumTriggered() {
  QDesktopServices::openUrl(QUrl::fromUserInput(COMMUNITY_FORUM_URL));
}

void MainWindow::reportIssueTriggered() {
  QDesktopServices::openUrl(QUrl::fromUserInput(REPORT_ISSUE_URL));
}

}
