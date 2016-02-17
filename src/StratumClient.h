// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QObject>
#include <QReadWriteLock>
#include <QTcpSocket>

#include <atomic>

#include "Worker.h"

class QTcpSocket;

namespace WalletGui {

struct JsonRpcRequest {
  QString method;
  QVariantMap params;
};

struct Job;

class StratumClient : public QObject, public IWorkerObserver {
  Q_OBJECT

public:
  StratumClient(QObject *parent, Job& _job, QReadWriteLock& _jobLock, std::atomic<quint32>& _nonce,
    const QString& _host, quint16 _port, const QString& _login, const QString& _password);
  ~StratumClient();

  void start();
  void stop();
  void processShare(const QString& _jobId, quint32 _nonce, const QByteArray& _result) Q_DECL_OVERRIDE;

  QString getPoolHost() const;
  quint16 getPoolPort() const;

protected:
  void timerEvent(QTimerEvent* _event) Q_DECL_OVERRIDE;

private:
  const QString m_host;
  const quint16 m_port;
  const QString m_login;
  const QString m_password;
  QTcpSocket* m_socket;
  QString m_currentSessionId;
  Job& m_currentJob;
  QReadWriteLock& m_jobLock;
  std::atomic<quint32>& m_nonce;
  quint64 m_requestCounter;
  QMap<quint64, JsonRpcRequest> m_activeRequestMap;
  int m_reconnectTimerId;
  int m_responseTimerId;

  void connectedToHost();
  void reconnect();
  void resetReconnectionTimer();
  void resetResponseTimer();
  void readyRead();
  void processData(const QJsonObject& _jsonObject);
  void processNotification(const QJsonObject& _jsonObject);
  void socketError(QTcpSocket::SocketError _error);
  QByteArray makeJsonRequest(const JsonRpcRequest& _request);
  void sendRequest(const JsonRpcRequest& _request);
  void loginRequest();

  void processLoginResponce(const QJsonObject& _responceObject, const JsonRpcRequest& _request);
  void processJobNotification(const QJsonObject& _notificationObject);
  void updateJob(const QVariantMap& _newJobMap);
  void submitShare(const QString& _jobId, quint32 _nonce, const QByteArray& _result);


Q_SIGNALS:
  void submitShareSignal(const QString& _jobId, quint32 _nonce, const QByteArray& _result);
  void socketErrorSignal(const QString& _errorText);
};

}
