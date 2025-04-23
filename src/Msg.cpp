#include "../include/private/RWP/Msg.hpp"

#include "../include/private/RWP/Crypt.hpp"

#include <vector>
#include <stdexcept>

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

		msg.header = header;
		msg.data = msgData;
		msg.crc = RWPCrypt::crc32(msgData, header->dataSize);

		return msg;
	}

	BinaryMsg Msg::encode(Msg* __msg) {
		std::vector<uint8_t> buffer = std::vector<uint8_t>();

		RWPUtil::EncodedBytes encoded = RWPUtil::encodeNumber(__msg->header->headerSize);
		RWPUtil::useBuffer(&buffer, &encoded);
		encoded = RWPUtil::encodeNumber(__msg->header->userId);
		RWPUtil::useBuffer(&buffer, &encoded);
		encoded = RWPUtil::encodeNumber(__msg->header->recipientId);
		RWPUtil::useBuffer(&buffer, &encoded);
		encoded = RWPUtil::encodeNumber(__msg->header->dataType);
		RWPUtil::useBuffer(&buffer, &encoded);
		encoded = RWPUtil::encodeNumber(__msg->header->dataSize);
		RWPUtil::useBuffer(&buffer, &encoded);
		__msg->header->crc = RWPCrypt::crc32(encoded.bytes, __msg->header->headerSize);
		encoded = RWPUtil::encodeNumber(__msg->header->crc);
		RWPUtil::useBuffer(&buffer, &encoded);

		RWPUtil::useBuffer(&buffer, __msg->data, __msg->header->dataSize);
		encoded = RWPUtil::encodeNumber(__msg->crc);
		RWPUtil::useBuffer(&buffer, &encoded);

		return BinaryMsg(buffer.data(), buffer.size());
	}

	Msg Msg::decode(uint8_t* __data) {
		Msg msg = Msg();
		msg.header = new MsgHeader();

		msg.header->headerSize = RWPUtil::decodeNumber<uint32_t>(__data);
		uint8_t* header = new uint8_t[msg.header->headerSize];
		memcpy(header, __data, msg.header->headerSize);

		uint8_t* ptr = __data + sizeof(uint32_t);
		msg.header->userId = RWPUtil::decodeNumber<uint32_t>(ptr);
		ptr += sizeof(uint32_t);
		msg.header->recipientId = RWPUtil::decodeNumber<uint32_t>(ptr);
		ptr += sizeof(uint32_t);
		msg.header->dataType = RWPUtil::decodeNumber<uint32_t>(ptr);
		ptr += sizeof(uint32_t);
		msg.header->dataSize = RWPUtil::decodeNumber<uint32_t>(ptr);
		ptr += sizeof(uint32_t);
		msg.header->crc = RWPUtil::decodeNumber<uint32_t>(ptr);
		ptr += sizeof(uint32_t);

		if (RWPCrypt::crc32(header, msg.header->headerSize) != msg.header->crc) {
			throw std::runtime_error("Header crc in unvalid");
		}
		
		msg.data = new uint8_t[msg.header->dataSize];
		memcpy(msg.data, ptr, msg.header->dataSize);
		ptr += msg.header->dataSize;
		uint8_t* binData = ptr;

		msg.crc = RWPUtil::decodeNumber<uint32_t>(ptr);

		if (RWPCrypt::crc32(binData, msg.header->dataSize) != msg.crc) {
			throw std::runtime_error("Data crc in unvalid");
		}

		return msg;
	}

	void Msg::release() {
		delete header;
		delete[] data;
	}

	uint32_t Msg::dataSize() const {
		return header->dataSize;
	}

	DataType Msg::dataType() const {
		switch (header->dataType) {
			case RWP_MSG_DATA_TYPE_TEXT: return DataType::Text;
			default: return DataType::Unknown;
		}
	}
}