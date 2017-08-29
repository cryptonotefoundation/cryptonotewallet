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

#include <QFrame>

#include "Application/IWalletUiItem.h"

class QCompleter;

namespace Ui {
  class TransferFrame;
}

namespace WalletGui {

class ICryptoNoteAdapter;

class TransferFrame : public QFrame, public IWalletUiItem {
  Q_OBJECT
  Q_DISABLE_COPY(TransferFrame)

public:
  explicit TransferFrame(QWidget* _parent);
  ~TransferFrame();

  bool readyToSend() const;
  QString getAddress() const;
  QString getAmountString() const;
  QString getLabel() const;

  void setAddress(const QString& _address);
  void setAmount(qreal _amount);
  void setLabel(const QString& _label);

  void disableRemoveButton(bool _disable);
  void hideBorder();

  void setAddressError(bool _error = true);
  void setDuplicationError(bool _error = true);
  void setAmountFormatError(bool _error = true);
  void setInsufficientFundsError(bool _error = true);
  void setBigTransactionError(bool _error = true);
  void setLabelLikePaymentIdError(bool _error = true);

  // QObject
  virtual bool eventFilter(QObject* _object, QEvent* _event) override;

  // IWalletUiItem
  virtual void setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) override;
  virtual void setAddressBookManager(IAddressBookManager* _addressBookManager) override;
  virtual void setMainWindow(QWidget* _mainWindow) override;
  virtual void setAddressBookModel(QAbstractItemModel* _model) override;

private:
  QScopedPointer<Ui::TransferFrame> m_ui;
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  IAddressBookManager* m_addressBookManager;
  QWidget* m_mainWindow;
  QAbstractItemModel* m_addressBookModel;
  QCompleter* m_addressCompleter;

  void validateAmount(double _amount);

  Q_SLOT void amountStringChanged(const QString& _amountString);
  Q_SLOT void addressChanged(const QString& _address);
  Q_SLOT void labelOrAddressChanged(const QString& _text);
  Q_SLOT void addressBookClicked();
  Q_SLOT void pasteClicked();

Q_SIGNALS:
  void amountStringChangedSignal(const QString& _amountString);
  void addressChangedSignal(const QString& _address);
};

}
