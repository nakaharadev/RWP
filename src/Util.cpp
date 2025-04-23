#include "../include/private/RWP/Util.hpp"

#include <span>

namespace RWPUtil {
	EncodedBytes encodeStr(std::string __str) {
		EncodedBytes bytes = EncodedBytes(__str.length());

		auto encoded = reinterpret_cast<const uint8_t*>(__str.data());
		memcpy(bytes.bytes, encoded, bytes.size);

		return bytes;
	}

	std::string decodeStr(EncodedBytes* __bytes) {
		return std::string(reinterpret_cast<const char*>(__bytes->bytes), __bytes->size);
	}

	void useBuffer(BinaryBuffer* __buffer, EncodedBytes* __bytes) {
		std::span<uint8_t> data_span(__bytes->bytes, __bytes->size);
		__buffer->insert(__buffer->end(), data_span.begin(), data_span.end());
	}

	void useBuffer(BinaryBuffer* __buffer, uint8_t* __bytes, uint32_t __size) {
		std::span<uint8_t> data_span(__bytes, __size);
		__buffer->insert(__buffer->end(), data_span.begin(), data_span.end());
	}
}