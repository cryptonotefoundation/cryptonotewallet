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

#include "CommonNodeAdapter.h"

namespace Logging {
  class ILogger;
}

namespace CryptoNote {
  class Currency;
}

namespace WalletGui {

class InProcessNodeAdapter : public CommonNodeAdapter {
  Q_OBJECT
  Q_DISABLE_COPY(InProcessNodeAdapter)

public:
  InProcessNodeAdapter(const CryptoNote::Currency& _currency, Logging::ILogger& _loggerManager, Logging::ILogger& _walletLogger, QObject* _parent);
  virtual ~InProcessNodeAdapter();

protected:
  virtual INodeAdapter* createWorker() const override;

private:
  const CryptoNote::Currency& m_currency;
  Logging::ILogger& m_loggerManager;
  Logging::ILogger& m_walletLogger;
};

}
