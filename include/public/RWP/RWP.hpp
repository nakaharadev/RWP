#pragma once

#include "../../private/RWP/Crypt.hpp"
#include "../../private/RWP/Util.hpp"
#include "../../private/RWP/Msg.hpp"

namespace RWP {
	enum MsgDataType {
		Text = DataType::Text,
		Unknown = DataType::Unknown
	};

	Msg create(std::string __strData, uint32_t __userId, uint32_t __recipientId) {
		return Msg::create(__strData, __userId, __recipientId);
	}

	Msg create(uint8_t* __data, uint32_t __size, uint32_t __userId, uint32_t __recipientId, uint32_t __dataType) {
		return Msg::create(__data, __size, __userId, __recipientId, __dataType);
	}
}