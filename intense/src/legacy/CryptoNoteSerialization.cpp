// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Bytecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Bytecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Bytecoin.  If not, see <http://www.gnu.org/licenses/>.

#include "CryptoNoteSerialization.h"

#include "ISerializer.h"
#include "SerializationOverloads.h"
#include "BinaryInputStreamSerializer.h"
#include "BinaryOutputStreamSerializer.h"

#include "crypto/crypto.h"

namespace {
	template <typename T>
	bool serializePod(T& v, Common::StringView name, CryptoNote::ISerializer& serializer) {
	  return serializer.binary(&v, sizeof(v), name);
	}	
}

namespace crypto {
	bool serialize(chacha8_iv& chacha, Common::StringView name, CryptoNote::ISerializer& serializer) {
	  return serializePod(chacha, name, serializer);
	}
	bool serialize(crypto::public_key& pubKey, Common::StringView name, CryptoNote::ISerializer& serializer) {
	  return serializePod(pubKey, name, serializer);
	}

	bool serialize(crypto::secret_key& secKey, Common::StringView name, CryptoNote::ISerializer& serializer) {
	  return serializePod(secKey, name, serializer);
	}
}