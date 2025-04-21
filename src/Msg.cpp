#include "../include/private/RWP/Msg.hpp"

#include "../include/private/RWP/Crypt.hpp"

#define RWP_MSG_DATA_TYPE_TEXT 0x1

namespace RWP {
#pragma pack(push, 1)
	struct MsgHeader {
		uint32_t headerSize;
		uint32_t userId;
		uint32_t recipientId;
		uint32_t dataType;
		uint32_t dataSize;
		uint32_t crc;
	};
#pragma pack(pop)

	Msg Msg::create(std::string __strData, uint32_t __userId, uint32_t __recipientId) {
		uint8_t* encoded = new uint8_t[__strData.length()];
		memcpy(encoded, __strData.data(), __strData.length());

		Msg msg = create(encoded, __strData.length(), __userId, __recipientId, RWP_MSG_DATA_TYPE_TEXT);
		delete[] encoded;
		
		return msg;
	}
	
	Msg Msg::create(uint8_t* __data, uint32_t __size, uint32_t __userId, uint32_t __recipientId, uint32_t __dataType) {
		Msg msg = Msg();

		MsgHeader* header = new MsgHeader();
		MsgData msgData = new uint8_t[__size];
		memcpy(msgData, __data, __size);

		header->headerSize = sizeof(uint32_t) * 6;
		header->userId = __userId;
		header->recipientId = __recipientId;
		header->dataType = __dataType;
		header->dataSize = __size;
		header->crc = RWPCrypt::crc32(msgData, __size);

		msg.header = header;
		msg.data = msgData;
		msg.crc = RWPCrypt::crc32(__data, header->dataSize);
	}

	void Msg::release() {
		delete header;
		delete data;
	}
}