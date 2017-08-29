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

#include <QTcpServer>

namespace QJsonRpc {

class JsonRpcObject;
class JsonRpcRequest;
class JsonRpcResponse;
class JsonRpcNotification;

class JsonRpcServer : public QTcpServer {
  Q_OBJECT
  Q_DISABLE_COPY(JsonRpcServer)

public:
  explicit JsonRpcServer(QObject* _parent);
  virtual ~JsonRpcServer();

  void setHandler(QObject* _jsonRpcHandler);

protected:
  void sendNotification(const JsonRpcNotification& _notification);

private:
  QObject* m_handler;

  void onAcceptError(QAbstractSocket::SocketError _socketError);
  void onNewConnection();
  void onSocketDisconneced();
  void onSocketError(QAbstractSocket::SocketError _socketError);
  void onSocketReadyRead();
  QJsonValue processJsonValue(const QJsonValue& _request);
  void sendError(QTcpSocket* _socket, int _errorCode, const QString& _errorMessage, const QString& _errorData);
  void sendObject(QTcpSocket* _socket, const JsonRpcObject& _object);
  void sendJson(QTcpSocket* _socket, const QJsonValue& _jsonValue);
};

}
