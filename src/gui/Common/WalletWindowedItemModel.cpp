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

#include "WalletWindowedItemModel.h"

namespace WalletGui {

WalletWindowedItemModel::WalletWindowedItemModel(int _filterRole, QObject* _parent) : QSortFilterProxyModel(_parent),
  m_filterRole(_filterRole), m_windowSize(0), m_windowBegin(0) {
}

WalletWindowedItemModel::~WalletWindowedItemModel() {
}

quint32 WalletWindowedItemModel::getWindowSize() const {
  return m_windowSize;
}

quint32 WalletWindowedItemModel::getWindowBegin() const {
  return m_windowBegin;
}

void WalletWindowedItemModel::setWindowSize(quint32 _windowSize) {
  if (m_windowSize != _windowSize) {
    m_windowSize = _windowSize;
    invalidateFilter();
  }
}

void WalletWindowedItemModel::setWindowBegin(quint32 _windowBegin) {
  if (m_windowBegin != _windowBegin) {
    m_windowBegin = _windowBegin;
    invalidateFilter();
  }
}

bool WalletWindowedItemModel::filterAcceptsRow(int _sourceRow, const QModelIndex &_sourceParent) const {
  if (getWindowSize() == 0) {
    return false;
  }

  if (m_filterRole == -1) {
    return _sourceRow >= m_windowBegin && _sourceRow < m_windowBegin + m_windowSize;
  }

  QModelIndex sourceIndex = sourceModel()->index(_sourceRow, 0, _sourceParent);
  return sourceIndex.data(m_filterRole).toUInt() >= m_windowBegin &&
    sourceIndex.data(m_filterRole).toUInt() < m_windowBegin + m_windowSize;
}

}
