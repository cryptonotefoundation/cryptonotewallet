// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QLabel>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTimer>

class QActionGroup;

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
  void quit();

protected:
  void closeEvent(QCloseEvent* _event) Q_DECL_OVERRIDE;
  bool event(QEvent* _event) Q_DECL_OVERRIDE;

private:
  QScopedPointer<Ui::MainWindow> m_ui;
  QLabel* m_connectionStateIconLabel;
  QLabel* m_encryptionStateIconLabel;
  QLabel* m_synchronizationStateIconLabel;
  QSystemTrayIcon* m_trayIcon;
  QActionGroup* m_tabActionGroup;
  bool m_isAboutToQuit;

  static MainWindow* m_instance;

  MainWindow();
  ~MainWindow();

  void connectToSignals();
  void initUi();

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

  Q_SLOT void createWallet();
  Q_SLOT void openWallet();
  Q_SLOT void backupWallet();
  Q_SLOT void encryptWallet();
  Q_SLOT void aboutQt();
  Q_SLOT void about();
  Q_SLOT void setStartOnLogin(bool _on);
  Q_SLOT void setMinimizeToTray(bool _on);
  Q_SLOT void setCloseToTray(bool _on);

#ifdef Q_OS_MAC
public:
  void restoreFromDock();

private:
  void installDockHandler();
#elif defined(Q_OS_WIN)
protected:
  void changeEvent(QEvent* _event) Q_DECL_OVERRIDE;

private:
  void trayActivated(QSystemTrayIcon::ActivationReason _reason);
#endif
};

}
