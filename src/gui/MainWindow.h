// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QLocale>
#include <QTranslator>
#include <QLabel>
#include <QPushButton>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTimer>
#include "CommandLineParser.h"
#include "PaymentServer.h"
#include "OptimizationManager.h"

class QActionGroup;
class OptimizationManager;

namespace Ui {
class MainWindow;
}

namespace WalletGui {

class MainWindow : public QMainWindow {
  Q_OBJECT
  Q_DISABLE_COPY(MainWindow)

public:
  static MainWindow& instance();
  void scrollToTransaction(const QModelIndex& _index);
  void handlePaymentRequest(QString _request);
  void isTrackingMode();
  void checkTrackingMode();
  void quit();

protected:
  void closeEvent(QCloseEvent* _event) Q_DECL_OVERRIDE;
  bool event(QEvent* _event) Q_DECL_OVERRIDE;
  void changeEvent(QEvent* _event) Q_DECL_OVERRIDE;

protected slots:
  void slotLanguageChanged(QAction* action);

private slots:
  void createLanguageMenu(void);

private:
  PaymentServer* paymentServer;
  OptimizationManager* optimizationManager;

  QScopedPointer<Ui::MainWindow> m_ui;
  QPushButton* m_connectionStateIconLabel;
  QLabel* m_encryptionStateIconLabel;
  QLabel* m_synchronizationStateIconLabel;
  QLabel* m_trackingModeIconLabel;
  QLabel* m_remoteModeIconLabel;
  QSystemTrayIcon* m_trayIcon;
  QActionGroup* m_tabActionGroup;
  QAction* accountWidget;
  QAction* toggleHideAction;
  bool m_isAboutToQuit;
  QList<QAction*> recentFileActionList;
  const int maxRecentFiles;

  QTranslator m_translator; // contains the translations for this application
  QTranslator m_translatorQt; // contains the translations for qt
  QString m_currLang; // contains the currently loaded language
  QString m_langPath; // Path of language files. This is always fixed to /languages.

  static MainWindow* m_instance;

  QMenu *trayIconMenu;

  MainWindow();
  ~MainWindow();

  void connectToSignals();
  void initUi();
  void setMainWindowTitle();
  void createTrayIcon();
  void createTrayIconMenu();
  void loadLanguage(const QString& rLanguage);
  void minimizeToTray(bool _on);
  void setStatusBarText(const QString& _text);
  void showMessage(const QString& _text, QtMsgType _type);
  void askForWalletPassword(bool _error);
  void encryptedFlagChanged(bool _encrypted);
  void peerCountUpdated(quint64 _peer_count);
  void walletSynchronizationInProgress();
  void walletSynchronized(int _error, const QString& _error_text);
  void walletOpened(bool _error, const QString& _error_text);
  void walletClosed();
  void updateWalletAddress(const QString& _address);
  void reset();
  void onShowQR();
  void onUriOpenSignal();
  void adjustForCurrentFile(const QString& filePath);
  void updateRecentActionList();
  void updateUnmixableBalance(quint64 _balance);
  void payTo(const QModelIndex& _index);

  Q_SLOT void createWallet();
  Q_SLOT void createNonDeterministicWallet();
  Q_SLOT void openWallet();
  Q_SLOT void closeWallet();
  Q_SLOT void importKey();
  Q_SLOT void importKeys();
  Q_SLOT void backupWallet();
  Q_SLOT void resetWallet();
  Q_SLOT void encryptWallet();
  Q_SLOT void aboutQt();
  Q_SLOT void about();
  Q_SLOT void setStartOnLogin(bool _on);
  Q_SLOT void setMinimizeToTray(bool _on);
  Q_SLOT void setCloseToTray(bool _on);
  Q_SLOT void setHideFusionTransactions(bool _on);
  Q_SLOT void showPrivateKeys();
  Q_SLOT void DisplayCmdLineHelp();
  Q_SLOT void openConnectionSettings();
  Q_SLOT void openOptimizationSettings();
  Q_SLOT void exportTrackingKey();
  Q_SLOT void importTrackingKey();
  Q_SLOT void signMessage();
  Q_SLOT void verifyMessage();
  Q_SLOT void openRecent();
  Q_SLOT void showStatusInfo();
  Q_SLOT void openLogFile();
  Q_SLOT void toggleHidden();
  Q_SLOT void showNormalIfMinimized(bool fToggleHidden = false);
  Q_SLOT void showMnemonicSeed();
  Q_SLOT void restoreFromMnemonicSeed();
  Q_SLOT void sweepUnmixable();
  Q_SLOT void getBalanceProof();

  bool isObscured(QWidget *w);
  bool checkPoint(const QPoint &p, const QWidget *w);

#ifdef Q_OS_MAC
public:
  void restoreFromDock();

private:
  void installDockHandler();
#endif

#ifdef Q_OS_WIN
private:
  Q_SLOT void trayActivated(QSystemTrayIcon::ActivationReason _reason);
#endif
};

}
