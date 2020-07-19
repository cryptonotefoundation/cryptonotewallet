// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma  once

#include <QDialog>
#include <QTableView>
#include <QShowEvent>
#include <QItemDelegate>
#include "Settings.h"

namespace Ui {
class ConnectionSettingsDialog;
}

namespace WalletGui {

class NodeModel;

class RemoteNodesView : public QTableView {
  Q_OBJECT

  public:
    explicit RemoteNodesView(QWidget *parent=0) : QTableView(parent) {}

  protected:
    void showEvent(QShowEvent *e);
};

class RemoteNodesDelegate : public QItemDelegate {
  Q_OBJECT

  public:
    explicit RemoteNodesDelegate(QObject *parent=0) : QItemDelegate(parent) {}

  protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

class ConnectionSettingsDialog : public QDialog {
  Q_OBJECT
  Q_DISABLE_COPY(ConnectionSettingsDialog)

public:
  ConnectionSettingsDialog(QWidget* _parent);
  ~ConnectionSettingsDialog();

  QString setConnectionMode() const;
  quint16 setLocalDaemonPort() const;
  NodeSetting setRemoteNode() const;
  void initConnectionSettings();

private:
  QScopedPointer<Ui::ConnectionSettingsDialog> m_ui;
  NodeModel *m_nodeModel;
  QTableView *m_remoteNodesView;
  void updateNodeSelect();
  void setupRemoteNodesView(QTableView *view);
  int m_nodesCurrentIndex;

  Q_SLOT void addNodeClicked();
  Q_SLOT void removeNodeClicked();
  Q_SLOT void nodesCurrentIndex(int currentIndex);

};

}

