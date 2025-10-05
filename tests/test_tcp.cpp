#include <gtest/gtest.h>
#include "tcp/tcp_segment.h"

TEST(TCPSegmentTest, SYNFlag) {
    TCPSegment segment;
    segment.set_flags(TCPSegment::SYN);
    
    auto serialized = segment.serialize();
    EXPECT_FALSE(serialized.empty());
    
    TCPSegment parsed;
    bool success = parsed.deserialize(serialized);
    EXPECT_TRUE(success);
    EXPECT_EQ(parsed.get_header().flags & TCPSegment::SYN, TCPSegment::SYN);
}
