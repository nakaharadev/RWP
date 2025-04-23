#pragma once

#include "Util.hpp"

#include <cstdint>
#include <string>
#include <any>

#define RWP_MSG_DATA_TYPE_TEXT		0x1
#define RWP_MSG_DATA_TYPE_UNKNOWN	0x2

namespace RWP {
	using MsgData = uint8_t*;

	struct MsgHeader;

	struct BinaryMsg {
		BinaryMsg(uint32_t __size) {
			size = __size;
			bytes = new uint8_t[__size];
		}

		BinaryMsg(uint8_t* __bytes, uint32_t __size) {
			size = __size;
			bytes = new uint8_t[__size];

			memcpy(bytes, __bytes, size);
		}

		uint8_t* bytes;
		uint32_t size;
	};

	enum DataType {
		Text = RWP_MSG_DATA_TYPE_TEXT,
		Unknown = RWP_MSG_DATA_TYPE_UNKNOWN
	};

	struct Msg {
		~Msg() {
			release();
		}

		MsgHeader* header;
		MsgData data;
		uint32_t crc;

		void release();

		uint32_t dataSize() const;
		DataType dataType() const;

		std::any decodeData(DataType __type) {
			if (__type == DataType::Text) {
				RWPUtil::EncodedBytes encoded(data, dataSize());
				return RWPUtil::decodeStr(&encoded);
			}

			return nullptr;
		}
		
		static Msg create(std::string __strData, uint32_t __userId, uint32_t __recipientId);
		static Msg create(uint8_t* __data, uint32_t __size, uint32_t __userId, uint32_t __recipientId, uint32_t __dataType);
		static BinaryMsg encode(Msg* __msg);
		static Msg decode(uint8_t* __data);
	};
}