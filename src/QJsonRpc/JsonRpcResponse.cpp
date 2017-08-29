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

#include "JsonRpcResponse.h"
#include "JsonRpcRequest.h"

namespace QJsonRpc {

JsonRpcResponse::JsonRpcResponse() : JsonRpcObject(TYPE_RESPONSE) {
}

JsonRpcResponse::JsonRpcResponse(const QJsonObject& _jsonObject) : JsonRpcObject(_jsonObject, TYPE_RESPONSE) {
}

JsonRpcResponse::~JsonRpcResponse() {
}

QString JsonRpcResponse::getId() const {
  return getValue(idTagName).toString();
}

QVariantList JsonRpcResponse::getResultAsArray() const {
  return getValue(resultTagName).toArray().toVariantList();
}

QVariantMap JsonRpcResponse::getResultAsObject() const {
  return getValue(paramsTagName).toObject().toVariantMap();
}

bool JsonRpcResponse::isErrorResponse() const {
  return contains(errorTagName);
}

int JsonRpcResponse::getErrorCode() const {
  return getValue(errorTagName).toObject().value(errorCodeTagName).toInt();
}

QString JsonRpcResponse::getErrorMessage() const {
  return getValue(errorTagName).toObject().value(errorMessageTagName).toString();
}

QVariant JsonRpcResponse::getErrorData() const {
  return getValue(errorTagName).toObject().value(errorDataTagName).toVariant();
}

void JsonRpcResponse::setId(const QString& _id) {
  QJsonValue id(_id.isNull() ? QJsonValue::Null : QJsonValue(_id));
  setValue(idTagName, id);
}

void JsonRpcResponse::setResultFromArray(const QVariantList& _variantList) {
  setValue(resultTagName, QJsonArray::fromVariantList(_variantList));
}

void JsonRpcResponse::setResultFromObject(const QVariantMap& _variantMap) {
  setValue(resultTagName, QJsonObject::fromVariantMap(_variantMap));
}

void JsonRpcResponse::setError(int _errorCode, const QString& _errorMessage, const QVariant& _errorData) {
  QJsonObject errorObject;
  errorObject.insert(errorCodeTagName, _errorCode);
  errorObject.insert(errorMessageTagName, _errorMessage);
  if (!_errorData.isNull()) {
    errorObject.insert(errorDataTagName, QJsonValue::fromVariant(_errorData));
  }

  setValue(errorTagName, errorObject);
}

}
