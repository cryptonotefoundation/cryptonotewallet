// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2021 The Karbo developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QMutex>
#include <QObject>
#include <QTime>
#include <QTimer>
#include <QPushButton>

#include <list>
#include <vector>
#include <atomic>
#include <fstream>

#include <boost/program_options.hpp>

#include <IWalletLegacy.h>
#include "Wallet/WalletRpcServer.h"

namespace WalletGui {

class ITransfersContainer;

class WalletAdapter : public QObject, public CryptoNote::IWalletLegacyObserver {
  Q_OBJECT
  Q_DISABLE_COPY(WalletAdapter)

public:
  static WalletAdapter& instance();

  void open(const QString& _password);
  void createWallet();
  void createNonDeterministic();
  void createWithKeys(const CryptoNote::AccountKeys& _keys);
  void createWithKeys(const CryptoNote::AccountKeys& _keys, const quint32 _sync_heigth);
  void close();
  bool save(bool _details, bool _cache);
  void backup(const QString& _file);
  void autoBackup();
  void reset();

  QString getAddress() const;
  quint64 getActualBalance() const;
  quint64 getUnmixableBalance() const;
  quint64 getPendingBalance() const;
  quint64 getTransactionCount() const;
  quint64 getTransferCount() const;
  bool getTransaction(CryptoNote::TransactionId& _id, CryptoNote::WalletLegacyTransaction& _transaction);
  bool getTransfer(CryptoNote::TransferId& _id, CryptoNote::WalletLegacyTransfer& _transfer);
  bool getAccountKeys(CryptoNote::AccountKeys& _keys);
  QString getTxProof(Crypto::Hash& _txid, CryptoNote::AccountPublicAddress& _address, Crypto::SecretKey& _tx_key);
  QString getReserveProof(const quint64 &_reserve, const QString &_message);
  Crypto::SecretKey getTxKey(Crypto::Hash& txid);
  size_t getUnlockedOutputsCount();

  std::vector<CryptoNote::TransactionOutputInformation> getOutputs();
  std::vector<CryptoNote::TransactionOutputInformation> getLockedOutputs();
  std::vector<CryptoNote::TransactionOutputInformation> getUnlockedOutputs();
  std::vector<CryptoNote::TransactionSpentOutputInformation> getSpentOutputs();

  void sendTransaction(const std::vector<CryptoNote::WalletLegacyTransfer>& _transfers, quint64 _fee, const QString& _payment_id, quint64 _mixin);
  void sendTransaction(const std::vector<CryptoNote::WalletLegacyTransfer>& _transfers, const std::list<CryptoNote::TransactionOutputInformation>& _selectedOuts, quint64 _fee, const QString& _payment_id, quint64 _mixin);

  QString prepareRawTransaction(const std::vector<CryptoNote::WalletLegacyTransfer>& _transfers, quint64 _fee, const QString& _payment_id, quint64 _mixin);
  QString prepareRawTransaction(const std::vector<CryptoNote::WalletLegacyTransfer>& _transfers, const std::list<CryptoNote::TransactionOutputInformation>& _selectedOuts, quint64 _fee, const QString& _payment_id, quint64 _mixin);

  quint64 estimateFusion(quint64 _threshold);
  std::list<CryptoNote::TransactionOutputInformation> getFusionTransfersToSend(quint64 _threshold, size_t _min_input_count, size_t _max_input_count);
  void sendFusionTransaction(const std::list<CryptoNote::TransactionOutputInformation>& _fusion_inputs, quint64 _fee, const QString& _extra, quint64 _mixin);
  bool isFusionTransaction(const CryptoNote::WalletLegacyTransaction& walletTx) const;

  bool isOpen() const;

  bool changePassword(const QString& _old_pass, const QString& _new_pass);
  void setWalletFile(const QString& _path);

  QString signMessage(const QString &data);
  bool verifyMessage(const QString &data, const CryptoNote::AccountPublicAddress &address, const QString &signature);

  void initCompleted(std::error_code _result) Q_DECL_OVERRIDE;
  void saveCompleted(std::error_code _result) Q_DECL_OVERRIDE;
  void synchronizationProgressUpdated(uint32_t _current, uint32_t _total) Q_DECL_OVERRIDE;
  void synchronizationCompleted(std::error_code _error) Q_DECL_OVERRIDE;
  void actualBalanceUpdated(uint64_t _actual_balance) Q_DECL_OVERRIDE;
  void pendingBalanceUpdated(uint64_t _pending_balance) Q_DECL_OVERRIDE;
  void unmixableBalanceUpdated(uint64_t _dust_balance) Q_DECL_OVERRIDE;
  void externalTransactionCreated(CryptoNote::TransactionId _transaction_id) Q_DECL_OVERRIDE;
  void sendTransactionCompleted(CryptoNote::TransactionId _transaction_id, std::error_code _result) Q_DECL_OVERRIDE;
  void transactionUpdated(CryptoNote::TransactionId _transaction_id) Q_DECL_OVERRIDE;

  bool isDeterministic() const;
  bool isDeterministic(CryptoNote::AccountKeys& _keys) const;
  QString getMnemonicSeed(QString _language) const;
  CryptoNote::AccountKeys getKeysFromMnemonicSeed(QString& _seed) const;
  bool tryOpen(const QString& _password);

private:
  std::fstream m_file;
  CryptoNote::IWalletLegacy* m_wallet;
  Tools::wallet_rpc_server* m_wallet_rpc;
  QMutex m_mutex;
  std::atomic<bool> m_isBackupInProgress;
  std::atomic<bool> m_isSynchronized;
  std::atomic<quint64> m_lastWalletTransactionId;
  QTimer m_newTransactionsNotificationTimer;
  QPushButton* m_closeButton;
  Logging::LoggerRef m_logger;
  uint32_t m_syncSpeed;
  uint32_t m_syncPeriod;
  struct PerfType { uint32_t height; QTime time; };
  std::vector<PerfType> m_perfData;

  boost::program_options::variables_map m_wrpcOptions;

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
  QString walletErrorMessage(int _error_code);
  void runWalletRpc();

  static void renameFile(const QString& _old_name, const QString& _new_name);
  Q_SLOT void updateBlockStatusText();
  Q_SLOT void updateBlockStatusTextWithDelay();

Q_SIGNALS:
  void walletInitCompletedSignal(int _error, const QString& _error_text);
  void walletCloseCompletedSignal();
  void walletSaveCompletedSignal(int _error, const QString& _error_text);
  void walletSynchronizationProgressUpdatedSignal(quint64 _current, quint64 _total);
  void walletSynchronizationCompletedSignal(int _error, const QString& _error_text);
  void walletActualBalanceUpdatedSignal(quint64 _actual_balance);
  void walletPendingBalanceUpdatedSignal(quint64 _pending_balance);
  void walletUnmixableBalanceUpdatedSignal(quint64 _dust_balance);
  void walletTransactionCreatedSignal(CryptoNote::TransactionId _transaction_id);
  void walletSendTransactionCompletedSignal(CryptoNote::TransactionId _transaction_id, int _error, const QString& _error_text);
  void walletTransactionUpdatedSignal(CryptoNote::TransactionId _transaction_id);
  void walletStateChangedSignal(const QString &_state_text);

  void openWalletWithPasswordSignal(bool _error);
  void changeWalletPasswordSignal();
  void updateWalletAddressSignal(const QString& _address);
  void reloadWalletTransactionsSignal();
  void updateBlockStatusTextSignal();
  void updateBlockStatusTextWithDelaySignal();
};

}
