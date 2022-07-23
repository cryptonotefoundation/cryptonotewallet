// Copyright (c) 2012-2016, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2016-2022, The Karbo developers
//
// This file is part of Karbo.
//
// Karbo is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Karbo is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Karbo.  If not, see <http://www.gnu.org/licenses/>.

#include "Miner.h"

#include <future>
#include <numeric>
#include <sstream>
#include <thread>
#include <QDebug>
#include <QTime>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>
#include <boost/limits.hpp>
#include <boost/utility/value_init.hpp>

#include <CryptoNoteConfig.h>

#include "crypto/crypto.h"
#include "crypto/hash.h"
#include "crypto/random.h"
#include "Common/CommandLine.h"
#include "Common/Math.h"
#include "Common/StringTools.h"
#include "Serialization/SerializationTools.h"
#include "CryptoNoteCore/CryptoNoteTools.h"
#include "CryptoNoteCore/CryptoNoteFormatUtils.h"
#include "CryptoNoteCore/TransactionExtra.h"

#include "CurrencyAdapter.h"
#include "Wallet/WalletRpcServerCommandsDefinitions.h"

#include "NodeAdapter.h"

#include <QThread>
#include <QTimerEvent>

#undef ERROR

using namespace Logging;
using namespace CryptoNote;

namespace WalletGui
{

  Miner::Miner(QObject* _parent, Logging::ILogger &log) :
    QObject(_parent),
    m_logger(log, "Miner"),
    m_stop_mining(true),
    m_template(boost::value_initialized<Block>()),
    m_template_no(0),
    m_diffic(0),
    m_pausers_count(0),
    m_threads_total(0),
    m_starter_nonce(0),
    m_last_hr_merge_time(0),
    m_hashes(0),
    m_do_mining(false),
    m_current_hash_rate(0),
    m_hash_rate(0),
    m_update_block_template_interval(30),
    m_update_merge_hr_interval(2) {
  }
  //-----------------------------------------------------------------------------------------------------
  Miner::~Miner() {
    stop();
  }
  //-----------------------------------------------------------------------------------------------------
  bool Miner::set_block_template(const Block& bl, const difficulty_type& di) {
    std::lock_guard<decltype(m_template_lock)> lk(m_template_lock);

    m_template = bl;

    if (m_template.majorVersion == BLOCK_MAJOR_VERSION_2 || m_template.majorVersion == BLOCK_MAJOR_VERSION_3) {
      CryptoNote::TransactionExtraMergeMiningTag mm_tag;
      mm_tag.depth = 0;
      if (!CryptoNote::get_aux_block_header_hash(m_template, mm_tag.merkleRoot)) {
        return false;
      }

      m_template.parentBlock.baseTransaction.extra.clear();
      if (!CryptoNote::appendMergeMiningTagToExtra(m_template.parentBlock.baseTransaction.extra, mm_tag)) {
        return false;
      }
    }

    m_diffic = di;
    ++m_template_no;
    m_starter_nonce = Random::randomValue<uint32_t>();
    return true;
  }
  //-----------------------------------------------------------------------------------------------------
  bool Miner::on_block_chain_update() {
    if (!is_mining()) {
      return true;
    }

    //pause();

    if (request_block_template()) {
      //resume();
      return true;
    }
    else {
      stop();
    }

    return false;
  }
  //-----------------------------------------------------------------------------------------------------
  bool Miner::request_block_template() {
    qDebug() << "Requesting block template";
    Block bl = boost::value_initialized<Block>();
    CryptoNote::difficulty_type di = 0;
    uint32_t height;
    CryptoNote::BinaryArray extra_nonce;

    if (!NodeAdapter::instance().getBlockTemplate(bl, m_account, extra_nonce, di, height)) {
      m_logger(Logging::ERROR) << "Failed to get_block_template(), stopping mining";
      Q_EMIT minerMessageSignal(QString("Failed to get_block_template()"));
      return false;
    }

    set_block_template(bl, di);

    return true;
  }
  //-----------------------------------------------------------------------------------------------------
  bool Miner::on_idle()
  {
    m_update_block_template_interval.call([&](){
      if (is_mining())
        request_block_template();
      return true;
    });

    m_update_merge_hr_interval.call([&](){
      merge_hr();
      return true;
    });

    return true;
  }
  //-----------------------------------------------------------------------------------------------------
  uint64_t millisecondsSinceEpoch() {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
  }

  //-----------------------------------------------------------------------------------------------------
  void Miner::merge_hr()
  {
    if(m_last_hr_merge_time && is_mining()) {
      m_current_hash_rate = m_hashes * 1000 / (millisecondsSinceEpoch() - m_last_hr_merge_time + 1);
      std::lock_guard<std::mutex> lk(m_last_hash_rates_lock);
      m_last_hash_rates.push_back(m_current_hash_rate);
      if(m_last_hash_rates.size() > 19)
        m_last_hash_rates.pop_front();

      uint64_t total_hr = std::accumulate(m_last_hash_rates.begin(), m_last_hash_rates.end(), static_cast<uint64_t>(0));
      m_hash_rate = static_cast<float>(total_hr) / static_cast<float>(m_last_hash_rates.size());
      qDebug() << "Hashrate: " << m_hash_rate << " H/s";
    }
    
    m_last_hr_merge_time = millisecondsSinceEpoch();
    m_hashes = 0;
  }

  //-----------------------------------------------------------------------------------------------------
  bool Miner::is_mining()
  {
    return !m_stop_mining;
  }
  //-----------------------------------------------------------------------------------------------------
  bool Miner::start(size_t threads_count)
  {   
    if (!m_stop_mining) {
      m_logger(Logging::DEBUGGING) << "Starting miner but it's already started";
      return false;
    }

    std::lock_guard<std::mutex> lk(m_threads_lock);

    if(!m_threads.empty()) {
      m_logger(Logging::DEBUGGING) << "Unable to start miner because there are active mining threads";
      return false;
    }

    if (!WalletAdapter::instance().getAccountKeys(m_account)) {
      m_logger(Logging::ERROR) << "Unable to start miner because couldn't get account keys";
    }

    m_threads_total = static_cast<uint32_t>(threads_count);
    m_starter_nonce = Random::randomValue<uint32_t>();

    // always request block template on start
    if (!request_block_template()) {
      m_logger(Logging::ERROR) << "Unable to start miner because block template request was unsuccessful";
      return false;
    }

    m_stop_mining = false;
    m_pausers_count = 0; // in case mining wasn't resumed after pause

    for (uint32_t i = 0; i != threads_count; i++) {
      m_threads.push_back(std::thread(std::bind(&Miner::worker_thread, this, i)));
    }

    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");

    m_logger(Logging::INFO) << "Mining has started with " << threads_count << " thread(s), at difficulty " << m_diffic << " good luck!";
    Q_EMIT minerMessageSignal(QString("%1 Mining has started with %2 thread(s) at difficulty %3, good luck!").arg(formattedTime).arg(threads_count).arg(m_diffic));
    return true;
  }
  
  //-----------------------------------------------------------------------------------------------------
  double Miner::get_speed()
  {
    if(is_mining())
      return m_hash_rate.load();
    else
      return 0;
  }
  
  //-----------------------------------------------------------------------------------------------------
  void Miner::send_stop_signal() 
  {
    m_stop_mining = true;
  }

  //-----------------------------------------------------------------------------------------------------
  bool Miner::stop()
  {
    send_stop_signal();

    int threadsCount = m_threads.size();

    std::lock_guard<std::mutex> lk(m_threads_lock);

    for (auto& th : m_threads) {
      th.detach();
    }

    m_threads.clear();

    m_current_hash_rate = 0;
    m_hash_rate = 0;
    m_last_hash_rates.clear();

    m_logger(Logging::INFO) << "Mining stopped, " << m_threads.size() << " threads finished" ;
    Q_EMIT minerMessageSignal(QString("Mining stopped, %1 threads finished").arg(threadsCount));

    return true;
  }
  //-----------------------------------------------------------------------------------------------------
  void Miner::on_synchronized()
  {
    if(m_do_mining) {
      start(m_threads_total);
    }
  }
  //-----------------------------------------------------------------------------------------------------
  void Miner::pause()
  {
    std::lock_guard<std::mutex> lk(m_miners_count_lock);
    ++m_pausers_count;
    if(m_pausers_count == 1 && is_mining())
      qDebug() << "MINING PAUSED";
  }
  //-----------------------------------------------------------------------------------------------------
  void Miner::resume()
  {
    std::lock_guard<std::mutex> lk(m_miners_count_lock);
    --m_pausers_count;
    if(m_pausers_count < 0)
    {
      m_pausers_count = 0;
      m_logger(Logging::DEBUGGING) << "Unexpected Miner::resume() called";
      //Q_EMIT minerMessageSignal(QString("Unexpected Miner::resume() called"));
    }
    if(!m_pausers_count && is_mining())
      qDebug() << "MINING RESUMED";
      //Q_EMIT minerMessageSignal(QString("MINING RESUMED"));
  }
  //-----------------------------------------------------------------------------------------------------
  bool Miner::worker_thread(uint32_t th_local_index)
  {
    m_logger(Logging::DEBUGGING) << "Miner thread was started ["<< th_local_index << "]";
    uint32_t nonce = m_starter_nonce + th_local_index;
    difficulty_type local_diff = 0;
    uint32_t local_template_ver = 0;
    Crypto::cn_context context;
    Block b;

    while(!m_stop_mining)
    {
      if(m_pausers_count) //anti split workaround
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        continue;
      }

      if(local_template_ver != m_template_no) {
        std::unique_lock<std::mutex> lk(m_template_lock);
        b = m_template;
        local_diff = m_diffic;
        lk.unlock();

        local_template_ver = m_template_no;
        nonce = m_starter_nonce + th_local_index;
      }

      if(!local_template_ver) //no any set_block_template call
      {
        m_logger(Logging::DEBUGGING) << "Block template not set yet";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        continue;
      }

      b.nonce = nonce;

      // step 1: sing the block
      if (b.majorVersion >= CryptoNote::BLOCK_MAJOR_VERSION_5) {
        BinaryArray ba;
        if (!get_block_hashing_blob(b, ba)) {
          m_logger(Logging::ERROR) << "get_block_hashing_blob for signature failed.";
          Q_EMIT minerMessageSignal(QString("get_block_hashing_blob for signature failed"));
          m_stop_mining = true;
        }

        Crypto::Hash h = Crypto::cn_fast_hash(ba.data(), ba.size());
        try {
          Crypto::PublicKey txPublicKey = getTransactionPublicKeyFromExtra(b.baseTransaction.extra);
          Crypto::KeyDerivation derivation;
          if (!Crypto::generate_key_derivation(txPublicKey, m_account.viewSecretKey, derivation)) {
            m_logger(Logging::ERROR) << "Failed to generate_key_derivation for block signature";
            Q_EMIT minerMessageSignal(QString("Failed to generate_key_derivation for block signature"));
            m_stop_mining = true;
          }
          Crypto::SecretKey ephSecKey;
          Crypto::derive_secret_key(derivation, 0, m_account.spendSecretKey, ephSecKey);
          Crypto::PublicKey ephPubKey = boost::get<KeyOutput>(b.baseTransaction.outputs[0].target).key;

          Crypto::generate_signature(h, ephPubKey, ephSecKey, b.signature);
        }
        catch (std::exception& e) {
          m_logger(Logging::ERROR) << "Signing block failed: " << e.what();
          Q_EMIT minerMessageSignal(QString("Signing block failed") + QString(e.what()));
          m_stop_mining = true;
        }
      }

      // step 2: get long hash
      Crypto::Hash pow;

      if (!m_stop_mining) {
        if (!NodeAdapter::instance().getBlockLongHash(context, b, pow)) {
          m_logger(Logging::ERROR) << "getBlockLongHash failed.";
          Q_EMIT minerMessageSignal(QString("getBlockLongHash failed"));
          m_stop_mining = true;
        }
      }

      if (!m_stop_mining && check_hash(pow, local_diff))
      {
        // we lucky!

        //pause();

        Crypto::Hash id;
        if (!get_block_hash(b, id)) {
          m_logger(Logging::ERROR) << "Failed to get block hash.";
          Q_EMIT minerMessageSignal(QString("Failed to get block hash"));
          m_stop_mining = true;
        }
        uint32_t bh = boost::get<BaseInput>(b.baseTransaction.inputs[0]).blockIndex;

        QDateTime date = QDateTime::currentDateTime();
        QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");

        m_logger(Logging::INFO) << "Found block " << Common::podToHex(id) << " at height " << bh << " for difficulty: " << local_diff << ", POW " << Common::podToHex(pow);
        Q_EMIT minerMessageSignal(QString("%1 Found block %2 at height %3 for difficulty %4, POW %5").arg(formattedTime).arg(QString::fromStdString(Common::podToHex(id))).arg(bh).arg(local_diff).arg(QString::fromStdString(Common::podToHex(pow))));

        if(!NodeAdapter::instance().handleBlockFound(b)) {
          m_logger(Logging::ERROR) << "Failed to submit block";
          Q_EMIT minerMessageSignal(QString("Failed to submit block"));
        } else {
          // yay!
        }
      }

      nonce += m_threads_total;
      ++m_hashes;
    }
    m_logger(Logging::DEBUGGING) << "Miner thread stopped ["<< th_local_index << "]";
    return true;
  }
}
