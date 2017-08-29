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

#include <QObject>
#include <QMap>
#include <QMetaObject>

#include "INodeAdapter.h"
#include "INode.h"

namespace Logging {
  class ILogger;
}

namespace System {
  class Dispatcher;
  class Event;
}

namespace CryptoNote {
  class RocksDBWrapper;
  class Currency;
  class Core;
  class CryptoNoteProtocolHandler;
  class NodeServer;
  class InProcessNode;
}

namespace WalletGui {

class IBlockChainExplorerAdapter;

class InProcessNodeWorker : public QObject, public INodeAdapter, public CryptoNote::INodeObserver {
  Q_OBJECT
  Q_DISABLE_COPY(InProcessNodeWorker)

public:
  InProcessNodeWorker(const CryptoNote::Currency& _currency, Logging::ILogger& _loggerManager, Logging::ILogger& m_walletLogger,
    QObject* _parent);
  virtual ~InProcessNodeWorker();

  // ICryptoNoteAdapter
  virtual void init() override;
  virtual void deinit() override;
  virtual NodeType getNodeType() const override;
  virtual quintptr getPeerCount() const override;
  virtual quint32 getKnownBlockCount() const override;
  virtual quint32 getLocalBlockCount() const override;
  virtual CryptoNote::BlockHeaderInfo getLastLocalBlockInfo() const override;
  virtual void addObserver(INodeAdapterObserver* _observer) override;
  virtual void removeObserver(INodeAdapterObserver* _observer) override;
  virtual IBlockChainExplorerAdapter* getBlockChainExplorerAdapter() override;
  virtual IWalletAdapter* getWalletAdapter() override;

  // CryptoNote::INodeObserver
  virtual void peerCountUpdated(size_t _count) override;
  virtual void localBlockchainUpdated(uint32_t _height) override;
  virtual void lastKnownBlockHeightUpdated(uint32_t _height) override;

private:
  const CryptoNote::Currency& m_currency;
  Logging::ILogger& m_loggerManager;
  QScopedPointer<CryptoNote::RocksDBWrapper> m_database;
  QScopedPointer<CryptoNote::Core> m_core;
  Logging::ILogger& m_walletLogger;
  QScopedPointer<System::Dispatcher> m_dispatcher;
  QScopedPointer<CryptoNote::CryptoNoteProtocolHandler> m_protocolHandler;
  QScopedPointer<CryptoNote::NodeServer> m_nodeServer;
  QScopedPointer<CryptoNote::InProcessNode> m_node;
  IBlockChainExplorerAdapter* m_blockchainExplorerAdapter;
  QMap<INodeAdapterObserver*, QList<QMetaObject::Connection>> m_observerConnections;


  Q_INVOKABLE void initImpl();
  Q_INVOKABLE void deinitImpl();
  InitStatus initCore();
  InitStatus initNodeServer();
  InitStatus initInProcessNode();

Q_SIGNALS:
  void initCompletedSignal(int _status);
  void deinitCompletedSignal();
  void peerCountUpdatedSignal(quintptr _count);
  void localBlockchainUpdatedSignal(CryptoNote::BlockHeaderInfo _lastLocalBlockInfo);
  void lastKnownBlockHeightUpdatedSignal(quint32 _height);
  void connectionStatusUpdatedSignal(bool _connected);
  void nodeInitCompletedSignal(int _errorCode);
  void readyToRunSignal(int _errorCode);
};

}
