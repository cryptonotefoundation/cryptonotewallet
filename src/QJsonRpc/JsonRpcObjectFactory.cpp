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

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "JsonRpcObjectFactory.h"
#include "JsonRpcNotification.h"
#include "JsonRpcRequest.h"
#include "JsonRpcResponse.h"

namespace QJsonRpc {

JsonRpcObject* JsonRpcObjectFactory::createJsonRpcObject(const QJsonValue& _jsonValue, int& _jsonRpcErrorCode,
  QString& _jsonRpcErrorString, QString& _jsonRpcErrorData) {
  if (_jsonValue.isObject()) {
    return parseJsonRpcObject(_jsonValue.toObject(), _jsonRpcErrorCode, _jsonRpcErrorString,
      _jsonRpcErrorData);
  }

  _jsonRpcErrorCode = JsonRpcObject::JSON_RPC_INVALID_REQUEST_ERROR;
  _jsonRpcErrorString = JsonRpcObject::JSON_RPC_INVALID_REQUEST_ERROR_MESSAGE;
  return nullptr;
}

JsonRpcObject* JsonRpcObjectFactory::parseJsonRpcObject(const QJsonObject& _jsonObject, int& _jsonRpcErrorCode, QString& _jsonRpcErrorString,
  QString& _jsonRpcErrorData) {
  if (!_jsonObject.contains(JsonRpcObject::jsonRpcTagName) ||
    _jsonObject.value(JsonRpcObject::jsonRpcTagName).toString().compare(JsonRpcObject::jsonRpcVersion)) {
    _jsonRpcErrorCode = JsonRpcObject::JSON_RPC_INVALID_REQUEST_ERROR;
    _jsonRpcErrorString = JsonRpcObject::JSON_RPC_INVALID_REQUEST_ERROR_MESSAGE;
    _jsonRpcErrorData = "Incorrect JSON RPC version";
    return nullptr;
  }

  if (JsonRpcObject::isValidJsonRpcRequest(_jsonObject)) {
    return new JsonRpcRequest(_jsonObject);
  }

  if (JsonRpcObject::isValidJsonRpcResponse(_jsonObject)) {
    return new JsonRpcResponse(_jsonObject);
  }

  if (JsonRpcObject::isValidJsonRpcNotification(_jsonObject)) {
    return new JsonRpcNotification(_jsonObject);
  }

  _jsonRpcErrorCode = JsonRpcObject::JSON_RPC_INVALID_REQUEST_ERROR;
  _jsonRpcErrorString = JsonRpcObject::JSON_RPC_INVALID_REQUEST_ERROR_MESSAGE;
  return nullptr;
}

}
