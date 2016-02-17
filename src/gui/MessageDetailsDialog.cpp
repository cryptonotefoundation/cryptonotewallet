// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QFileDialog>
#include <QMessageBox>

#include "MessageDetailsDialog.h"
#include "MainWindow.h"
#include "MessagesModel.h"

#include "ui_messagedetailsdialog.h"

namespace WalletGui {

MessageDetailsDialog::MessageDetailsDialog(const QModelIndex& _index, QWidget* _parent) : QDialog(_parent),
  m_ui(new Ui::MessageDetailsDialog) {
  m_ui->setupUi(this);
  QModelIndex modelIndex = MessagesModel::instance().index(_index.data(MessagesModel::ROLE_ROW).toInt(), 0);
  m_dataMapper.setModel(&MessagesModel::instance());
  m_dataMapper.addMapping(m_ui->m_heightLabel, MessagesModel::COLUMN_HEIGHT, "text");
  m_dataMapper.addMapping(m_ui->m_hashLabel, MessagesModel::COLUMN_HASH, "text");
  m_dataMapper.addMapping(m_ui->m_amountLabel, MessagesModel::COLUMN_AMOUNT, "text");
  m_dataMapper.addMapping(m_ui->m_sizeLabel, MessagesModel::COLUMN_MESSAGE_SIZE, "text");
  m_dataMapper.addMapping(m_ui->m_messageTextEdit, MessagesModel::COLUMN_FULL_MESSAGE, "plainText");
  m_dataMapper.addMapping(m_ui->m_replyButton, MessagesModel::COLUMN_HAS_REPLY_TO, "enabled");
  m_dataMapper.setCurrentModelIndex(modelIndex);

  m_ui->m_prevButton->setEnabled(m_dataMapper.currentIndex() > 0);
  m_ui->m_nextButton->setEnabled(m_dataMapper.currentIndex() < MessagesModel::instance().rowCount() - 1);
}

MessageDetailsDialog::~MessageDetailsDialog() {
}

QModelIndex MessageDetailsDialog::getCurrentMessageIndex() const {
  return MessagesModel::instance().index(m_dataMapper.currentIndex(), 0);
}

void MessageDetailsDialog::prevClicked() {
  m_dataMapper.toPrevious();
  m_ui->m_prevButton->setEnabled(m_dataMapper.currentIndex() > 0);
  m_ui->m_nextButton->setEnabled(m_dataMapper.currentIndex() < MessagesModel::instance().rowCount() - 1);
}

void MessageDetailsDialog::nextClicked() {
  m_dataMapper.toNext();
  m_ui->m_prevButton->setEnabled(m_dataMapper.currentIndex() > 0);
  m_ui->m_nextButton->setEnabled(m_dataMapper.currentIndex() < MessagesModel::instance().rowCount() - 1);
}

void MessageDetailsDialog::saveClicked() {
  QString filePath = QFileDialog::getSaveFileName(this, tr("Save message"), QDir::homePath());
  if (!filePath.isEmpty()) {
    QFile file(filePath);
    if (file.exists()) {
      if (QMessageBox::warning(&MainWindow::instance(), tr("File already exists"),
        tr("Warning! File already exists and will be overwritten, are you sure?"), QMessageBox::Cancel, QMessageBox::Ok) != QMessageBox::Ok) {
        return;
      }
    }

    if (!file.open(QFile::WriteOnly | QFile::Truncate)) {
      QMessageBox::critical(&MainWindow::instance(), tr("File error"), file.errorString());
      return;
    }

    QString message = m_ui->m_messageTextEdit->toPlainText();
    file.write(message.toUtf8());
    file.close();
  }
}

}
