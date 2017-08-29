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

#include <QSemaphore>

#include "SemaphoreUtils.h"

namespace WalletGui {

SemaphoreLocker::SemaphoreLocker(QSemaphore& _semaphore) : m_semaphore(_semaphore) {
  m_semaphore.acquire(1);
}

SemaphoreLocker::~SemaphoreLocker() {
  Q_ASSERT(m_semaphore.available() == 0);
  m_semaphore.release(1);
}

void SemaphoreLocker::wait() {
  m_semaphore.acquire(1);
}

SemaphoreUnlocker::SemaphoreUnlocker(QSemaphore& _semaphore) : m_semaphore(_semaphore) {
  Q_ASSERT(m_semaphore.available() == 0);
}

SemaphoreUnlocker::~SemaphoreUnlocker() {
  Q_ASSERT(m_semaphore.available() == 0);
  m_semaphore.release(1);
}

}
