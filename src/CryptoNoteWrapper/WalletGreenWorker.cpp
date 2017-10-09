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

#include <QDir>
#include <QEventLoop>
#include <QHash>
#include <QTemporaryFile>

#include <fstream>

#include "WalletGreenWorker.h"
#include "GuardExecutor.h"
#include "WalletLogger/WalletLogger.h"
#include "SemaphoreUtils.h"
#include "System/Dispatcher.h"
#include "System/Event.h"
#include "Wallet/LegacyKeysImporter.h"
#include "Wallet/WalletGreen.h"
#include "Wallet/WalletErrors.h"

namespace WalletGui {

namespace {

IWalletAdapter::WalletInitStatus getInitStatus(int _errorCode) {
  switch (_errorCode) {
  case 0:
    return IWalletAdapter::INIT_SUCCESS;
  case CryptoNote::error::WRONG_PASSWORD:
    return IWalletAdapter::INIT_WRONG_PASSWORD;
  case CryptoNote::error::INTERNAL_WALLET_ERROR:
    return IWalletAdapter::INIT_INTERNAL_ERROR;
  default:
    break;
  }

  return IWalletAdapter::INIT_INTERNAL_ERROR;
}

IWalletAdapter::WalletSaveStatus getSaveStatus(int _errorCode) {
  switch (_errorCode) {
  case 0:
    return IWalletAdapter::SAVE_SUCCESS;
  case CryptoNote::error::INTERNAL_WALLET_ERROR:
    return IWalletAdapter::SAVE_ERROR;
  default:
    break;
  }

  return IWalletAdapter::SAVE_ERROR;
}

IWalletAdapter::PasswordStatus getPasswordStatus(int _errorCode) {
  switch (_errorCode) {
  case 0:
    return IWalletAdapter::PASSWORD_SUCCESS;
  default:
    break;
  }

  return IWalletAdapter::PASSWORD_ERROR;
}

IWalletAdapter::SendTransactionStatus getSendStatus(int _errorCode) {
  switch (_errorCode) {
  case 0:
    return IWalletAdapter::SEND_SUCCESS;
  case CryptoNote::error::WRONG_AMOUNT:
    return IWalletAdapter::SEND_WRONG_AMOUNT;
  case CryptoNote::error::MIXIN_COUNT_TOO_BIG:
    return IWalletAdapter::SEND_BIG_MIXIN;
  case CryptoNote::error::TRANSACTION_SIZE_TOO_BIG:
    return IWalletAdapter::SEND_BIG_TRANSACTION_SIZE;
  default:
    break;
  }

  return IWalletAdapter::SEND_INTERNAL_ERROR;
}

}

WalletGreenWorker::WalletGreenWorker(const CryptoNote::Currency& _currency, CryptoNote::INode& _node, Logging::ILogger& _logger, QObject* _parent) :
  QObject(_parent), m_currency(_currency), m_node(_node), m_logger(_logger), m_isOpen(false), m_isEncrypted(false), m_isSaved(false), m_walletSemaphore(1) {
}

WalletGreenWorker::~WalletGreenWorker() {
}

IWalletAdapter::WalletInitStatus WalletGreenWorker::create(const QString& _walletPath, const QString& _password) {
  Q_ASSERT(m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  init();
  QEventLoop waitLoop;
  WalletLogger::info(tr("[Wallet] Generating new wallet..."));
  m_dispatcher->remoteSpawn([this, &_walletPath, &_password, &waitLoop]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    int errorCode = 0;
    try {
      m_wallet->initialize(_walletPath.toStdString(), _password.toStdString());
      m_wallet->createAddress();
    } catch (const std::system_error& _error) {
      WalletLogger::critical(tr("[Wallet] Generate wallet error: %1").arg(_error.code().message().data()));
      errorCode = _error.code().value();
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Generate wallet runtime error: %1").arg(_error.what()));
      errorCode = CryptoNote::error::INTERNAL_WALLET_ERROR;
    }

    waitLoop.exit(errorCode);
  });

  int result = waitLoop.exec();
  locker.wait();
  WalletLogger::info(tr("[Wallet] Wallet generate status: %1").arg(result));
  if (result == 0) {
    startEventLoop();
    m_isOpen.store(true);
    Q_EMIT walletOpenedSignal();
  }

  return getInitStatus(result);
}

IWalletAdapter::WalletInitStatus WalletGreenWorker::load(const QString& _walletPath, const QString& _password) {
  Q_ASSERT(m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  init();
  QEventLoop waitLoop;
  WalletLogger::info(tr("[Wallet] Loading wallet..."));
  m_dispatcher->remoteSpawn([this, &_walletPath, &_password, &waitLoop]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    int errorCode = 0;
    try {
      std::string userData;
      m_wallet->load(_walletPath.toStdString(), _password.toStdString(), userData);
#if QT_VERSION <  0x050400
      m_userData = QByteArray(userData.data(), userData.size());
#else
      m_userData = QByteArray::fromStdString(userData);
#endif
    } catch (const std::system_error& _error) {
      WalletLogger::critical(tr("[Wallet] Load wallet error: %1").arg(_error.code().message().data()));
      errorCode = _error.code().value();
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Load wallet runtime error: %1").arg(_error.what()));
      errorCode = CryptoNote::error::INTERNAL_WALLET_ERROR;
    }

    waitLoop.exit(errorCode);
  });

  int result = waitLoop.exec();
  locker.wait();
  WalletInitStatus initStatus = getInitStatus(result);
  WalletLogger::info(tr("[Wallet] Wallet load result: %1").arg(result));
  if (initStatus == INIT_SUCCESS) {
    startEventLoop();
    if (!_password.isEmpty()) {
      m_isEncrypted.store(true);
    }

    m_isOpen.store(true);
    Q_EMIT walletOpenedSignal();
  } else {
    Q_EMIT walletOpenErrorSignal(initStatus);
  }

  return initStatus;
}

IWalletAdapter::WalletInitStatus WalletGreenWorker::loadLegacyKeys(const QString& _legacyKeysFile, const QString& _walletPath,
  const QString& _password) {
  Q_ASSERT(m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  init();
  QEventLoop waitLoop;
  WalletLogger::info(tr("[Wallet] Importing keys from file..."));
  m_dispatcher->remoteSpawn([this, &_legacyKeysFile, &_walletPath, &_password, &waitLoop]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    int errorCode = 0;
    try {
      std::ofstream outputStream(_walletPath.toStdString(), std::ios::binary | std::ios::trunc);
      CryptoNote::importLegacyKeys(_legacyKeysFile.toStdString(), _password.toStdString(), outputStream);
      outputStream.flush();
      outputStream.close();
      m_wallet->load(_walletPath.toStdString(), _password.toStdString());
    } catch (const std::system_error& _error) {
      WalletLogger::critical(tr("[Wallet] Import keys from file error: %1").arg(_error.code().message().data()));
      errorCode = _error.code().value();
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Import keys from file runtime error: %1").arg(_error.what()));
      errorCode = CryptoNote::error::INTERNAL_WALLET_ERROR;
    }

    waitLoop.exit(errorCode);
  });

  int result = waitLoop.exec();
  locker.wait();
  WalletInitStatus initStatus = getInitStatus(result);
  WalletLogger::info(tr("[Wallet] Import keys from file result: %1").arg(result));
  if (initStatus == INIT_SUCCESS) {
    startEventLoop();
    if (!_password.isEmpty()) {
      m_isEncrypted.store(true);
    }

    m_isOpen.store(true);
    Q_EMIT walletOpenedSignal();
  } else {
    Q_EMIT walletOpenErrorSignal(initStatus);
  }

  return initStatus;
}

IWalletAdapter::WalletInitStatus WalletGreenWorker::createWithKeys(const QString& _walletPath, const AccountKeys& _accountKeys) {
  Q_ASSERT(m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  init();
  QEventLoop waitLoop;
  WalletLogger::info(tr("[Wallet] Importing keys..."));
  m_dispatcher->remoteSpawn([this, &_walletPath, &_accountKeys, &waitLoop]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    int errorCode = 0;
    try {
      m_wallet->initializeWithViewKey(_walletPath.toStdString(), "", _accountKeys.viewKeys.secretKey);
      if (std::memcmp(&_accountKeys.spendKeys.secretKey, &CryptoNote::NULL_SECRET_KEY, sizeof(Crypto::SecretKey)) == 0) {
        m_wallet->createAddress(_accountKeys.spendKeys.publicKey);
      } else {
        m_wallet->createAddress(_accountKeys.spendKeys.secretKey);
      }
    } catch (const std::system_error& _error) {
      WalletLogger::critical(tr("[Wallet] Import keys error: %1").arg(_error.code().message().data()));
      errorCode = _error.code().value();
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Import keys runtime error: %1").arg(_error.what()));
      errorCode = CryptoNote::error::INTERNAL_WALLET_ERROR;
    }

    waitLoop.exit(errorCode);
  });

  int result = waitLoop.exec();
  locker.wait();
  WalletLogger::info(tr("[Wallet] Import keys result: %1").arg(result));
  if (result == 0) {
    startEventLoop();
    m_isOpen.store(true);
    Q_EMIT walletOpenedSignal();
  }

  return getInitStatus(result);
}

IWalletAdapter::WalletSaveStatus WalletGreenWorker::save(CryptoNote::WalletSaveLevel _saveLevel, bool _saveUserData) {
  Q_ASSERT(!m_wallet.isNull());
  if (m_isSaved.load() && _saveLevel == CryptoNote::WalletSaveLevel::SAVE_ALL) {
    WalletLogger::debug(tr("[Wallet] Wallet already saved"));
    return getSaveStatus(0);
  }

  SemaphoreLocker locker(m_walletSemaphore);
  QEventLoop waitLoop;
  WalletLogger::debug(tr("[Wallet] Saving wallet..."));
  m_dispatcher->remoteSpawn([this, _saveLevel, &_saveUserData, &waitLoop]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    int errorCode = 0;
    try {
      if (_saveUserData && !m_userData.isEmpty()) {
#if QT_VERSION < 0x050400
        m_wallet->save(_saveLevel, std::string(m_userData.data(), m_userData.size()));
#else
        m_wallet->save(_saveLevel, m_userData.toStdString());
#endif
      } else {
        m_wallet->save(_saveLevel, std::string());
      }
    } catch (const std::system_error& _error) {
      WalletLogger::critical(tr("[Wallet] Save error: %1").arg(_error.code().message().data()));
      errorCode = _error.code().value();
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Save runtime error: %1").arg(_error.what()));
      errorCode = CryptoNote::error::INTERNAL_WALLET_ERROR;
    }

    waitLoop.exit(errorCode);
  });

  int result = waitLoop.exec();
  locker.wait();
  if (result == 0  && _saveLevel == CryptoNote::WalletSaveLevel::SAVE_ALL) {
    m_isSaved.store(true);
  }

  WalletLogger::debug(tr("[Wallet] Wallet save result: %1").arg(result));
  return getSaveStatus(result);
}

IWalletAdapter::WalletSaveStatus WalletGreenWorker::exportWallet(const QString& _path, bool _encrypt, CryptoNote::WalletSaveLevel _saveLevel, bool _saveUserData) {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  QEventLoop waitLoop;
  WalletLogger::debug(tr("[Wallet] Saving wallet..."));
  m_dispatcher->remoteSpawn([this, &_path, &_encrypt, _saveLevel, &_saveUserData, &waitLoop]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    int errorCode = 0;
    try {
#if QT_VERSION < 0x050400
      m_wallet->exportWallet(_path.toStdString(), _encrypt, _saveLevel, std::string(m_userData.data(), m_userData.size()));
#else
      m_wallet->exportWallet(_path.toStdString(), _encrypt, _saveLevel, m_userData.toStdString());
#endif
    } catch (const std::system_error& _error) {
      WalletLogger::critical(tr("[Wallet] Export error: %1").arg(_error.code().message().data()));
      errorCode = _error.code().value();
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Export runtime error: %1").arg(_error.what()));
      errorCode = CryptoNote::error::INTERNAL_WALLET_ERROR;
    }

    waitLoop.exit(errorCode);
  });

  int result = waitLoop.exec();
  locker.wait();
  WalletLogger::debug(tr("[Wallet] Wallet export result: %1").arg(result));
  return getSaveStatus(result);
}

IWalletAdapter::PasswordStatus WalletGreenWorker::changePassword(const QString& _oldPassword, const QString& _newPassword) {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  int errorCode = 0;
  WalletLogger::info(tr("[Wallet] Changing wallet password"));
  m_dispatcher->remoteSpawn([this, &_oldPassword, &_newPassword, &errorCode]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    try {
      m_wallet->changePassword(_oldPassword.toStdString(), _newPassword.toStdString());
    } catch (const std::system_error& _error) {
      WalletLogger::critical(tr("[Wallet] Change password error: %1").arg(_error.code().message().data()));
      errorCode = _error.code().value();
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Change password runtime error: %1").arg(_error.what()));
      errorCode = CryptoNote::error::INTERNAL_WALLET_ERROR;
    }
  });

  locker.wait();
  WalletLogger::info(tr("[Wallet] Change password result: %1").arg(errorCode));
  if (errorCode == 0) {
    if (!_newPassword.isEmpty()) {
      m_isEncrypted.store(true);
    }

    m_isSaved.store(false);
    Q_EMIT passwordChangedSignal();
  }

  return getPasswordStatus(errorCode);
}

void WalletGreenWorker::close() {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  m_isOpen.store(false);
  m_userData.clear();
  deinit();
  Q_EMIT walletClosedSignal();
  m_isEncrypted.store(false);
  m_isSaved.store(false);
  WalletLogger::info(tr("[Wallet] Wallet closed"));
}

bool WalletGreenWorker::resetPendingTransactions() const {
	
	SemaphoreLocker locker(m_walletSemaphore);
	bool ret = false;
	WalletLogger::debug(tr("[Wallet] Resetting unconfirmed transactions..."));
	m_dispatcher->remoteSpawn([this, &ret]() {
		SemaphoreUnlocker unlocker(m_walletSemaphore);
		try {
			m_wallet->resetPendingTransactions();
			ret = true;
		}
		catch (const std::system_error& _error) {
			WalletLogger::critical(tr("[Wallet] Reset unconfirmed error: %1").arg(_error.code().message().data()));
		}
		catch (const std::exception& _error) {
			WalletLogger::critical(tr("[Wallet] Reset unconfirmed runtime error: %1").arg(_error.what()));
		}
	});

	locker.wait();
	return ret;
}

bool WalletGreenWorker::isOpen() const {
  return m_isOpen.load();
}

bool WalletGreenWorker::isEncrypted() const {
  return m_isEncrypted.load();
}

bool WalletGreenWorker::isTrackingWallet() const {
  AccountKeys accountKeys = getAccountKeys(0);
  QByteArray spendPrivateKey(reinterpret_cast<char*>(&accountKeys.spendKeys.secretKey), sizeof(accountKeys.spendKeys.secretKey));
  QByteArray zeroKey(sizeof(accountKeys.spendKeys.secretKey), 0);
  return spendPrivateKey == zeroKey;
}

QString WalletGreenWorker::getAddress(quintptr _addressIndex) const {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  QString result;
  m_dispatcher->remoteSpawn([this, _addressIndex, &result]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    try {
      Q_ASSERT(_addressIndex < m_wallet->getAddressCount());
      result = QString::fromStdString(m_wallet->getAddress(_addressIndex));
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Get address error: %1").arg(_error.what()));
      result = "";
    }
  });

  locker.wait();
  return result;
}

AccountKeys WalletGreenWorker::getAccountKeys(quintptr _addressIndex) const {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  AccountKeys result;
  m_dispatcher->remoteSpawn([this, _addressIndex, &result]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    try {
      result.viewKeys = m_wallet->getViewKey();
      result.spendKeys = m_wallet->getAddressSpendKey(_addressIndex);
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Get account keys error: %1").arg(_error.what()));
      std::memset(&result, 0, sizeof(result));
    }
  });

  locker.wait();
  return result;
}

quint64 WalletGreenWorker::getActualBalance() const {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  quint64 result;
  m_dispatcher->remoteSpawn([this, &result]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    try {
      result = m_wallet->getActualBalance();
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Get actual balance error: %1").arg(_error.what()));
      result = std::numeric_limits<quint64>::max();
    }
  });

  locker.wait();
  return result;
}

quint64 WalletGreenWorker::getPendingBalance() const {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  quint64 result;
  m_dispatcher->remoteSpawn([this, &result]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    try {
      result = m_wallet->getPendingBalance();
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Get pending balance error: %1").arg(_error.what()));
      result = std::numeric_limits<quint64>::max();
    }
  });

  locker.wait();
  return result;
}

quintptr WalletGreenWorker::getTransactionCount() const {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  quintptr result;
  m_dispatcher->remoteSpawn([this, &result]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    try {
      result = m_wallet->getTransactionCount();
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Get transaction count error: %1").arg(_error.what()));
      result = CryptoNote::WALLET_INVALID_TRANSACTION_ID;
    }
  });

  locker.wait();
  return result;
}

quintptr WalletGreenWorker::getTransactionTransferCount(quintptr _transactionIndex) const {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  quintptr result;
  m_dispatcher->remoteSpawn([this, _transactionIndex, &result]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    try {
      result = m_wallet->getTransactionTransferCount(_transactionIndex);
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Get transaction transfer count error: %1").arg(_error.what()));
      result = CryptoNote::WALLET_INVALID_TRANSFER_ID;
    }
  });

  locker.wait();
  return result;
}

bool WalletGreenWorker::getTransaction(quintptr _transactionIndex, CryptoNote::WalletTransaction& _transaction) const {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  bool result = true;
  m_dispatcher->remoteSpawn([this, _transactionIndex, &_transaction, &result]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    try {
      _transaction = m_wallet->getTransaction(_transactionIndex);
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Get transaction error: %1").arg(_error.what()));
      result = false;
    }
  });

  locker.wait();
  return result;
}

bool WalletGreenWorker::getFullTransactionInfo(quintptr _transactionIndex, FullTransactionInfo& _transactionInfo) const {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  bool result = true;
  m_dispatcher->remoteSpawn([this, _transactionIndex, &_transactionInfo, &result]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    try {
      _transactionInfo.walletTransaction = m_wallet->getTransaction(_transactionIndex);
      _transactionInfo.isFusionTransaction = m_wallet->isFusionTransaction(_transactionIndex);
      quintptr transferCount = m_wallet->getTransactionTransferCount(_transactionIndex);
      for (quintptr transferIndex = 0; transferIndex < transferCount; ++transferIndex) {
        _transactionInfo.transfers.append(m_wallet->getTransactionTransfer(_transactionIndex, transferIndex));
      }
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Get full transaction info error: %1").arg(_error.what()));
      result = false;
    }
  });

  locker.wait();
  return result;
}

bool WalletGreenWorker::getAllTransactions(QHash<quintptr, FullTransactionInfo>& _transactionInfos) const {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  bool result = true;
  m_dispatcher->remoteSpawn([this, &_transactionInfos, &result]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    quintptr transactionCount = m_wallet->getTransactionCount();
    _transactionInfos.reserve(transactionCount);
    try {
      for (quintptr transactionIndex = 0; transactionIndex < transactionCount; ++transactionIndex) {
        FullTransactionInfo transactionInfo;
        transactionInfo.walletTransaction = m_wallet->getTransaction(transactionIndex);
        transactionInfo.isFusionTransaction = m_wallet->isFusionTransaction(transactionIndex);
        quintptr transferCount = m_wallet->getTransactionTransferCount(transactionIndex);
        for (quintptr transferIndex = 0; transferIndex < transferCount; ++transferIndex) {
          transactionInfo.transfers.append(m_wallet->getTransactionTransfer(transactionIndex, transferIndex));
        }

        _transactionInfos.insert(transactionIndex, transactionInfo);
      }
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Get full transaction info range error: %1").arg(_error.what()));
      result = false;
    }
  });

  locker.wait();
  return result;
}

bool WalletGreenWorker::getTransactionTransfer(quintptr _transactionIndex, quintptr _transferIndex,
  CryptoNote::WalletTransfer& _transfer) const {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  bool result = true;
  m_dispatcher->remoteSpawn([this, _transactionIndex, _transferIndex, &_transfer, &result]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    try {
      _transfer = m_wallet->getTransactionTransfer(_transactionIndex, _transferIndex);
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Get transaction transfer error: %1").arg(_error.what()));
      result = false;
    }
  });

  locker.wait();
  return result;
}

bool WalletGreenWorker::isFusionTransaction(quintptr _transactionIndex) const {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  bool result = false;
  m_dispatcher->remoteSpawn([this, _transactionIndex, &result]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    try {
      result = m_wallet->isFusionTransaction(_transactionIndex);
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Is fusion transaction error: %1").arg(_error.what()));
      result = false;
    }
  });

  locker.wait();
  return result;
}

QByteArray WalletGreenWorker::getUserData() const {
  SemaphoreLocker locker(m_walletSemaphore);
  return m_userData;
}

IWalletAdapter::SendTransactionStatus WalletGreenWorker::sendTransaction(const CryptoNote::TransactionParameters& _transactionParameters) {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  int errorCode = 0;
  quintptr newTransactionId = CryptoNote::WALLET_INVALID_TRANSACTION_ID;
  WalletLogger::debug(tr("[Wallet] Sending transaction..."));
  m_dispatcher->remoteSpawn([this, &_transactionParameters, &errorCode, &newTransactionId]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    try {
      newTransactionId = m_wallet->transfer(_transactionParameters);
    } catch (const std::system_error& _error) {
      WalletLogger::critical(tr("[Wallet] Send transaction error: %1").arg(_error.code().message().data()));
      errorCode = _error.code().value();
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Send transaction runtime error: %1").arg(_error.what()));
      errorCode = CryptoNote::error::INTERNAL_WALLET_ERROR;
    }
  });

  locker.wait();
  WalletLogger::info(tr("[Wallet] Transaction send result: %1. New tranaction index=%2.").arg(errorCode).arg(newTransactionId));

  return getSendStatus(errorCode);
}

bool WalletGreenWorker::createFusionTransaction(quint64 _threshold, quint64 _mixin, const QString& _destinationAddress) {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  bool result = true;
  WalletLogger::debug(tr("[Wallet] Creating fusion tranaction..."));
  m_dispatcher->remoteSpawn([this, _threshold, _mixin, &_destinationAddress, &result]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    try {
      m_wallet->createFusionTransaction(_threshold, _mixin, std::vector<std::string>(), _destinationAddress.toStdString());
    } catch (const std::system_error& _error) {
      WalletLogger::critical(tr("[Wallet] Create fusion transaction error: %1").arg(_error.code().message().data()));
      result = false;
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Create fusion transaction runtime error: %1").arg(_error.what()));
      result = false;
    }
  });

  locker.wait();
  return result;
}

quintptr WalletGreenWorker::getOutputsToOptimizeCount(quint64 _threshold) const {
  Q_ASSERT(!m_wallet.isNull());
  SemaphoreLocker locker(m_walletSemaphore);
  CryptoNote::IFusionManager::EstimateResult result;
  m_dispatcher->remoteSpawn([this, _threshold, &result]() {
    SemaphoreUnlocker unlocker(m_walletSemaphore);
    try {
      result = m_wallet->estimate(_threshold);
    } catch (const std::exception& _error) {
      WalletLogger::critical(tr("[Wallet] Get optimization estimate error: %1").arg(_error.what()));
      result = {0, 0};
    }
  });

  locker.wait();
  return result.fusionReadyCount;
}

void WalletGreenWorker::setUserData(const QByteArray& _userData) {
  SemaphoreLocker locker(m_walletSemaphore);
  m_userData = _userData;
}

void WalletGreenWorker::addObserver(IWalletAdapterObserver* _observer) {
  QObject* observerObject = dynamic_cast<QObject*>(_observer);
  m_observerConnections[_observer] << connect(this, SIGNAL(walletOpenedSignal()), observerObject, SLOT(walletOpened()), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(walletOpenErrorSignal(int)), observerObject, SLOT(walletOpenError(int)), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(walletClosedSignal()), observerObject, SLOT(walletClosed()), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(passwordChangedSignal()), observerObject, SLOT(passwordChanged()), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(synchronizationProgressUpdatedSignal(quint32, quint32)), observerObject,
    SLOT(synchronizationProgressUpdated(quint32, quint32)), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(synchronizationCompletedSignal()), observerObject,
    SLOT(synchronizationCompleted()), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(balanceUpdatedSignal(quint64, quint64)), observerObject,
    SLOT(balanceUpdated(quint64, quint64)), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(externalTransactionCreatedSignal(quintptr, FullTransactionInfo)), observerObject,
    SLOT(externalTransactionCreated(quintptr, FullTransactionInfo)), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(transactionUpdatedSignal(quintptr, FullTransactionInfo)), observerObject,
    SLOT(transactionUpdated(quintptr, FullTransactionInfo)), Qt::QueuedConnection);
}

void WalletGreenWorker::removeObserver(IWalletAdapterObserver* _observer) {
  if (!m_observerConnections.contains(_observer) || m_observerConnections.value(_observer).isEmpty()) {
    return;
  }

  for (const auto& connection : m_observerConnections[_observer]) {
    disconnect(connection);
  }

  m_observerConnections[_observer].clear();
}

void WalletGreenWorker::init() {
  QEventLoop waitLoop;
  connect(this, &WalletGreenWorker::initCompletedSignal, &waitLoop, &QEventLoop::quit, Qt::QueuedConnection);
  QMetaObject::invokeMethod(this, "initImpl", Qt::QueuedConnection);
  waitLoop.exec();
}

void WalletGreenWorker::deinit() {
  QEventLoop waitLoop;
  connect(this, &WalletGreenWorker::deinitCompletedSignal, &waitLoop, &QEventLoop::quit, Qt::QueuedConnection);
  deinitImpl();
  waitLoop.exec();
}

void WalletGreenWorker::initImpl() {
  GuardExecutor executor(std::bind(&WalletGreenWorker::createObjects, this),
    std::bind(&WalletGreenWorker::deleteObjects, this));
  m_wallet->start();
  Q_EMIT initCompletedSignal();
  m_exitEvent->wait();
  m_wallet->stop();
  m_dispatcher->yield();
  Q_EMIT deinitCompletedSignal();
}

void WalletGreenWorker::deinitImpl() {
  Q_ASSERT(!m_dispatcher.isNull());
  m_dispatcher->remoteSpawn([this]() {
    m_exitEvent->set();
  });
}

void WalletGreenWorker::createObjects() {
  m_dispatcher.reset(new System::Dispatcher);
  m_exitEvent.reset(new System::Event(*m_dispatcher));
  m_wallet.reset(new CryptoNote::WalletGreen(*m_dispatcher, m_currency, m_node, m_logger));
}

void WalletGreenWorker::deleteObjects() {
  for (auto it = m_observerConnections.begin(); it != m_observerConnections.end(); ++it) {
    removeObserver(it.key());
  }

  m_observerConnections.clear();
  m_wallet.reset();
  m_exitEvent.reset();
  m_dispatcher.reset();
}

void WalletGreenWorker::startEventLoop() {
  m_dispatcher->remoteSpawn([this]() {
    try {
      for (;;) {
        CryptoNote::WalletEvent event = m_wallet->getEvent();
        processEvent(event);
      }
    } catch (...) {
	}
  });

}

void WalletGreenWorker::processEvent(const CryptoNote::WalletEvent& _event) {
  try {
    switch (_event.type) {
    case CryptoNote::WalletEventType::BALANCE_UNLOCKED:
      m_isSaved.store(false);
      WalletLogger::debug(tr("[Wallet] Event: Balance unlocked"));
      Q_EMIT balanceUpdatedSignal(m_wallet->getActualBalance(), m_wallet->getPendingBalance());
      break;
    case CryptoNote::WalletEventType::TRANSACTION_CREATED: {
      m_isSaved.store(false);
      WalletLogger::debug(tr("[Wallet] Event: Transaction created. Tranaction index=%1.").arg(_event.transactionCreated.transactionIndex));
      FullTransactionInfo transaction;
      if (getSyncFullTransactionInfo(_event.transactionCreated.transactionIndex, transaction)) {
        Q_EMIT externalTransactionCreatedSignal(_event.transactionCreated.transactionIndex, transaction);
        Q_EMIT balanceUpdatedSignal(m_wallet->getActualBalance(), m_wallet->getPendingBalance());
      }

      break;
    }
    case CryptoNote::WalletEventType::TRANSACTION_UPDATED: {
      m_isSaved.store(false);
      WalletLogger::debug(tr("[Wallet] Event: Transaction updated. Transaction index=%1.").arg(_event.transactionCreated.transactionIndex));
      FullTransactionInfo transaction;
      if (getSyncFullTransactionInfo(_event.transactionCreated.transactionIndex, transaction)) {
        Q_EMIT transactionUpdatedSignal(_event.transactionUpdated.transactionIndex, transaction);
        Q_EMIT balanceUpdatedSignal(m_wallet->getActualBalance(), m_wallet->getPendingBalance());
      }

      break;
    }
    case CryptoNote::WalletEventType::SYNC_PROGRESS_UPDATED:
      m_isSaved.store(false);
      WalletLogger::debug(tr("[Wallet] Event: Synchronization. Current=%1, Total=%2").arg(_event.synchronizationProgressUpdated.processedBlockCount).
        arg(_event.synchronizationProgressUpdated.totalBlockCount));
      Q_EMIT synchronizationProgressUpdatedSignal(_event.synchronizationProgressUpdated.processedBlockCount,
        _event.synchronizationProgressUpdated.totalBlockCount);
      break;
    case CryptoNote::WalletEventType::SYNC_COMPLETED:
      WalletLogger::debug("[Wallet] Event: Wallet synchronized");
      Q_EMIT synchronizationCompletedSignal();
      break;
    }
  } catch (const std::system_error& _error) {
    WalletLogger::critical(tr("[Wallet] Process event error: %1").arg(_error.code().message().data()));
  } catch (const std::runtime_error& _error) {
    WalletLogger::critical(tr("[Wallet] Process event runtime error: %1").arg(_error.what()));
  }
}

bool WalletGreenWorker::getSyncFullTransactionInfo(quintptr _transactionIndex, FullTransactionInfo& _transactionInfo) const {
  try {
    _transactionInfo.walletTransaction = m_wallet->getTransaction(_transactionIndex);
    _transactionInfo.isFusionTransaction = m_wallet->isFusionTransaction(_transactionIndex);
    quintptr transferCount = m_wallet->getTransactionTransferCount(_transactionIndex);
    for (quintptr transferIndex = 0; transferIndex < transferCount; ++transferIndex) {
      _transactionInfo.transfers.append(m_wallet->getTransactionTransfer(_transactionIndex, transferIndex));
    }
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Wallet] Get full transaction info error: %1").arg(_error.what()));
    return false;
  }

  return true;
}

}
