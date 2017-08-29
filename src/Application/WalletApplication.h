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

#pragma once

#include <QApplication>
#include <QSystemTrayIcon>

#include "Settings/Settings.h"

class QLockFile;
class QMainWindow;
class QSplashScreen;

namespace WalletGui {

class CommandLineParser;
class IAddressBookManager;
class IApplicationEventHandler;
class ICryptoNoteAdapter;
class IDonationManager;
class IOptimizationManager;
class IMiningManager;
class INewsReader;
class LogFileWatcher;

class WalletApplication : public QApplication, public ISettingsObserver {
  Q_OBJECT
  Q_DISABLE_COPY(WalletApplication)

public:
  WalletApplication(int& _argc, char** _argv);
  ~WalletApplication();

  bool init();
  void dockClickHandler();

  // ISettingsObserver
  virtual void settingsUpdated() override;

private:
  QLockFile* m_lockFile;
  QSystemTrayIcon* m_systemTrayIcon;
  CommandLineParser* m_commandLineParser;
  IApplicationEventHandler* m_applicationEventHandler;
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  IAddressBookManager* m_addressBookManager;
  IDonationManager* m_donationManager;
  IOptimizationManager* m_optimizationManager;
  IMiningManager* m_miningManager;
  INewsReader* m_blogReader;
  QMainWindow* m_mainWindow;
  QSplashScreen* m_splash;
  LogFileWatcher* m_logWatcher;
  bool m_isAboutToQuit;

  void loadFonts();
  void makeDataDir();
  void setupTheme();
  bool initCryptoNoteAdapter();
  void deinitCryptoNoteAdapter();
  void reinitCryptoNoteAdapter();
  void initUi();
  void initSystemTrayIcon();
  void showCoreInitError();
  bool askForRemoveDb();
  void trayActivated(QSystemTrayIcon::ActivationReason _reason);
  void prepareToQuit();
  void newLogString(const QString& _string);

#ifdef Q_OS_MAC
private:
  void installDockHandler();
#endif
};

}
