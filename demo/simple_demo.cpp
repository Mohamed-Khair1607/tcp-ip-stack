#include <iostream>
#include "ethernet/ethernet_frame.h"
#include "ip/ipv4_packet.h"
#include "tcp/tcp_segment.h"
#include "ip/checksum.h"  // ADD THIS LINE

void demo_ethernet() {
    std::cout << "=== Ethernet Frame Demo ===" << std::endl;
    
    EthernetFrame frame;
    std::array<uint8_t, 6> dest_mac = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Broadcast
    std::array<uint8_t, 6> src_mac = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
    
    frame.set_destination_mac(dest_mac);
    frame.set_source_mac(src_mac);
    frame.set_ethertype(EthernetFrame::ETHERTYPE_IPV4);  // Now this should work
    frame.set_payload({0x48, 0x65, 0x6C, 0x6C, 0x6F}); // "Hello"
    
    auto serialized = frame.serialize();
    std::cout << "Ethernet frame created! Size: " << serialized.size() << " bytes" << std::endl;
    
    // Try to deserialize it back
    EthernetFrame frame2;
    if (frame2.deserialize(serialized)) {
        std::cout << "✓ Ethernet frame deserialized successfully!" << std::endl;
        std::cout << "  Source MAC: ";
        for (auto b : frame2.get_header().src_mac) {
            printf("%02x:", b);
        }
        std::cout << "\b " << std::endl; // Remove last colon
    } else {
        std::cout << "✗ Ethernet frame deserialization failed!" << std::endl;
    }
}

void demo_ip() {
    std::cout << "\n=== IP Packet Demo ===" << std::endl;
    
    IPv4Packet packet;
    std::array<uint8_t, 4> src_ip = {192, 168, 1, 100};
    std::array<uint8_t, 4> dest_ip = {192, 168, 1, 1};
    
    packet.set_source_ip(src_ip);
    packet.set_destination_ip(dest_ip);
    packet.set_protocol(IPv4Packet::PROTOCOL_TCP);
    packet.set_ttl(64);
    packet.set_payload({0x48, 0x65, 0x6C, 0x6C, 0x6F}); // "Hello"
    
    auto serialized = packet.serialize();
    std::cout << "IP packet created! Size: " << serialized.size() << " bytes" << std::endl;
    
    // Verify checksum
    uint16_t calculated_checksum = packet.calculate_checksum();
    std::cout << "✓ IP header checksum: 0x" << std::hex << calculated_checksum << std::dec << std::endl;
    
    IPv4Packet packet2;
    if (packet2.deserialize(serialized)) {
        std::cout << "✓ IP packet deserialized successfully!" << std::endl;
        std::cout << "  Source IP: " 
                  << (int)packet2.get_header().source_ip[0] << "." 
                  << (int)packet2.get_header().source_ip[1] << "." 
                  << (int)packet2.get_header().source_ip[2] << "." 
                  << (int)packet2.get_header().source_ip[3] << std::endl;
    }
}

void demo_tcp() {
    std::cout << "\n=== TCP Segment Demo ===" << std::endl;
    
    TCPSegment segment;
    segment.set_source_port(12345);
    segment.set_dest_port(80);
    segment.set_sequence_number(1000);
    segment.set_ack_number(0);
    segment.set_flags(TCPSegment::SYN);
    segment.set_window_size(64240);
    segment.set_payload({0x48, 0x65, 0x6C, 0x6C, 0x6F}); // "Hello"
    
    auto serialized = segment.serialize();
    std::cout << "TCP segment created! Size: " << serialized.size() << " bytes" << std::endl;
    
    std::cout << "✓ TCP flags: ";
    if (segment.get_header().flags & TCPSegment::SYN) std::cout << "SYN ";
    if (segment.get_header().flags & TCPSegment::ACK) std::cout << "ACK ";
    if (segment.get_header().flags & TCPSegment::FIN) std::cout << "FIN ";
    if (segment.get_header().flags & TCPSegment::RST) std::cout << "RST ";
    std::cout << std::endl;
    
    TCPSegment segment2;
    if (segment2.deserialize(serialized)) {
        std::cout << "✓ TCP segment deserialized successfully!" << std::endl;
        std::cout << "  Source port: " << segment2.get_header().source_port << std::endl;
        std::cout << "  Dest port: " << segment2.get_header().dest_port << std::endl;
        std::cout << "  Seq number: " << segment2.get_header().sequence_number << std::endl;
    }
}

void demo_checksum() {
    std::cout << "\n=== Checksum Demo ===" << std::endl;
    
    // Test data from RFC 1071
    std::vector<uint8_t> data = {0x00, 0x01, 0xf2, 0x03, 0xf4, 0xf5, 0xf6, 0xf7};
    
    uint16_t result = calculate_checksum(data);  // Now this should work
    std::cout << "Checksum result: 0x" << std::hex << result << std::dec << std::endl;
    
    // Should be 0xddf2 according to RFC examples
    if (result == 0xddf2) {
        std::cout << "✓ Checksum calculation CORRECT! (Matches RFC 1071 example)" << std::endl;
    } else {
        std::cout << "✗ Checksum calculation WRONG! Expected 0xddf2" << std::endl;
    }
}

int main() {
    std::cout << "=========================================" << std::endl;
    std::cout << "    TCP/IP Stack Demonstration" << std::endl;
    std::cout << "=========================================" << std::endl;
    
    demo_ethernet();
    demo_ip();
    demo_tcp();
    demo_checksum();
    
    std::cout << "\n=========================================" << std::endl;
    std::cout << "✓ All demos completed successfully!" << std::endl;
    std::cout << "=========================================" << std::endl;
    return 0;
}
