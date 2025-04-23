#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace RWPUtil {
	using BinaryBuffer = std::vector<uint8_t>;

	struct EncodedBytes {
		EncodedBytes(uint32_t __size) {
			size = __size;
			bytes = new uint8_t[__size];
		}

		EncodedBytes(uint8_t* __data, uint32_t __size) {
			size = __size;
			bytes = new uint8_t[__size];
			std::copy(__data, __data + size, bytes);
		}

		// Copy constructor
		EncodedBytes(const EncodedBytes& other) {
			size = other.size;
			bytes = new uint8_t[size];
			std::copy(other.bytes, other.bytes + size, bytes);
		}

		// Copy assignment
		EncodedBytes& operator=(const EncodedBytes& other) {
			if (this != &other) {
				delete[] bytes;  // Free existing memory
				size = other.size;
				bytes = new uint8_t[size];
				std::copy(other.bytes, other.bytes + size, bytes);
			}
			return *this;
		}

		~EncodedBytes() { delete[] bytes; }
		
		// Move constructor (C++11)
		EncodedBytes(EncodedBytes&& other) noexcept
			: bytes(other.bytes), size(other.size) {
			other.bytes = nullptr;
			other.size = 0;
		}

		// Move assignment (C++11)
		EncodedBytes& operator=(EncodedBytes&& other) noexcept {
			if (this != &other) {
				delete[] bytes;
				bytes = other.bytes;
				size = other.size;
				other.bytes = nullptr;
				other.size = 0;
			}
			return *this;
		}

		uint8_t* bytes;
		uint32_t size;
	};

	EncodedBytes encodeStr(std::string __str);
	std::string decodeStr(EncodedBytes* __bytes);
	void useBuffer(BinaryBuffer* __buffer, EncodedBytes* __bytes);
	void useBuffer(BinaryBuffer* __buffer, uint8_t* __bytes, uint32_t __size);

	template<typename T>
	EncodedBytes encodeNumber(T __bytes) {
		EncodedBytes result = EncodedBytes(sizeof(T));
		memcpy(result.bytes, &__bytes, sizeof(T));

		return result;
	}

	template<typename T>
	T decodeNumber(EncodedBytes* __bytes) {
		if (sizeof(T) != __bytes->size)
			std::exit(-1);

		T result;
		memcpy(&result, __bytes->bytes, __bytes->size);

		return result;
	}

	template<typename T>
	T decodeNumber(uint8_t* __bytes) {
		T result;
		memcpy(&result, __bytes, sizeof(T));

		return result;
	}
}