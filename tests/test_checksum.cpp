#include <gtest/gtest.h>
#include "ip/checksum.h"

TEST(ChecksumTest, BasicCalculation) {
    std::vector<uint8_t> data = {0x45, 0x00, 0x00, 0x73, 0x00, 0x00, 0x40, 0x00, 
                                0x40, 0x11, 0x00, 0x00, 0xc0, 0xa8, 0x00, 0x01, 
                                0xc0, 0xa8, 0x00, 0xc7};
    
    uint16_t checksum = calculate_checksum(data);
    EXPECT_EQ(checksum, 0xb861);
}

TEST(ChecksumTest, ZeroData) {
    std::vector<uint8_t> data(20, 0);
    uint16_t checksum = calculate_checksum(data);
    EXPECT_EQ(checksum, 0xFFFF);
}
