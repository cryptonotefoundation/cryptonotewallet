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

#include "JsonRpcRequest.h"

namespace QJsonRpc {

JsonRpcRequest::JsonRpcRequest() : JsonRpcObject(TYPE_REQUEST) {
}

JsonRpcRequest::JsonRpcRequest(const QJsonObject& _jsonObject) : JsonRpcObject(_jsonObject, TYPE_REQUEST) {
}

JsonRpcRequest::~JsonRpcRequest() {
}

QString JsonRpcRequest::getId() const {
  return getValue(idTagName).toString();
}

QString JsonRpcRequest::getMethod() const {
  return getValue(methodTagName).toString();
}

QVariantList JsonRpcRequest::getParamsAsArray() const {
  return getValue(paramsTagName).toArray().toVariantList();
}

QVariantMap JsonRpcRequest::getParamsAsObject() const {
  return getValue(paramsTagName).toObject().toVariantMap();
}

void JsonRpcRequest::setId(const QString& _id) {
  QJsonValue id(_id.isNull() ? QJsonValue::Null : QJsonValue(_id));
  setValue(idTagName, id);
}

void JsonRpcRequest::setMethod(const QString& _method) {
  setValue(methodTagName, _method);
}

void JsonRpcRequest::setParamsFromArray(const QVariantList& _variantList) {
  setValue(paramsTagName, QJsonArray::fromVariantList(_variantList));
}

void JsonRpcRequest::setParamsFromObject(const QVariantMap& _variantMap) {
  setValue(paramsTagName, QJsonObject::fromVariantMap(_variantMap));
}

}
