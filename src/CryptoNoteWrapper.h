// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2020 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <system_error>
#include <INode.h>

namespace CryptoNote {

class INode;
class IWalletLegacy;
class Currency;
class CoreConfig;
class NetNodeConfig;

}

namespace Logging {
  class LoggerManager;
}

namespace WalletGui {

class Node {
public:
  virtual ~Node() = 0;
  virtual void init(const std::function<void(std::error_code)>& callback) = 0;
  virtual void deinit() = 0;
  
  virtual std::string convertPaymentId(const std::string& paymentIdString) = 0;
  virtual std::string extractPaymentId(const std::string& extra) = 0;
  virtual uint64_t getLastKnownBlockHeight() const = 0;
  virtual uint64_t getLastLocalBlockHeight() const = 0;
  virtual uint64_t getLastLocalBlockTimestamp() const = 0;
  virtual uint64_t getPeerCount() = 0;
  virtual uint64_t getDifficulty() = 0;
  virtual uint64_t getTxCount() = 0;
  virtual uint64_t getTxPoolSize() = 0;
  virtual uint64_t getAltBlocksCount() = 0;
  virtual uint64_t getConnectionsCount() = 0;
  virtual uint64_t getOutgoingConnectionsCount() = 0;
  virtual uint64_t getIncomingConnectionsCount() = 0;
  virtual uint64_t getWhitePeerlistSize() = 0;
  virtual uint64_t getGreyPeerlistSize() = 0;
  virtual uint64_t getMinimalFee() = 0;
  virtual std::string feeAddress() const = 0;
  virtual uint64_t feeAmount() const = 0;
  virtual uint8_t getCurrentBlockMajorVersion() = 0;
  virtual uint64_t getAlreadyGeneratedCoins() = 0;
  virtual CryptoNote::BlockHeaderInfo getLastLocalBlockHeaderInfo() = 0;

  virtual std::vector<CryptoNote::p2pConnection> getConnections() = 0;

  virtual CryptoNote::IWalletLegacy* createWallet() = 0;
};

class INodeCallback {
public:
  virtual void peerCountUpdated(Node& node, size_t count) = 0;
  virtual void localBlockchainUpdated(Node& node, uint64_t height) = 0;
  virtual void lastKnownBlockHeightUpdated(Node& node, uint64_t height) = 0;
};

Node* createRpcNode(const CryptoNote::Currency& currency, INodeCallback& callback, Logging::LoggerManager& logManager, const std::string& nodeHost, unsigned short nodePort);
Node* createInprocessNode(const CryptoNote::Currency& currency, Logging::LoggerManager& logManager,
  const CryptoNote::CoreConfig& coreConfig, const CryptoNote::NetNodeConfig& netNodeConfig, INodeCallback& callback);

}
