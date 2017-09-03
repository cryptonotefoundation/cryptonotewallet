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

class QAbstractItemModel;

namespace Ui {
  class OptionsDialog;
}

namespace WalletGui {

class ICryptoNoteAdapter;
class IDonationManager;
class IOptimizationManager;

class OptionsDialog : public QDialog {
  Q_OBJECT
  Q_DISABLE_COPY(OptionsDialog)

public:
  OptionsDialog(ICryptoNoteAdapter* _cryptoNoteAdapter, IDonationManager* _donationManager,
    IOptimizationManager* _optimizationManager, QAbstractItemModel* _addressBookModel, QWidget* _parent);
  ~OptionsDialog();

  void setDonationAddress(const QString& _label, const QString& _address);
  bool needToRestartApplication() const;

  Q_SLOT void accept() override;

private:
  QScopedPointer<Ui::OptionsDialog> m_ui;
  bool m_needToRestart;

  Q_SLOT void showRestartWarning(bool _show);
  Q_SLOT void disableAcceptButton(bool _disable);
};

}
