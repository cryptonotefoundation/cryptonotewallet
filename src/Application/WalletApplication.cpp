// Copyright (c) 2017-2018, Intense Coin developers
// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Bytecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Bytecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Bytecoin.  If not, see <http://www.gnu.org/licenses/>.

#include <QEvent>
#include <QFileOpenEvent>
#include <QFontDatabase>
#include <QImageReader>
#include <QLocale>
#include <QLockFile>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QStyleFactory>
#include <QTcpServer>

#ifdef Q_OS_WIN
#define NOMINMAX
#include <Windows.h>
#include <WtsApi32.h>
#endif

#include "WalletApplication.h"
#include "AddressBookManager.h"
#include "ApplicationEventHandler.h"
#include "BlogReader.h"
#include "CommandLineParser.h"
#include "Gui/Common/ExitWidget.h"
#include "Gui/Common/P2pBindPortErrorDialog.h"
#include "Gui/Common/QuestionDialog.h"
#include "Gui/Common/WalletOkButton.h"
#include "CryptoNoteWrapper/CryptoNoteAdapter.h"
#include "IBlockChainExplorerAdapter.h"
#include "LogFileWatcher.h"
#include "WalletLogger/WalletLogger.h"
#include "Gui/MainWindow/MainWindow.h"
#include "MiningManager.h"
#include "OptimizationManager.h"
#include "QJsonRpc/JsonRpcServer.h"
#include "Settings/Settings.h"
#include "SignalHandler.h"
#include "Style/Style.h"
#include "WalletSplashScreen.h"

namespace WalletGui {

namespace {

const char BYTECOIN_URI_SCHEME_NAME[] = "intensecoin";
const QRegularExpression LOG_SPLASH_REG_EXP("\\[Core\\] Imported block with index");

quint16 findPort() {
  QTcpServer srv;
  for (quint32 i = 0; i < 100; ++i) {
    if(srv.listen(QHostAddress::Any)) {
      return srv.serverPort();
    }
  }

  return 0;
}

bool rmDir(const QString& dirPath) {
  QDir dir(dirPath);
  if (!dir.exists())
    return true;

  QFileInfoList infoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
  if (infoList.isEmpty()) {
    return dir.rmdir(".");
  }

  for (const QFileInfo& info : infoList) {
    if (info.isDir()) {
      if (!rmDir(info.absoluteFilePath())) {
        return false;
      }
    } else if (info.isFile()) {
      if (!QFile::remove(info.absoluteFilePath())) {
        return false;
      }
    }
  }

  return true;
}

}

WalletApplication::WalletApplication(int& _argc, char** _argv) : QApplication(_argc, _argv), m_lockFile(nullptr),
  m_systemTrayIcon(new QSystemTrayIcon(this)), m_applicationEventHandler(new ApplicationEventHandler(this)),
  m_optimizationManager(nullptr), m_blogReader(new BlogReader(this)), m_mainWindow(nullptr), m_splash(nullptr),
  m_logWatcher(nullptr), m_isAboutToQuit(false) {
  setApplicationName("intensecoinwallet");
  setApplicationVersion(Settings::instance().getVersion());
  setQuitOnLastWindowClosed(false);
  setStyle(QStyleFactory::create("fusion"));
  QLocale::setDefault(QLocale::c());
  loadFonts();
}

WalletApplication::~WalletApplication() {
  if (m_lockFile != nullptr) {
    delete m_lockFile;
  }
}

bool WalletApplication::init() {
  qRegisterMetaType<quintptr>("quintptr");
  setupTheme();
  m_commandLineParser = new CommandLineParser(this);
  Settings::instance().setCommandLineParser(m_commandLineParser);
  bool cmdLineParseResult = m_commandLineParser->process(arguments());
  Settings::instance().init();

#ifdef Q_OS_WIN
  if(!cmdLineParseResult) {
    QMessageBox::critical(nullptr, QObject::tr("Error"), m_commandLineParser->getErrorText());
    return false;
  } else if (m_commandLineParser->hasHelpOption()) {
    QMessageBox::information(nullptr, QObject::tr("Help"), m_commandLineParser->getHelpText());
    return false;
  }
#endif

  Settings::instance().addObserver(this);
  makeDataDir();
  WalletLogger::init(Settings::instance().getDataDir(), Settings::instance().hasDebugOption(), this);
  WalletLogger::info(tr("[Application] Initializing..."));
  m_lockFile = new QLockFile(Settings::instance().getDataDir().absoluteFilePath("intensecoinwallet.lock"));
  QUrl paymentUrl = QUrl::fromUserInput(arguments().last());
  if (paymentUrl.scheme() != BYTECOIN_URI_SCHEME_NAME) {
    paymentUrl = QUrl();
  }

#ifndef Q_OS_MAC
  if (paymentUrl.isValid()) {
    m_applicationEventHandler->sendUrl(paymentUrl);
  }
#endif
  if (!m_lockFile->tryLock()) {
    WalletLogger::warning(tr("[Application] Intensecoin wallet already running"));
    if (!paymentUrl.isValid()) {
      QMessageBox::warning(nullptr, QObject::tr("Fail"), "Intensecoin wallet already running");
    }

    return false;
  }

  m_applicationEventHandler->init();
  if (Settings::instance().isNewsEnabled()) {
    m_blogReader->init();
  }

  SignalHandler::instance().init();
  QObject::connect(&SignalHandler::instance(), &SignalHandler::quitSignal, this, &WalletApplication::quit);
  if (!Settings::instance().isRunMinimizedEnabled()) {
    m_splash = new WalletSplashScreen(nullptr);
    initSystemTrayIcon();
  }

  m_cryptoNoteAdapter = new CryptoNoteAdapter(Settings::instance().getDataDir(), Settings::instance().isTestnet(),
    Settings::instance().hasDebugOption(), this);
  if(initCryptoNoteAdapter()) {
    WalletLogger::info(tr("[Application] Initialized successfully"));
    initUi();
    connect(this, &WalletApplication::aboutToQuit, this, &WalletApplication::prepareToQuit);
    return true;
  }

  WalletLogger::info(tr("[Application] Not initialized"));
  return false;
}

void WalletApplication::dockClickHandler() {
  if (m_isAboutToQuit) {
    return;
  }

  if (m_splash != nullptr && !m_splash->isVisible()) {
    m_splash->show();
  } else if (m_mainWindow != nullptr) {
    m_mainWindow->show();
  }
}

void WalletApplication::settingsUpdated() {
  if (Settings::instance().isNewsEnabled()) {
    m_blogReader->init();
  } else {
    m_blogReader->deinit();
  }
}

void WalletApplication::loadFonts() {
  QFontDatabase::addApplicationFont(":font/Bold");
  QFontDatabase::addApplicationFont(":font/BoldItalic");
  QFontDatabase::addApplicationFont(":font/ExtraBold");
  QFontDatabase::addApplicationFont(":font/ExtraBoldItalic");
  QFontDatabase::addApplicationFont(":font/Italic");
  QFontDatabase::addApplicationFont(":font/Light");
  QFontDatabase::addApplicationFont(":font/LightItalic");
  QFontDatabase::addApplicationFont(":font/Regular");
  QFontDatabase::addApplicationFont(":font/Semibold");
  QFontDatabase::addApplicationFont(":font/SemiboldItalic");
  QFont font;
  font.setFamily("Open Sans");
  font.setStyleStrategy(QFont::PreferAntialias);
  QApplication::setFont(font);
}

void WalletApplication::makeDataDir() {
  QString dataDirPath = Settings::instance().getDataDir().absolutePath();
  if (!QDir().exists(dataDirPath)) {
    QDir().mkpath(dataDirPath);
  }
}

void WalletApplication::setupTheme() {
  QFont font;
  font.setFamily("Open Sans");
  font.setStyleStrategy(QFont::PreferAntialias);
  QApplication::setFont(font);
}

bool WalletApplication::initCryptoNoteAdapter() {
  for (;;) {
    if (m_splash != nullptr) {
      m_splash->show();
      m_splash->showMessage(QObject::tr("Loading blockchain..."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
      if (m_logWatcher == nullptr) {
        m_logWatcher = new LogFileWatcher(Settings::instance().getDataDir().absoluteFilePath(CORE_LOG_FILE_NAME), this);
        connect(m_logWatcher, &LogFileWatcher::newLogStringSignal, this, &WalletApplication::newLogString);
      }
    }

    int initStatus = m_cryptoNoteAdapter->init(Settings::instance().getConnectionMethod(),
      Settings::instance().getLocalRpcPort(), Settings::instance().getRemoteRpcUrl());
    switch (initStatus) {
    case INodeAdapter::INIT_SUCCESS: {
      return true;
    }

    case INodeAdapter::INIT_DB_IN_USAGE: {
      QMessageBox dlg;
      dlg.setStyleSheet(QStringLiteral("min-width: 700px; font-size: %1px;").arg(Style::FONT_LARGE));
      QPushButton* okButton = new WalletOkButton(&dlg);
      okButton->setObjectName("m_okButton");
      okButton->setText(QObject::tr("Ok"));
      dlg.addButton(okButton, QMessageBox::AcceptRole);
      dlg.setText(QObject::tr("The database is currently used by another application or service.\n"
      "If you have intensecoind with non-default RPC port, you should terminate it and relaunch IntensecoinWallet\n"
      "or\n"
      "Set the Local deamon required port in IntensecoinWallet Menu/Preferences/Connection settings."));
	  if (m_splash != nullptr)
		  m_splash->hide();
      dlg.exec();
      return false;
    }

    case INodeAdapter::INIT_DB_FAILED: {
      if (askForRemoveDb()) {
        QString dbPath = Settings::instance().getDataDir().absoluteFilePath(
          Settings::instance().isTestnet() ? "testnet_DB" : "DB");

        if (!rmDir(dbPath)) {
          QMessageBox dlg;
          QPushButton* okButton = new QPushButton(&dlg);
          okButton->setObjectName("m_okButton");
          okButton->setText(QObject::tr("Ok"));
          dlg.addButton(okButton, QMessageBox::AcceptRole);
          dlg.setText(QObject::tr("Error occured"));
		  if (m_splash != nullptr)
			  m_splash->hide();
          dlg.exec();
          return false;
        }

        continue;
      }

      return false;
    }

    case INodeAdapter::INIT_CORE_FAILED: {
      showCoreInitError();
      return false;
    }

    case INodeAdapter::INIT_P2P_BIND_FAILED: {
      m_cryptoNoteAdapter->deinit();
      quint16 newPort = findPort();
      P2pBindPortErrorDialog dlg(Settings::instance().getP2pBindPort(), newPort, m_mainWindow);
      if (m_splash != nullptr) {
        m_splash->hide();
      }

      if (dlg.exec() == QDialog::Rejected) {
        return false;
      }

      Settings::instance().setP2pBindPort(dlg.getNewPort());
      continue;
    }

    default:
      return false;
    }
  }

  return false;
}

void WalletApplication::deinitCryptoNoteAdapter() {
  if (m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->isOpen()) {
    m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->save(CryptoNote::WalletSaveLevel::SAVE_ALL, true);
    m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->close();
  }

  m_cryptoNoteAdapter->deinit();
}

void WalletApplication::reinitCryptoNoteAdapter() {
  deinitCryptoNoteAdapter();
  initCryptoNoteAdapter();
}

void WalletApplication::initUi() {
  AddressBookManager* addressBookManager = new AddressBookManager(m_cryptoNoteAdapter, this);
  m_addressBookManager = addressBookManager;
  m_donationManager = addressBookManager;
  m_optimizationManager= new OptimizationManager(m_cryptoNoteAdapter, this);
  m_miningManager = new MiningManager(m_cryptoNoteAdapter, m_donationManager, this);
  if (m_splash != nullptr) {
    m_splash->showMessage(QObject::tr("Initializing GUI..."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
  }

  QFile styleSheetFile(":style/qss");
  styleSheetFile.open(QIODevice::ReadOnly);
  QByteArray styleSheet = styleSheetFile.readAll();
  styleSheetFile.close();
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(styleSheet));
  m_mainWindow = new MainWindow(m_cryptoNoteAdapter, m_addressBookManager, m_donationManager, m_optimizationManager,
    m_miningManager, m_applicationEventHandler, m_blogReader, styleSheet, nullptr);
  connect(static_cast<MainWindow*>(m_mainWindow), &MainWindow::reinitCryptoNoteAdapterSignal,
    this, &WalletApplication::reinitCryptoNoteAdapter);
  if (m_splash != nullptr) {
    if (m_logWatcher != nullptr) {
      m_logWatcher->deleteLater();
      m_logWatcher = nullptr;
    }

    m_splash->finish(m_mainWindow);
    m_splash->deleteLater();
    m_splash = nullptr;
  }

  m_mainWindow->show();
#ifdef Q_OS_WIN
  WTSRegisterSessionNotification((HWND)m_mainWindow->winId(), NOTIFY_FOR_THIS_SESSION);
#endif
}

void WalletApplication::initSystemTrayIcon() {
#ifdef Q_OS_MAC
  installDockHandler();
#else
  if (!Settings::instance().isSystemTrayAvailable()) {
    return;
  }

  connect(m_systemTrayIcon, &QSystemTrayIcon::activated, this, &WalletApplication::trayActivated);
  m_systemTrayIcon->setIcon(Settings::instance().getCurrentStyle().getSystemTrayIcon());
  m_systemTrayIcon->show();
#endif
}

void WalletApplication::showCoreInitError() {
  QMessageBox dlg;
  QPushButton* okButton = new QPushButton(&dlg);
  okButton->setObjectName("m_okButton");
  okButton->setText(QObject::tr("Ok"));
  dlg.addButton(okButton, QMessageBox::AcceptRole);
  dlg.setText(QObject::tr("Blockchain load error"));
  if (m_splash != nullptr)
	  m_splash->hide();
  dlg.exec();
}

bool WalletApplication::askForRemoveDb()
{
  QuestionDialog dlg(tr("Blockchain load error"),
    tr("Erase Blockchain folder and sync from scratch?\n"
    "Attention! If you keep any important files in the same folder they may be\n"
    "erased as well. Are you sure you want to erase the folder and sync again?"), m_mainWindow);
  if (m_splash != nullptr)
	  m_splash->hide();
  return (dlg.exec() == QDialog::Accepted);
}

void WalletApplication::trayActivated(QSystemTrayIcon::ActivationReason _reason) {
  if (m_splash != nullptr && !m_splash->isVisible()) {
    m_splash->show();
  } else if (m_mainWindow != nullptr) {
    Qt::WindowStates state = m_mainWindow->window()->windowState();
    m_mainWindow->window()->setWindowState(state & ~Qt::WindowMinimized);
    m_mainWindow->window()->show();
    m_mainWindow->raise();
    m_mainWindow->activateWindow();
  }
}

void WalletApplication::prepareToQuit() {
  WalletLogger::debug(tr("[Application] Prepare to quit..."));
  if (Settings::instance().isNewsEnabled()) {
    m_blogReader->deinit();
  }

  Settings::instance().removeObserver(this);
  m_isAboutToQuit = true;
  m_systemTrayIcon->hide();
  m_mainWindow->close();
  ExitWidget* exitWidget = new ExitWidget(nullptr);
  exitWidget->setAttribute(Qt::WA_DeleteOnClose);
  exitWidget->show();
  deinitCryptoNoteAdapter();
  exitWidget->close();
  m_mainWindow->deleteLater();
  WalletLogger::info(tr("[Application] Quit"));
  processEvents();
  WalletLogger::deinit();
}

void WalletApplication::newLogString(const QString& _string) {
  QRegularExpressionMatch match = LOG_SPLASH_REG_EXP.match(_string);
  if (match.hasMatch()) {
    QString message = _string.mid(match.capturedEnd()).prepend(tr("Import"));
    if (m_splash != nullptr) {
      m_splash->showMessage(message, Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    }
  }
}

}
