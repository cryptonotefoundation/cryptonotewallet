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

#include <QDialog>

class QPersistentModelIndex;

namespace Ui {
class NewAddressDialog;
}

namespace WalletGui {

class IAddressBookManager;
class IDonationManager;
class ICryptoNoteAdapter;

class NewAddressDialog : public QDialog {
  Q_OBJECT
  Q_DISABLE_COPY(NewAddressDialog)

public:
  NewAddressDialog(ICryptoNoteAdapter* _cryptoNoteAdapter, IAddressBookManager* _addressBookManager,
    IDonationManager* _donationManager, QWidget* _parent);
  NewAddressDialog(ICryptoNoteAdapter* _cryptoNoteAdapter, IAddressBookManager* _addressBookManager,
    IDonationManager* _donationManager, const QPersistentModelIndex& _index, QWidget* _parent);
  ~NewAddressDialog();

  QString getAddress() const;
  QString getLabel() const;
  bool hasDonationFlag() const;

  void setAddressError(bool _error);
  void setAddressDuplicationError(bool _error);
  void setLabelDuplicationError(bool _error);

private:
  QScopedPointer<Ui::NewAddressDialog> m_ui;
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  IAddressBookManager* m_addressBookManager;
  IDonationManager* m_donationManager;

  bool checkForErrors() const;

  Q_SLOT void validateAddress(const QString& _address);
  Q_SLOT void validateLabel(const QString& _label);
  Q_SLOT void donationCheckStateChanged(int _checkState);
};

}
