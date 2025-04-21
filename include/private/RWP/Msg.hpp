#pragma once

#include <cstdint>
#include <string>

namespace RWP {
	using BinaryMsg = uint8_t*;
	using MsgData = uint8_t*;

	struct MsgHeader;

	struct Msg {
		~Msg() {
			release();
		}

		MsgHeader* header;
		MsgData data;
		uint32_t crc;

		void release();
		
		static Msg create(std::string __strData, uint32_t __userId, uint32_t __recipientId);
		static Msg create(uint8_t* __data, uint32_t __size, uint32_t __userId, uint32_t __recipientId, uint32_t __dataType);
	};
}