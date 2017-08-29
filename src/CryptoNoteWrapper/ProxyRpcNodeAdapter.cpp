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

#include "ProxyRpcNodeAdapter.h"
#include "ProxyRpcNodeWorker.h"
#include "WalletGreenAdapter.h"

namespace WalletGui {

ProxyRpcNodeAdapter::ProxyRpcNodeAdapter(const CryptoNote::Currency& _currency, Logging::ILogger& _loggerManager, Logging::ILogger& _walletLogger,
  const QString& _nodeHost, quint16 _nodePort, QObject* _parent) : CommonNodeAdapter(_parent), m_currency(_currency),
  m_loggerManager(_loggerManager), m_walletLogger(_walletLogger), m_nodeHost(_nodeHost), m_nodePort(_nodePort) {
}

ProxyRpcNodeAdapter::~ProxyRpcNodeAdapter() {
}

INodeAdapter* ProxyRpcNodeAdapter::createWorker() const {
  return new ProxyRpcNodeWorker(m_currency, m_loggerManager, m_walletLogger, m_nodeHost, m_nodePort, nullptr);
}

}
