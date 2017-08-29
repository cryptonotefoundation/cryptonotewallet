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

#include "JsonRpcNotification.h"

namespace QJsonRpc {

JsonRpcNotification::JsonRpcNotification() : JsonRpcObject(TYPE_NOTIFICATION) {
}

JsonRpcNotification::JsonRpcNotification(const QJsonObject& _jsonObject) : JsonRpcObject(_jsonObject, TYPE_NOTIFICATION) {
}

JsonRpcNotification::~JsonRpcNotification() {
}

QString JsonRpcNotification::getMethod() const {
  return getValue(methodTagName).toString();
}

QVariantList JsonRpcNotification::getParamsAsArray() const {
  return getValue(paramsTagName).toArray().toVariantList();
}

QVariantMap JsonRpcNotification::getParamsAsObject() const {
  return getValue(paramsTagName).toObject().toVariantMap();
}

void JsonRpcNotification::setMethod(const QString& _method) {
  setValue(methodTagName, _method);
}

void JsonRpcNotification::setParamsFromArray(const QVariantList& _variantList) {
  setValue(paramsTagName, QJsonArray::fromVariantList(_variantList));
}

void JsonRpcNotification::setParamsFromObject(const QVariantMap& _variantMap) {
  setValue(paramsTagName, QJsonObject::fromVariantMap(_variantMap));
}

}
