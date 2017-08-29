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

#include <QDataStream>
#include <QDateTime>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QTextStream>
#include <QTimer>
#include <QTimerEvent>

#include "StratumClient.h"
#include "WalletLogger/WalletLogger.h"

namespace WalletGui {

namespace {

const QString JSON_RPC_TAG_NAME_ID = "id";
const QString JSON_RPC_TAG_NAME_ERROR = "error";
const QString JSON_RPC_TAG_NAME_MESSAGE = "message";
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

const int RECONNECT_TIMER_INTERVAL = 30000;
const int RESPONSE_TIMER_INTERVAL = 10000;

}

StratumClient::StratumClient(Job& _job, QReadWriteLock& _jobLock, std::atomic<quint32>& _nonce, const QString& _host,
    quint16 _port, quint32 _difficulty, const QString& _login, const QString& _password, QObject *parent) :
  QObject(parent), m_host(_host), m_port(_port), m_difficulty(_difficulty), m_login(_login), m_password(_password),
  m_socket(new QTcpSocket(this)), m_currentSessionId(), m_currentJob(_job), m_jobLock(_jobLock), m_nonce(_nonce),
  m_requestCounter(0), m_reconnectTimerId(-1), m_responseTimerId(-1), m_goodShareCount(0), m_badShareCount(0),
  m_connectionErrorCount(0), m_lastConnectionError() {
  connect(m_socket, &QTcpSocket::connected, this, &StratumClient::connectedToHost);
  connect(m_socket, &QTcpSocket::readyRead, this, &StratumClient::readyRead);
  connect(m_socket, static_cast<void (QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error), this, &StratumClient::socketError);
}

StratumClient::~StratumClient() {
}

void StratumClient::start() {
  Q_ASSERT(m_socket->state() == QTcpSocket::UnconnectedState);
  WalletLogger::debug(tr("[Stratum] Connecting to mining pool %1:%2").arg(m_host).arg(m_port));
  m_socket->connectToHost(m_host, m_port);
}

void StratumClient::stop() {
  QEventLoop waitLoop;
  QTimer disconnectTimer;
  disconnectTimer.setSingleShot(true);
  disconnectTimer.setInterval(RECONNECT_TIMER_INTERVAL);
  connect(m_socket, &QTcpSocket::disconnected, &waitLoop, &QEventLoop::quit);
  connect(&disconnectTimer, &QTimer::timeout, &waitLoop, &QEventLoop::quit);
  WalletLogger::debug(tr("[Stratum] Disconnecting..."));
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
  {
    QWriteLocker lock(&m_jobLock);
    m_currentJob = Job();
  }

  m_lastConnectionError = QDateTime();
  Q_EMIT stoppedSignal();
}

QString StratumClient::getLogin() const {
  return m_login;
}

QString StratumClient::getPoolHost() const {
  return m_host;
}

quint16 StratumClient::getPoolPort() const {
  return m_port;
}

quint32 StratumClient::getDifficulty() const {
  QReadLocker lock(&m_jobLock);
  if (!m_currentJob.jobId.isEmpty()) {
    return 0x00000000ffffffff / static_cast<quint64>(m_currentJob.target);
  }

  return 0;
}

quint32 StratumClient::getGoodShareCount() const {
  return m_goodShareCount;
}

quint32 StratumClient::getBadShareCount() const {
  return m_badShareCount;
}

quint32 StratumClient::getConnectionErrorCount() const {
  return m_connectionErrorCount;
}

QDateTime StratumClient::getLastConnectionErrorTime() const {
  return m_lastConnectionError;
}

void StratumClient::addObserver(IPoolClientObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  m_observerConnections[_observer] << connect(this, SIGNAL(startedSignal()), observer, SLOT(started()));
  m_observerConnections[_observer] << connect(this, SIGNAL(stoppedSignal()), observer, SLOT(stopped()));
  m_observerConnections[_observer] << connect(this, SIGNAL(socketErrorSignal()), observer, SLOT(socketError()));
  m_observerConnections[_observer] << connect(this, SIGNAL(difficultyChangedSignal(quint32)), observer, SLOT(difficultyChanged(quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(goodShareCountChangedSignal(quint32)), observer, SLOT(goodShareCountChanged(quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(badShareCountChangedSignal(quint32)), observer, SLOT(badShareCountChanged(quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(connectionErrorCountChangedSignal(quint32)), observer, SLOT(connectionErrorCountChanged(quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(lastConnectionErrorTimeChangedSignal(QDateTime)), observer, SLOT(lastConnectionErrorTimeChanged(QDateTime)));
}

void StratumClient::removeObserver(IPoolClientObserver* _observer) {
  if (!m_observerConnections.contains(_observer) || m_observerConnections.value(_observer).isEmpty()) {
    return;
  }

  for (const auto& connection : m_observerConnections[_observer]) {
    disconnect(connection);
  }

  m_observerConnections[_observer].clear();
}

void StratumClient::shareFound(const QString& _jobId, quint32 _nonce, const QByteArray& _result) {
  submitShare(_jobId, _nonce, _result);
}

void StratumClient::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_reconnectTimerId) {
    resetReconnectionTimer();
    m_socket->abort();
    start();
    return;
  } else if (_event->timerId() == m_responseTimerId) {
    resetResponseTimer();
    ++m_connectionErrorCount;
    Q_EMIT connectionErrorCountChangedSignal(m_connectionErrorCount);
    m_lastConnectionError = QDateTime::currentDateTime();
    Q_EMIT lastConnectionErrorTimeChangedSignal(m_lastConnectionError);
    Q_EMIT socketErrorSignal();
    WalletLogger::warning(tr("[Stratum] Response timed out"));
    reconnect();
    return;
  }

  QObject::timerEvent(_event);
}

void StratumClient::connectedToHost() {
  loginRequest();
}

void StratumClient::reconnect() {
  if (m_responseTimerId != -1) {
    killTimer(m_responseTimerId);
    m_responseTimerId = -1;
  }

  m_activeRequestMap.clear();
  m_currentSessionId.clear();
  {
    QWriteLocker lock(&m_jobLock);
    m_currentJob = Job();
  }

  if (m_reconnectTimerId == -1) {
    m_reconnectTimerId = startTimer(RECONNECT_TIMER_INTERVAL);
  }
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
    WalletLogger::debug(QString("[Stratum] <<<< %1").arg(line));
    QJsonParseError parseError;
    QJsonObject dataObject = QJsonDocument::fromJson(line.toUtf8(), &parseError).object();
    if (parseError.error == QJsonParseError::NoError) {
      processData(dataObject);
    } else {
      WalletLogger::critical(tr("[Stratum] Json parse error: %1").arg(parseError.errorString()));
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
      WalletLogger::warning(tr("[Stratum] Unknown responce with id=%1").arg(id));
      return;
    }

    JsonRpcRequest request = m_activeRequestMap.take(id);
    if (request.method == STRATUM_METHOD_NAME_LOGIN) {
      processLoginResponce(_jsonObject, request);
    } else if (request.method == STRATUM_METHOD_NAME_SUBMIT) {
      processSubmitResponce(_jsonObject, request);
    }
  }
}

void StratumClient::processNotification(const QJsonObject& _jsonObject) {
  if (_jsonObject.value(JSON_RPC_TAG_NAME_METHOD).toString() == STRATUM_METHOD_NAME_JOB) {
    processJobNotification(_jsonObject);
  }
}

void StratumClient::socketError(QTcpSocket::SocketError _error) {
  ++m_connectionErrorCount;
  m_lastConnectionError = QDateTime::currentDateTime();
  WalletLogger::critical(tr("[Stratum] Socket error: %1. Reconnecting...").arg(m_socket->errorString()));
  Q_EMIT socketErrorSignal();
  Q_EMIT connectionErrorCountChangedSignal(m_connectionErrorCount);
  Q_EMIT lastConnectionErrorTimeChangedSignal(m_lastConnectionError);
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
  if (m_socket->state() != QTcpSocket::ConnectedState) {
    return;
  }

  QByteArray requestData = makeJsonRequest(_request);
  WalletLogger::debug(QString("[Stratum] >>>> %1").arg(QString::fromUtf8(requestData)));
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
  QString login = m_login;
  if (m_difficulty > 0) {
    login.append(QString(".%1").arg(m_difficulty));
  }

  loginRequest.params[STRATUM_LOGIN_PARAM_NAME_LOGIN] = login;
  loginRequest.params[STRATUM_LOGIN_PARAM_NAME_PASS] = m_password;
  sendRequest(loginRequest);
}

void StratumClient::processLoginResponce(const QJsonObject& _responceObject, const JsonRpcRequest& _request) {
  if (_responceObject.contains(JSON_RPC_TAG_NAME_ERROR) && !_responceObject.value(JSON_RPC_TAG_NAME_ERROR).isNull()) {
    WalletLogger::critical(tr("[Stratum] Login failed: %1. Reconnecting...").arg(_responceObject.value(JSON_RPC_TAG_NAME_ERROR).toObject().value(JSON_RPC_TAG_NAME_MESSAGE).toString()));
    ++m_connectionErrorCount;
    Q_EMIT connectionErrorCountChangedSignal(m_connectionErrorCount);
    m_lastConnectionError = QDateTime::currentDateTime();
    Q_EMIT lastConnectionErrorTimeChangedSignal(m_lastConnectionError);
    Q_EMIT socketErrorSignal();
    reconnect();
    return;
  }

  QString status = _responceObject.value(JSON_RPC_TAG_NAME_RESULT).toObject().value(STRATUM_LOGIN_PARAM_NAME_STATUS).toString();
  if (status != "OK") {
    WalletLogger::critical(tr("[Stratum] Login failed. Invalid status: %1. Reconnecting...").arg(status));
    ++m_connectionErrorCount;
    Q_EMIT connectionErrorCountChangedSignal(m_connectionErrorCount);
    m_lastConnectionError = QDateTime::currentDateTime();
    Q_EMIT lastConnectionErrorTimeChangedSignal(m_lastConnectionError);
    Q_EMIT socketErrorSignal();
    reconnect();
    return;
  }

  m_currentSessionId = _responceObject.value(JSON_RPC_TAG_NAME_RESULT).toObject().value(STRATUM_LOGIN_PARAM_NAME_SESSION_ID).toString();
  updateJob(_responceObject.value(JSON_RPC_TAG_NAME_RESULT).toObject().value(STRATUM_LOGIN_PARAM_NAME_JOB).toObject().toVariantMap());
  m_lastConnectionError = QDateTime();
  Q_EMIT lastConnectionErrorTimeChangedSignal(m_lastConnectionError);
  Q_EMIT startedSignal();
}

void StratumClient::processSubmitResponce(const QJsonObject& _responceObject, const JsonRpcRequest& _request) {
  if (_responceObject.contains(JSON_RPC_TAG_NAME_ERROR) && !_responceObject.value(JSON_RPC_TAG_NAME_ERROR).isNull()) {
    Q_EMIT badShareCountChangedSignal(++m_badShareCount);
    WalletLogger::warning(tr("[Stratum] Share submit error: %1").arg(_responceObject.value(JSON_RPC_TAG_NAME_ERROR).toObject().value(JSON_RPC_TAG_NAME_MESSAGE).toString()));

    reconnect();
  } else {
    Q_EMIT goodShareCountChangedSignal(++m_goodShareCount);
    WalletLogger::debug(tr("[Stratum] Share submitted"));
  }
}

void StratumClient::processJobNotification(const QJsonObject& _notificationObject) {
  updateJob(_notificationObject.value(JSON_RPC_TAG_NAME_PARAMS).toObject().toVariantMap());
}

void StratumClient::updateJob(const QVariantMap& _newJobMap) {
  QString jobId = _newJobMap.value(STRATUM_JOB_PARAM_NAME_JOB_ID).toString();
  if (!jobId.isEmpty()) {
    QWriteLocker lock(&m_jobLock);
    QByteArray blob = QByteArray::fromHex(_newJobMap.value(STRATUM_JOB_PARAM_NAME_JOB_BLOB).toByteArray());
    QByteArray targetArr = QByteArray::fromHex(_newJobMap.value(STRATUM_JOB_PARAM_NAME_JOB_TARGET).toByteArray());
    quint32 target;
    QDataStream targetStream(targetArr);
    targetStream.setByteOrder(QDataStream::LittleEndian);
    targetStream >> target;
    m_currentJob = {jobId, target, blob};
    m_nonce = 0;
    WalletLogger::debug(QString("[Stratum] New mining job: id=\"%1\"").arg(jobId));
  }

  quint32 difficulty = getDifficulty();
  Q_EMIT difficultyChangedSignal(difficulty);
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
