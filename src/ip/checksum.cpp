#include "ip/checksum.h"
#include <cstdint>
#include <vector>

uint16_t calculate_checksum(const std::vector<uint8_t>& data) {
    uint32_t sum = 0;
    auto length = data.size();  // Use auto to avoid type issues
    
    // Sum up 16-bit words
    for (decltype(length) i = 0; i + 1 < length; i += 2) {
        sum += (static_cast<uint16_t>(data[i]) << 8) | data[i + 1];
    }
    
    // Handle odd length
    if (length % 2 != 0) {
        sum += static_cast<uint16_t>(data[length - 1]) << 8;
    }
    
    // Fold 32-bit sum to 16 bits
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    
    return static_cast<uint16_t>(~sum);
}
