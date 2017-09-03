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

#include <QDir>
#include <QFileInfo>
#include <QObject>

namespace WalletGui {

class BlockchainInstaller : public QObject {
  Q_OBJECT

public:
  explicit BlockchainInstaller(QObject* _parent);
  ~BlockchainInstaller();

  void exec();

private:
  const QString m_blockIndexesFileName;
  const QString m_blocksFileName;
  const QDir m_intensecoinDir;
  const QDir m_applicationDir;

  bool checkIfCurrentBlockchainExists() const;
  bool checkIfIntensecoinBlockchainExists() const;
  bool getGenesisBlockFromBlockchain(char** _genesisBlockData) const;
  bool checkIfBlockchainOutdated(quint64& _current, quint64& _intensecoin) const;
  QFileInfo currentBlockchainInfo() const;
  QFileInfo intensecoinBlockchainInfo() const;

  void copyProgress(quint64 _copied, quint64 _total);
  void installBlockchain();

Q_SIGNALS:
  void copyFileSignal(const QString& _sourcePath, const QString& _targetPath);
  void showMessageSignal(const QString& _message);
};

class AsyncFileProcessor : public QObject {
  Q_OBJECT

public:
  AsyncFileProcessor();
  ~AsyncFileProcessor();

  void copy(const QString& _sourcePath, const QString& _targetPath);

Q_SIGNALS:
  void copyProgressSignal(quint64 _copied, quint64 _total);
  void copyFinished();
  void errorSignal(int _error);
};

}
