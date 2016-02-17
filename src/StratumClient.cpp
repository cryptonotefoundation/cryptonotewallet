// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDataStream>
#include <QDebug>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QTextStream>
#include <QTimer>
#include <QTimerEvent>

#include "StratumClient.h"

namespace WalletGui {

const QString JSON_RPC_TAG_NAME_ID = "id";
const QString JSON_RPC_TAG_NAME_ERROR = "error";
const QString JSON_RPC_TAG_NAME_METHOD = "method";
const QString JSON_RPC_TAG_NAME_PARAMS = "params";
const QString JSON_RPC_TAG_NAME_RESULT = "result";
const QString JSON_RPC_TAG_NAME_JSONRPC = "jsonrpc";

const QString STRATUM_METHOD_NAME_LOGIN = "login";
const QString STRATUM_METHOD_NAME_JOB = "job";
const QString STRATUM_METHOD_NAME_SUBMIT = "submit";
const QString STRATUM_LOGIN_PARAM_NAME_AGENT = "agent";
const QString STRATUM_LOGIN_PARAM_NAME_LOGIN = "login";
const QString STRATUM_LOGIN_PARAM_NAME_PASS = "pass";
const QString STRATUM_LOGIN_PARAM_NAME_SESSION_ID = "id";
const QString STRATUM_LOGIN_PARAM_NAME_STATUS = "status";
const QString STRATUM_LOGIN_PARAM_NAME_JOB = "job";
const QString STRATUM_JOB_PARAM_NAME_JOB_BLOB = "blob";
const QString STRATUM_JOB_PARAM_NAME_JOB_ID = "job_id";
const QString STRATUM_JOB_PARAM_NAME_JOB_TARGET = "target";
const QString STRATUM_SUBMIT_PARAM_NAME_SESSION_ID = STRATUM_LOGIN_PARAM_NAME_SESSION_ID;
const QString STRATUM_SUBMIT_PARAM_NAME_JOB_ID = STRATUM_JOB_PARAM_NAME_JOB_ID;
const QString STRATUM_SUBMIT_PARAM_NAME_NONCE = "nonce";
const QString STRATUM_SUBMIT_PARAM_NAME_RESULT = "result";

const int RECONNECT_TIMER_INTERVAL = 3000;
const int RESPONSE_TIMER_INTERVAL = 10000;

StratumClient::StratumClient(QObject *parent, Job& _job, QReadWriteLock& _jobLock, std::atomic<quint32>& _nonce,
  const QString& _host, quint16 _port, const QString& _login, const QString& _password) :
  QObject(parent), m_host(_host), m_port(_port), m_login(_login), m_password(_password),
  m_socket(new QTcpSocket(this)), m_currentSessionId(), m_currentJob(_job), m_jobLock(_jobLock), m_nonce(_nonce),
  m_requestCounter(0), m_reconnectTimerId(-1), m_responseTimerId(-1) {
  connect(m_socket, &QTcpSocket::connected, this, &StratumClient::connectedToHost);
  connect(m_socket, &QTcpSocket::readyRead, this, &StratumClient::readyRead);
  connect(m_socket, static_cast<void (QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error), this, &StratumClient::socketError);
  connect(this, &StratumClient::submitShareSignal, this, &StratumClient::submitShare, Qt::QueuedConnection);
}

StratumClient::~StratumClient() {
}

void StratumClient::start() {
  Q_ASSERT(m_socket->state() != QTcpSocket::ConnectedState);
  m_socket->connectToHost(m_host, m_port);
}

void StratumClient::stop() {
  QEventLoop waitLoop;
  QTimer disconnectTimer;
  disconnectTimer.setSingleShot(true);
  disconnectTimer.setInterval(RECONNECT_TIMER_INTERVAL);
  connect(m_socket, &QTcpSocket::disconnected, &waitLoop, &QEventLoop::quit);
  connect(&disconnectTimer, &QTimer::timeout, &waitLoop, &QEventLoop::quit);
  m_socket->disconnectFromHost();
  disconnectTimer.start();
  if (m_socket->state() != QTcpSocket::UnconnectedState) {
    waitLoop.exec();
  }

  if (!disconnectTimer.isActive()) {
    m_socket->abort();
    disconnectTimer.stop();
  }

  if (m_reconnectTimerId != -1) {
    killTimer(m_reconnectTimerId);
    m_reconnectTimerId = -1;
  }

  if (m_responseTimerId != -1) {
    killTimer(m_responseTimerId);
    m_responseTimerId = -1;
  }

  m_activeRequestMap.clear();
  m_currentSessionId.clear();
  QWriteLocker lock(&m_jobLock);
  m_currentJob = Job();
}

void StratumClient::processShare(const QString& _jobId, quint32 _nonce, const QByteArray& _result) {
  Q_EMIT submitShareSignal(_jobId, _nonce, _result);
}

QString StratumClient::getPoolHost() const {
  return m_host;
}

quint16 StratumClient::getPoolPort() const {
  return m_port;
}

void StratumClient::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_reconnectTimerId) {
    resetReconnectionTimer();
    start();
    return;
  } else if (_event->timerId() == m_responseTimerId) {
    resetResponseTimer();
    reconnect();
  }

  QObject::timerEvent(_event);
}

void StratumClient::connectedToHost() {
  qDebug() << "Connected!!!";
  loginRequest();
}

void StratumClient::reconnect() {
  stop();
  m_reconnectTimerId = startTimer(RECONNECT_TIMER_INTERVAL);
}

void StratumClient::resetReconnectionTimer() {
  if (m_reconnectTimerId != -1) {
    killTimer(m_reconnectTimerId);
    m_reconnectTimerId = -1;
  }
}

void StratumClient::resetResponseTimer() {
  if (m_responseTimerId != -1) {
    killTimer(m_responseTimerId);
    m_responseTimerId = -1;
  }
}

void StratumClient::readyRead() {
  if (m_responseTimerId != -1) {
    killTimer(m_responseTimerId);
    m_responseTimerId = -1;
  }

  QTextStream dataStream(m_socket);
  for (QString line = dataStream.readLine(); !line.isEmpty(); line = dataStream.readLine()) {
    qDebug() << "<<<< " << line;
    QJsonParseError parseError;
    QJsonObject dataObject = QJsonDocument::fromJson(line.toUtf8(), &parseError).object();
    if (parseError.error == QJsonParseError::NoError) {
      processData(dataObject);
    } else {
      qDebug() << "Json parse error: " << parseError.errorString();
    }
  }
}

void StratumClient::processData(const QJsonObject& _jsonObject) {
  if (!_jsonObject.contains(JSON_RPC_TAG_NAME_ID)) {
    processNotification(_jsonObject);
    return;
  } else {
    quint64 id = _jsonObject.value(JSON_RPC_TAG_NAME_ID).toString().toULongLong();
    if (!m_activeRequestMap.contains(id)) {
      qDebug() << "Unknown responce with id = " << id;
      return;
    }

    JsonRpcRequest request = m_activeRequestMap.take(id);
    if (request.method == STRATUM_METHOD_NAME_LOGIN) {
      processLoginResponce(_jsonObject, request);
    }
  }
}

void StratumClient::processNotification(const QJsonObject& _jsonObject) {
  if (_jsonObject.value(JSON_RPC_TAG_NAME_METHOD).toString() == STRATUM_METHOD_NAME_JOB) {
    processJobNotification(_jsonObject);
  }
}

void StratumClient::socketError(QTcpSocket::SocketError _error) {
  qDebug() << "Socket error:" << m_socket->errorString() << ". Reconnecting...";
  Q_EMIT socketErrorSignal(m_socket->errorString());
  reconnect();
}

QByteArray StratumClient::makeJsonRequest(const JsonRpcRequest& _request) {
  QJsonObject jsonRequestObject;
  jsonRequestObject.insert(JSON_RPC_TAG_NAME_ID, QString::number(++m_requestCounter));
  jsonRequestObject.insert(JSON_RPC_TAG_NAME_JSONRPC, QString("2.0"));
  jsonRequestObject.insert(JSON_RPC_TAG_NAME_METHOD, _request.method);
  jsonRequestObject.insert(JSON_RPC_TAG_NAME_PARAMS, QJsonObject::fromVariantMap(_request.params));
  return QJsonDocument(jsonRequestObject).toJson(QJsonDocument::Compact);
}

void StratumClient::sendRequest(const JsonRpcRequest& _request) {
  Q_ASSERT(m_socket->state() == QTcpSocket::ConnectedState);
  QByteArray requestData = makeJsonRequest(_request);
  qDebug() << ">>>> " << requestData;
  m_socket->write(requestData + "\n");
  m_activeRequestMap.insert(m_requestCounter, _request);
  if (m_responseTimerId == -1) {
    m_responseTimerId = startTimer(RESPONSE_TIMER_INTERVAL);
  }
}

void StratumClient::loginRequest() {
  JsonRpcRequest loginRequest;
  loginRequest.method = STRATUM_METHOD_NAME_LOGIN;
  loginRequest.params[STRATUM_LOGIN_PARAM_NAME_AGENT] = "Miner";
  loginRequest.params[STRATUM_LOGIN_PARAM_NAME_LOGIN] = m_login;
  loginRequest.params[STRATUM_LOGIN_PARAM_NAME_PASS] = m_password;
  sendRequest(loginRequest);
}

void StratumClient::processLoginResponce(const QJsonObject& _responceObject, const JsonRpcRequest& _request) {
  if (_responceObject.contains(JSON_RPC_TAG_NAME_ERROR) && !_responceObject.value(JSON_RPC_TAG_NAME_ERROR).isNull()) {
    qDebug() << "Login failed. JsonRPC error. Reconnecting...";
    reconnect();
    return;
  }

  if (_responceObject.value(JSON_RPC_TAG_NAME_RESULT).toObject().value(STRATUM_LOGIN_PARAM_NAME_STATUS).toString() != "OK") {
    qDebug() << "Login failed. Invalid status. Reconnecting...";
    reconnect();
    return;
  }

  m_currentSessionId = _responceObject.value(JSON_RPC_TAG_NAME_RESULT).toObject().value(STRATUM_LOGIN_PARAM_NAME_SESSION_ID).toString();
  updateJob(_responceObject.value(JSON_RPC_TAG_NAME_RESULT).toObject().value(STRATUM_LOGIN_PARAM_NAME_JOB).toObject().toVariantMap());
}

void StratumClient::processJobNotification(const QJsonObject& _notificationObject) {
  updateJob(_notificationObject.value(JSON_RPC_TAG_NAME_PARAMS).toObject().toVariantMap());
}

void StratumClient::updateJob(const QVariantMap& _newJobMap) {
  QString jobId = _newJobMap.value(STRATUM_JOB_PARAM_NAME_JOB_ID).toString();
  if (jobId.isEmpty()) {
    qDebug() << "Job didn't changed";
  } else {
    QWriteLocker lock(&m_jobLock);
    QByteArray blob = QByteArray::fromHex(_newJobMap.value(STRATUM_JOB_PARAM_NAME_JOB_BLOB).toByteArray());
    QByteArray targetArr = QByteArray::fromHex(_newJobMap.value(STRATUM_JOB_PARAM_NAME_JOB_TARGET).toByteArray());
    quint32 target;
    QDataStream targetStream(targetArr);
    targetStream.setByteOrder(QDataStream::LittleEndian);
    targetStream >> target;
    m_currentJob.jobId = jobId;
    m_currentJob.blob = QByteArray::fromHex(_newJobMap.value(STRATUM_JOB_PARAM_NAME_JOB_BLOB).toByteArray());;
    m_currentJob.target = target;
    m_nonce = 0;
  }
}

void StratumClient::submitShare(const QString& _jobId, quint32 _nonce, const QByteArray& _result) {
  QReadLocker lock(&m_jobLock);
  if (m_currentJob.jobId != _jobId) {
    return;
  }

  JsonRpcRequest submitRequest;
  submitRequest.method = STRATUM_METHOD_NAME_SUBMIT;

  submitRequest.params.insert(STRATUM_SUBMIT_PARAM_NAME_SESSION_ID, m_currentSessionId);
  submitRequest.params.insert(STRATUM_SUBMIT_PARAM_NAME_JOB_ID, _jobId);
  QByteArray nonceArr;
  QDataStream nonceStream(&nonceArr, QIODevice::WriteOnly);
  nonceStream.setByteOrder(QDataStream::LittleEndian);
  nonceStream << _nonce;

  submitRequest.params.insert(STRATUM_SUBMIT_PARAM_NAME_NONCE, QString::fromUtf8(nonceArr.toHex()));
  submitRequest.params.insert(STRATUM_SUBMIT_PARAM_NAME_RESULT, _result.toHex());
  sendRequest(submitRequest);
}

}
