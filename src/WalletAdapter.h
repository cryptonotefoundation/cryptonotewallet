// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QMutex>
#include <QObject>
#include <QTimer>

#include <atomic>
#include <fstream>

#include <IWalletLegacy.h>

namespace WalletGui {

class WalletAdapter : public QObject, public CryptoNote::IWalletLegacyObserver {
  Q_OBJECT
  Q_DISABLE_COPY(WalletAdapter)

public:
  static WalletAdapter& instance();

  void open(const QString& _password);
  void createWithKeys(const CryptoNote::AccountKeys& _keys);
  void close();
  bool save(bool _details, bool _cache);
  void backup(const QString& _file);

  QString getAddress() const;
  quint64 getActualBalance() const;
  quint64 getPendingBalance() const;
  quint64 getActualDepositBalance() const;
  quint64 getPendingDepositBalance() const;
  quint64 getTransactionCount() const;
  quint64 getTransferCount() const;
  quint64 getDepositCount() const;
  bool getTransaction(CryptoNote::TransactionId _id, CryptoNote::WalletLegacyTransaction& _transaction);
  bool getTransfer(CryptoNote::TransferId _id, CryptoNote::WalletLegacyTransfer& _transfer);
  bool getDeposit(CryptoNote::DepositId _id, CryptoNote::Deposit& _deposit);
  bool getAccountKeys(CryptoNote::AccountKeys& _keys);
  bool isOpen() const;
  void sendTransaction(const QVector<CryptoNote::WalletLegacyTransfer>& _transfers, quint64 _fee, const QString& _payment_id, quint64 _mixin,
    const QVector<CryptoNote::TransactionMessage>& _messages);
  void sendMessage(const QVector<CryptoNote::WalletLegacyTransfer>& _transfers, quint64 _fee, quint64 _mixin,
    const QVector<CryptoNote::TransactionMessage>& _messages);
  void deposit(quint32 _term, quint64 _amount, quint64 _fee, quint64 _mixIn);
  void withdrawUnlockedDeposits(QVector<CryptoNote::DepositId> _depositIds, quint64 _fee);
  bool changePassword(const QString& _old_pass, const QString& _new_pass);
  void setWalletFile(const QString& _path);

  void initCompleted(std::error_code _result) Q_DECL_OVERRIDE;
  void saveCompleted(std::error_code _result) Q_DECL_OVERRIDE;
  void synchronizationProgressUpdated(uint32_t _current, uint32_t _total) Q_DECL_OVERRIDE;
  void synchronizationCompleted(std::error_code _error) Q_DECL_OVERRIDE;
  void actualBalanceUpdated(uint64_t _actualBalance) Q_DECL_OVERRIDE;
  void pendingBalanceUpdated(uint64_t _pendingBalance) Q_DECL_OVERRIDE;
  void actualDepositBalanceUpdated(uint64_t _actualDepositBalance) Q_DECL_OVERRIDE;
  void pendingDepositBalanceUpdated(uint64_t _pendingDepositBalance) Q_DECL_OVERRIDE;
  void externalTransactionCreated(CryptoNote::TransactionId _transactionId) Q_DECL_OVERRIDE;
  void sendTransactionCompleted(CryptoNote::TransactionId _transactionId, std::error_code _result) Q_DECL_OVERRIDE;
  void transactionUpdated(CryptoNote::TransactionId _transactionId) Q_DECL_OVERRIDE;
  void depositsUpdated(const std::vector<CryptoNote::DepositId>& _depositIds) Q_DECL_OVERRIDE;

private:
  std::fstream m_file;
  CryptoNote::IWalletLegacy* m_wallet;
  QMutex m_mutex;
  std::atomic<bool> m_isBackupInProgress;
  std::atomic<bool> m_isSynchronized;
  std::atomic<quint64> m_lastWalletTransactionId;
  QTimer m_newTransactionsNotificationTimer;
  std::atomic<CryptoNote::TransactionId> m_sentTransactionId;
  std::atomic<CryptoNote::TransactionId> m_sentMessageId;
  std::atomic<CryptoNote::TransactionId> m_depositId;
  std::atomic<CryptoNote::TransactionId> m_depositWithdrawalId;


  WalletAdapter();
  ~WalletAdapter();

  void onWalletInitCompleted(int _error, const QString& _error_text);
  void onWalletSendTransactionCompleted(CryptoNote::TransactionId _transaction_id, int _error, const QString& _error_text);

  bool importLegacyWallet(const QString &_password);
  bool save(const QString& _file, bool _details, bool _cache);
  void lock();
  void unlock();
  bool openFile(const QString& _file, bool _read_only);
  void closeFile();
  void notifyAboutLastTransaction();

  static void renameFile(const QString& _old_name, const QString& _new_name);
  Q_SLOT void updateBlockStatusText();
  Q_SLOT void updateBlockStatusTextWithDelay();

Q_SIGNALS:
  void walletInitCompletedSignal(int _error, const QString& _errorText);
  void walletCloseCompletedSignal();
  void walletSaveCompletedSignal(int _error, const QString& _errorText);
  void walletSynchronizationProgressUpdatedSignal(quint64 _current, quint64 _total);
  void walletSynchronizationCompletedSignal(int _error, const QString& _error_text);
  void walletActualBalanceUpdatedSignal(quint64 _actualBalance);
  void walletPendingBalanceUpdatedSignal(quint64 _pendingBalance);
  void walletActualDepositBalanceUpdatedSignal(quint64 _actualDepositBalance);
  void walletPendingDepositBalanceUpdatedSignal(quint64 _pendingDepositBalance);
  void walletTransactionCreatedSignal(CryptoNote::TransactionId _transactionId);
  void walletSendTransactionCompletedSignal(CryptoNote::TransactionId _transactionId, int _error, const QString& _errorText);
  void walletSendMessageCompletedSignal(CryptoNote::TransactionId _transactionId, int _error, const QString& _errorText);
  void walletCreateDepositCompletedSignal(CryptoNote::TransactionId _transactionId, int _error, const QString& _errorText);
  void walletWithdrawDepositCompletedSignal(CryptoNote::TransactionId _transactionId, int _error, const QString& _errorText);
  void walletTransactionUpdatedSignal(CryptoNote::TransactionId _transactionId);
  void walletDepositsUpdatedSignal(const QVector<CryptoNote::DepositId>& _depositIds);
  void walletStateChangedSignal(const QString &_stateText);

  void openWalletWithPasswordSignal(bool _error);
  void changeWalletPasswordSignal();
  void updateWalletAddressSignal(const QString& _address);
  void reloadWalletTransactionsSignal();
  void updateBlockStatusTextSignal();
  void updateBlockStatusTextWithDelaySignal();
};



}
