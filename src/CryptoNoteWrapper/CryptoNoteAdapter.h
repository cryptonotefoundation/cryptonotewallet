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

#include <QDir>
#include <QObject>
#include <QUrl>

#include "ICryptoNoteAdapter.h"
#include "INodeAdapter.h"

#include "Logging/LoggerManager.h"
#include "CryptoNoteCore/Currency.h"

class QDir;

namespace WalletGui {

const char CORE_LOG_FILE_NAME[] = "core.log";
const char WALLET_LOG_FILE_NAME[] = "wallet.log";

class INodeAdapter;

class CryptoNoteAdapter : public QObject, public ICryptoNoteAdapter, public INodeAdapterObserver {
  Q_OBJECT
  Q_DISABLE_COPY(CryptoNoteAdapter)

public:
  CryptoNoteAdapter(const QDir& _dataDir, bool _testnet, bool _debug, QObject* _parent);
  virtual ~CryptoNoteAdapter();

  // ICryptoNoteAdapter
  virtual int init(ConnectionMethod _connectionMethod, quint16 _localDaemonPort,
    const QUrl& _remoteDaemonUrl) override;
  virtual void deinit() override;
  virtual INodeAdapter* getNodeAdapter() const override;
  virtual bool isValidAddress(const QString& _address) const override;
  virtual bool isValidPaymentId(const QString& _paymentId) const override;
  virtual QString getCurrencyTicker() const override;
  virtual quint64 getMinimalFee() const override;
  virtual quint64 getTargetTime() const override;
  virtual QString formatAmount(qint64 _amount) const override;
  virtual QString formatUnsignedAmount(quint64 _amount) const override;
  virtual QString formatAmountToShort(qint64 _amount) const override;
  virtual qint64 parseAmount(const QString& _amountString) const override;
  virtual quint64 parseUnsignedAmount(const QString& _amountString) const override;
  virtual std::string convertPaymentToExtra(const QString& _paymentIdString) const override;
  virtual QString extractPaymentIdFromExtra(const std::string& _extra) const override;
  virtual void addObserver(ICryptoNoteAdapterObserver* _observer) override;
  virtual void removeObserver(ICryptoNoteAdapterObserver* _observer) override;

  // INodeAdapterObserver
  Q_SLOT virtual void initCompleted(int _status) override;
  Q_SLOT virtual void deinitCompleted() override;
  Q_SLOT virtual void peerCountUpdated(quintptr _count) override;
  Q_SLOT virtual void localBlockchainUpdated(CryptoNote::BlockHeaderInfo _lastLocalBlockInfo) override;
  Q_SLOT virtual void lastKnownBlockHeightUpdated(quint32 _height) override;
  Q_SLOT virtual void connectionStatusUpdated(bool _connected) override;

protected:
  virtual void timerEvent(QTimerEvent* _event) override;

private:
  QDir m_dataDir;
  bool m_testnet;
  bool m_debug;
  ConnectionMethod m_connectionMethod;
  quint16 m_localDaemodPort;
  QUrl m_remoteDaemonUrl;
  Logging::LoggerManager m_coreLogger;
  Logging::LoggerManager m_walletLogger;
  CryptoNote::Currency m_currency;
  INodeAdapter* m_nodeAdapter;
  int m_autoConnectionTimerId;

  void initCoreLogger(bool _debug);
  void initWalletLogger(bool _debug);
  void initNode();
  void initAutoConnection();
  void initInProcessNode();
  void initLocalRpcNode();
  void initRemoteRpcNode();
  void onLocalDaemonNotFound();

  static void configureLogger(Logging::LoggerManager& _logger, const QString& _logFilePath, bool _debug);

Q_SIGNALS:
  void initCompletedSignal(int _initStatus);
  void deinitCompletedSignal();
};

}
