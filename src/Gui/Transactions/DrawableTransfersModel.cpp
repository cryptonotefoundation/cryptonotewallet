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

#include "DrawableTransfersModel.h"
#include "Settings/Settings.h"
#include "IAddressBookManager.h"
#include "ICryptoNoteAdapter.h"
#include "Models/TransfersModel.h"
#include "Style/Style.h"

#include "IWallet.h"

namespace WalletGui {

DrawableTransfersModel::DrawableTransfersModel(ICryptoNoteAdapter* _cryptoNoteAdapter, IAddressBookManager* _addressBookManager, const QString& _walletAddress,
  QObject* _parent) : QAbstractProxyModel(_parent), m_cryptoNoteAdapter(_cryptoNoteAdapter), m_addressBookManager(_addressBookManager),
  m_walletAddress(_walletAddress), m_showDetails(false) {
}

DrawableTransfersModel::~DrawableTransfersModel() {
}

void DrawableTransfersModel::showDetails(bool _show) {
  beginResetModel();
  m_showDetails = _show;
  endResetModel();
}

Qt::ItemFlags DrawableTransfersModel::flags(const QModelIndex& _index) const {
  if (!m_showDetails) {
    return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsSelectable;
  }

  return QAbstractProxyModel::flags(_index);
}

int DrawableTransfersModel::columnCount(const QModelIndex& _parent) const {
  return sourceModel()->columnCount(_parent);
}

int DrawableTransfersModel::rowCount(const QModelIndex& _parent) const {
  if (!m_showDetails) {
    return 1;
  }

  return sourceModel()->rowCount();
}

QVariant DrawableTransfersModel::data(const QModelIndex& _proxyIndex, int _role) const {
  if (m_showDetails) {
  switch (_role) {
    case Qt::DisplayRole:
      return getDisplayRole(_proxyIndex);
    case Qt::DecorationRole:
      return getDecorationRole(_proxyIndex);
    case Qt::TextColorRole:
      return getTextColorRole(_proxyIndex);

    default:
      break;
    }

    return QAbstractProxyModel::data(_proxyIndex, _role);
  } else {
    switch (_role) {
    case Qt::DisplayRole:
      return getAggregatedDisplayRole(_proxyIndex);
    case Qt::TextAlignmentRole:
      return getAggregateTextAlignmentRole(_proxyIndex);
    case Qt::TextColorRole:
      return getAggregateTextColorRole(_proxyIndex);
    default:
      break;
    }
  }

  return QVariant();
}

QModelIndex DrawableTransfersModel::index(int _row, int _column, const QModelIndex& _parent) const {
  if (!hasIndex(_row, _column, _parent)) {
    return QModelIndex();
  }

  return createIndex(_row, _column, _row);
}

QModelIndex DrawableTransfersModel::parent(const QModelIndex& _index) const {
  return QModelIndex();
}

QModelIndex DrawableTransfersModel::mapFromSource(const QModelIndex& _sourceIndex) const {
  if (m_showDetails && _sourceIndex.isValid()) {
    return index(_sourceIndex.row(), _sourceIndex.column(), _sourceIndex.parent());
  }

  if (!m_showDetails) {
    return createIndex(0, 0, quintptr(0));
  }

  return QModelIndex();
}

QModelIndex DrawableTransfersModel::mapToSource(const QModelIndex& _proxyIndex) const {
  if (m_showDetails && _proxyIndex.isValid()) {
    return sourceModel()->index(_proxyIndex.row(), _proxyIndex.column(), _proxyIndex.parent());
  }

  return QModelIndex();
}

void DrawableTransfersModel::setSourceModel(QAbstractItemModel* _sourceModel) {
  QAbstractProxyModel::setSourceModel(_sourceModel);
  if (_sourceModel->rowCount() == 1) {
    m_showDetails = true;
  }
}

QVariant DrawableTransfersModel::getDisplayRole(const QModelIndex& _index) const {
  if (_index.column() == TransfersModel::COLUMN_ADDRESS) {
    if (_index.data(TransfersModel::ROLE_TYPE).toInt() == static_cast<int>(CryptoNote::WalletTransferType::CHANGE)) {
      return tr("Change");
    }

    QString address = mapToSource(_index).data().toString();
    if (address.isEmpty()) {
      return tr("Unknown");
    }

    return address;
  }

  return mapToSource(_index).data();
}

QVariant DrawableTransfersModel::getDecorationRole(const QModelIndex& _index) const {
  if (_index.column() == TransfersModel::COLUMN_ADDRESS) {
    bool isDonationTransfer = _index.data(TransfersModel::ROLE_IS_DONATION_TRANSFER).toBool();
    if (isDonationTransfer) {
      return QPixmap(":icons/donation_small");
    }

    return QAbstractProxyModel::data(_index, Qt::DecorationRole);
  }

  return QVariant();
}

QVariant DrawableTransfersModel::getTextColorRole(const QModelIndex& _index) const {
  if (_index.column() == TransfersModel::COLUMN_AMOUNT) {
    qint64 amount = _index.data(TransfersModel::ROLE_AMOUNT).value<qint64>();
    if (amount < 0) {
      return QColor(Settings::instance().getCurrentStyle().fontColorRed());
    } else if (amount > 0) {
      return QColor(Settings::instance().getCurrentStyle().fontColorGreen());
    }
  }

  return QVariant();
}

QVariant DrawableTransfersModel::getAggregatedDisplayRole(const QModelIndex& _index) const {
  switch (_index.column()) {
  case TransfersModel::COLUMN_ADDRESS:
    return tr("Transaction contains %1 transfers").arg(sourceModel()->rowCount());
  case TransfersModel::COLUMN_AMOUNT:
    return m_cryptoNoteAdapter->formatAmountToShort(getAmountSum());
  default:
    break;
  }

  return QVariant();
}

QVariant DrawableTransfersModel::getAggregateTextAlignmentRole(const QModelIndex& _index) const {
  return headerData(_index.column(), Qt::Horizontal, Qt::TextAlignmentRole);
}

QVariant DrawableTransfersModel::getAggregateTextColorRole(const QModelIndex& _index) const {
  if (_index.column() == TransfersModel::COLUMN_AMOUNT) {
    qint64 amount = getAmountSum();
    if (amount < 0) {
      return QColor(Settings::instance().getCurrentStyle().fontColorRed());
    } else if (amount > 0) {
      return QColor(Settings::instance().getCurrentStyle().fontColorGreen());
    }
  } else if (_index.column() == TransfersModel::COLUMN_ADDRESS) {
    return QColor(Settings::instance().getCurrentStyle().fontColorGray());
  }

  return QVariant();
}

qint64 DrawableTransfersModel::getAmountSum() const {
  qint64 result = 0;
  for (int i = 0; i < sourceModel()->rowCount(); ++i) {
    if (!m_walletAddress.compare(sourceModel()->index(i, 0).data(TransfersModel::ROLE_ADDRESS).toString())) {
      result += sourceModel()->index(i, 0).data(TransfersModel::ROLE_AMOUNT).value<qint64>();
    }
  }

  return result;
}

}
