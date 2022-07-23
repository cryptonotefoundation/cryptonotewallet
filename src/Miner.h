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

#pragma once

#include <QObject>
#include <QReadWriteLock>

#include <atomic>
#include <list>
#include <mutex>
#include <thread>

#include "CryptoNoteCore/CryptoNoteBasic.h"
#include "CryptoNoteCore/Currency.h"
#include "CryptoNoteCore/Difficulty.h"
#include "CryptoNoteCore/OnceInInterval.h"
#include "Logging/LoggerRef.h"
#include "Serialization/ISerializer.h"
#include "WalletAdapter.h"

using namespace CryptoNote;

namespace WalletGui {
  class Miner : public QObject {
    Q_OBJECT

  public:
    Miner(QObject* _parent, Logging::ILogger& log);
    ~Miner();

    bool set_block_template(const Block& bl, const difficulty_type& diffic);
    bool on_block_chain_update();
    bool start(size_t threads_count);
    double get_speed();
    void send_stop_signal();
    bool stop();
    bool is_mining();
    bool on_idle();
    void on_synchronized();
    void pause();
    void resume();
    void merge_hr();

  private:
    bool worker_thread(uint32_t th_local_index);
    bool request_block_template();

    struct miner_config
    {
      uint64_t current_extra_message_index;
      void serialize(ISerializer& s) {
        KV_MEMBER(current_extra_message_index)
      }
    };

    std::atomic<bool> m_stop_mining;
    std::mutex m_template_lock;
    Block m_template;
    std::atomic<uint32_t> m_template_no;
    std::atomic<uint32_t> m_starter_nonce;
    difficulty_type m_diffic;

    std::atomic<uint32_t> m_threads_total;
    std::atomic<int32_t> m_pausers_count;
    std::mutex m_miners_count_lock;

    std::list<std::thread> m_threads;
    std::mutex m_threads_lock;
    AccountKeys m_account;
    OnceInInterval m_update_block_template_interval;
    OnceInInterval m_update_merge_hr_interval;

    std::vector<BinaryArray> m_extra_messages;
    miner_config m_config;
    std::string m_config_folder_path;
    std::atomic<uint64_t> m_last_hr_merge_time;
    std::atomic<uint64_t> m_hashes;
    std::atomic<uint64_t> m_current_hash_rate;
    std::atomic<double> m_hash_rate;
    std::mutex m_last_hash_rates_lock;
    std::list<uint64_t> m_last_hash_rates;
    bool m_do_mining;

    Logging::LoggerRef m_logger;

  Q_SIGNALS:
    void minerMessageSignal(const QString& _message);

  };
}
