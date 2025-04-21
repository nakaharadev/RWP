#include "../include/private/RWP/Crypt.hpp"

namespace RWPCrypt {
    struct CRC32Table {
        uint32_t table[256];

        constexpr uint32_t generate_crc32_table_entry(uint32_t index) {
            uint32_t crc = index;
            for (int j = 0; j < 8; ++j) {
                crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
            }
            return crc;
        }

        constexpr CRC32Table() : table() {
            for (uint32_t i = 0; i < 256; ++i) {
                table[i] = generate_crc32_table_entry(i);
            }
        }
    };

    static constexpr CRC32Table crc32_table;

	uint32_t crc32(uint8_t* __data, uint32_t __size) {
        uint32_t crc = 0xFFFFFFFF;

        for (size_t i = 0; i < __size; ++i) {
            uint8_t byte = __data[i];
            uint32_t table_index = (crc ^ byte) & 0xFF;
            crc = (crc >> 8) ^ crc32_table.table[table_index];
        }

        return ~crc;
	}
}