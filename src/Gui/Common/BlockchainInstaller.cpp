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

#include <QApplication>
#include <QDir>
#include <QEventLoop>
#include <QFileInfo>
#include <QThread>

#include <cmath>
#include <cstring>

#include <Common/Util.h>

#include "BlockchainInstaller.h"
#include "Gui/Common/QuestionDialog.h"
#include "Settings/Settings.h"

#include "ui_QuestionDialog.h"

namespace WalletGui {

namespace {

const char ETHALON_GENESIS_BLOCK_STRING[] =
  "0100000000000000000000000000000000000000000000000000000000000000000000460000000"
  "10A01FF0001FFFFFFFFFFFF0F029B2E4C0281C0B02E7C53291A94D1D0CBFF8883F8024F5142EE49"
  "4FFBBD08807121013C086A48C15FB637A96991BC6D53CAF77068B5BA6EEB3C82357228C49790584"
  "A00005001FFFFFFFFFFFF0F01010A01FF0001FFFFFFFFFFFF0F029B2E4C0281C0B02E7C53291A94"
  "D1D0CBFF8883F8024F5142EE494FFBBD08807121013C086A48C15FB637A96991BC6D53CAF77068B"
  "5BA6EEB3C82357228C49790584A0100000000000000000000000000000000000000000000000000"
  "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
  "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
  "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
  "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
  "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
  "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
  "0000000000000000000000000000000000000000000000000000000000000000000000000000";

const quint32 BYTECOIN_BLOCK_SIZE = 0xd5;

}

BlockchainInstaller::BlockchainInstaller(QObject* _parent) : QObject(_parent), m_blockIndexesFileName("blockindexes.dat"), m_blocksFileName("blocks.dat"),
  m_intensecoinDir(Settings::instance().getDataDir().absolutePath()), m_applicationDir(QDir::current()) {
}

BlockchainInstaller::~BlockchainInstaller() {
}

void BlockchainInstaller::exec() {
  if (!checkIfCurrentBlockchainExists()) {
    return;
  }

  if (!checkIfIntensecoinBlockchainExists()) {
    installBlockchain();
    return;
  }

  quint64 currentHeight;
  quint64 intensecoinHeight;
  if (!checkIfBlockchainOutdated(currentHeight, intensecoinHeight)) {
    return;
  }

  QString questionStringTemplate = tr("Would you like to replace your current blockchain (height: %1)\nwith the one in your GUI wallet folder (height: %2)?");

  QuestionDialog dlg(tr("Blockchain installation"), QString(questionStringTemplate).arg(intensecoinHeight).arg(currentHeight), nullptr);
  if (dlg.exec() == QDialog::Accepted) {
    installBlockchain();
  }
}

bool BlockchainInstaller::checkIfCurrentBlockchainExists() const {
  char *genesisBlockData = nullptr;
  if (!getGenesisBlockFromBlockchain(&genesisBlockData)) {
    return false;
  }

  QByteArray ethalonGenesisBlock = QByteArray::fromHex(ETHALON_GENESIS_BLOCK_STRING);
  if (std::memcmp(ethalonGenesisBlock.data(), genesisBlockData, ethalonGenesisBlock.size())) {
    return false;
  }

  return true;
}

bool BlockchainInstaller::getGenesisBlockFromBlockchain(char** _genesisBlockData) const {
  if (!m_applicationDir.exists(m_blocksFileName) || !m_applicationDir.exists(m_blockIndexesFileName)) {
    return false;
  }

  QFile blockIndexesFile(m_applicationDir.absoluteFilePath(m_blockIndexesFileName));
  QFile blocksFile(m_applicationDir.absoluteFilePath(m_blocksFileName));
  if (!blockIndexesFile.open(QIODevice::ReadOnly) || !blocksFile.open(QIODevice::ReadOnly)) {
    return false;
  }

  QDataStream blockIndexesFileStream(&blockIndexesFile);

  blockIndexesFileStream.setByteOrder(QDataStream::LittleEndian);
  quint32 begin;
  quint32 offset;
  blockIndexesFileStream.skipRawData(sizeof(quint32));
  blockIndexesFileStream >> begin >> offset;
  quint32 blockSize = offset - begin;
  if (blockSize != BYTECOIN_BLOCK_SIZE) {
    return false;
  }

  QDataStream blocksFileStream(&blocksFile);
  blocksFileStream.setByteOrder(QDataStream::LittleEndian);
  *_genesisBlockData = new char[blockSize];
  if (blocksFileStream.readRawData(*_genesisBlockData, blockSize) != blockSize) {
    delete[] *_genesisBlockData;
    *_genesisBlockData = nullptr;
    return false;
  }
  return true;
}

bool BlockchainInstaller::checkIfIntensecoinBlockchainExists() const {
  return m_intensecoinDir.exists() && m_intensecoinDir.exists(m_blocksFileName);
}

bool BlockchainInstaller::checkIfBlockchainOutdated(quint64& _sourceHeight, quint64& _targetHeight) const {
  quint32 sourceHeight(0);
  quint32 targetHeight(0);
  QFile sourceBlockIndexesFile(m_applicationDir.absoluteFilePath(m_blockIndexesFileName));
  QFile targetBlockIndexesFile(m_intensecoinDir.absoluteFilePath(m_blockIndexesFileName));
  if (!sourceBlockIndexesFile.open(QIODevice::ReadOnly) || !targetBlockIndexesFile.open(QIODevice::ReadOnly)) {
    return false;
  }

  QDataStream sourceBlockIndexesFileStream(&sourceBlockIndexesFile);
  QDataStream targetBlockIndexesFileStream(&targetBlockIndexesFile);
  sourceBlockIndexesFileStream.setByteOrder(QDataStream::LittleEndian);
  targetBlockIndexesFileStream.setByteOrder(QDataStream::LittleEndian);
  sourceBlockIndexesFileStream >> sourceHeight;
  targetBlockIndexesFileStream >> targetHeight;
  _sourceHeight = sourceHeight - 1;
  _targetHeight = targetHeight - 1;
  return targetHeight < sourceHeight;
}

QFileInfo BlockchainInstaller::currentBlockchainInfo() const {
  return QFileInfo(m_applicationDir.absoluteFilePath(m_blocksFileName));
}

QFileInfo BlockchainInstaller::intensecoinBlockchainInfo() const {
  return QFileInfo(m_intensecoinDir.absoluteFilePath(m_blocksFileName));
}

void BlockchainInstaller::copyProgress(quint64 _copied, quint64 _total) {
  Q_EMIT showMessageSignal(QString(tr("Copying blockchain files... (%1%)")).arg((qreal)_copied / _total * 100, 0, 'f', 0));
}

void BlockchainInstaller::installBlockchain() {
  Q_EMIT showMessageSignal(tr("Copying blockchain files..."));
  m_intensecoinDir.mkpath(".");
  QThread workerThread;
  AsyncFileProcessor fp;
  fp.moveToThread(&workerThread);
  connect(this, &BlockchainInstaller::copyFileSignal, &fp, &AsyncFileProcessor::copy, Qt::QueuedConnection);
  connect(&fp, &AsyncFileProcessor::copyProgressSignal, this, &BlockchainInstaller::copyProgress);
  workerThread.start();
  QEventLoop waitLoop;
  connect(&fp, &AsyncFileProcessor::copyFinished, &waitLoop, &QEventLoop::quit);
  connect(&fp, &AsyncFileProcessor::errorSignal, &waitLoop, &QEventLoop::exit);

  Q_EMIT copyFileSignal(m_applicationDir.absoluteFilePath(m_blockIndexesFileName),
    m_intensecoinDir.absoluteFilePath(m_blockIndexesFileName));
  if (waitLoop.exec() != 0) {
    workerThread.quit();
    workerThread.wait();
    return;
  }

  Q_EMIT copyFileSignal(m_applicationDir.absoluteFilePath(m_blocksFileName), m_intensecoinDir.absoluteFilePath(m_blocksFileName));
  if (waitLoop.exec() != 0) {
    workerThread.quit();
    workerThread.wait();
    return;
  }

  workerThread.quit();
  workerThread.wait();
}

AsyncFileProcessor::AsyncFileProcessor() : QObject() {
}

AsyncFileProcessor::~AsyncFileProcessor() {
}

void AsyncFileProcessor::copy(const QString& _sourcePath, const QString& _targetPath) {
  quint64 copiedBytes = 0;
  QFile sourceFile(_sourcePath);
  QFile targetFile(_targetPath);

  const quint64 buffer_size = 0x100000;

  QScopedArrayPointer<char> buffer(new char[buffer_size]);
  const quint64 file_size = sourceFile.size();

  if (!sourceFile.open(QIODevice::ReadOnly)) {
    Q_EMIT errorSignal(sourceFile.error());
    return;
  }

  if (!targetFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    Q_EMIT errorSignal(targetFile.error());
    return;
  }

  qint64 readBytes = -1;
  for (readBytes = sourceFile.read(buffer.data(), buffer_size); readBytes > 0; readBytes = sourceFile.read(buffer.data(), buffer_size)) {
    if (targetFile.write(buffer.data(), readBytes) == -1) {
      Q_EMIT errorSignal(targetFile.error());
      return;
    }

    copiedBytes += readBytes;
    Q_EMIT copyProgressSignal(copiedBytes, file_size);
    QApplication::processEvents();
  }

  if (readBytes == -1) {
    Q_EMIT errorSignal(sourceFile.error());
    return;
  }

  Q_EMIT copyFinished();
}

}
