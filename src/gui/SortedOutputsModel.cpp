// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2021 Karbo developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDateTime>

#include "SortedOutputsModel.h"
#include "OutputsModel.h"

namespace WalletGui {

SortedOutputsModel& SortedOutputsModel::instance() {
  static SortedOutputsModel inst;
  return inst;
}

SortedOutputsModel::SortedOutputsModel() : QSortFilterProxyModel() {
  setSourceModel(&OutputsModel::instance());
  setDynamicSortFilter(true);
  sort(OutputsModel::COLUMN_GLOBAL_OUTPUT_INDEX, Qt::DescendingOrder);
}

SortedOutputsModel::~SortedOutputsModel() {
}

bool SortedOutputsModel::filterAcceptsRow(int _row, const QModelIndex &_parent) const {
  QModelIndex _index = sourceModel()->index(_row, 0, _parent);

  int state = _index.data(OutputsModel::ROLE_STATE).value<quint8>();

  if(m_selectedState != -1) {
    if(state != m_selectedState)
      return false;
  }

  return (sourceModel()->data(sourceModel()->index(_row, 2, _parent)).toString().contains(m_searchString,Qt::CaseInsensitive)
       || sourceModel()->data(sourceModel()->index(_row, 3, _parent)).toString().contains(m_searchString,Qt::CaseInsensitive)
       || sourceModel()->data(sourceModel()->index(_row, 4, _parent)).toString().contains(m_searchString,Qt::CaseInsensitive)
       || sourceModel()->data(sourceModel()->index(_row, 5, _parent)).toString().contains(m_searchString,Qt::CaseInsensitive)
  );

  return true;
}

void SortedOutputsModel::setSearchFor(const QString &searchString) {
  this->m_searchString = searchString;
  invalidateFilter();
}

void SortedOutputsModel::setState(const int state) {
  this->m_selectedState = state;
  invalidateFilter();
}

}
