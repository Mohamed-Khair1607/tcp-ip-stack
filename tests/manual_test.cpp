#include <iostream>
#include <cassert>
#include "ip/checksum.h"
#include "ethernet/ethernet_frame.h"
#include "ip/ipv4_packet.h"
#include "tcp/tcp_segment.h"

void test_checksum() {
    std::cout << "Testing checksum calculation..." << std::endl;
    
    // Test case from RFC 1071
    std::vector<uint8_t> data = {0x00, 0x01, 0xf2, 0x03, 0xf4, 0xf5, 0xf6, 0xf7};
    uint16_t result = calculate_checksum(data);
    
    std::cout << "Calculated: 0x" << std::hex << result << std::dec << std::endl;
    std::cout << "Expected:   0xddf2" << std::endl;
    
    if (result == 0xddf2) {
        std::cout << "✓ Checksum test PASSED" << std::endl;
    } else {
        std::cout << "✗ Checksum test FAILED" << std::endl;
        exit(1);
    }
}

void test_ethernet_roundtrip() {
    std::cout << "\nTesting Ethernet frame serialization/deserialization..." << std::endl;
    
    EthernetFrame frame1;
    std::array<uint8_t, 6> mac1 = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
    std::array<uint8_t, 6> mac2 = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    
    frame1.set_source_mac(mac1);
    frame1.set_destination_mac(mac2);
    frame1.set_ethertype(0x0800);
    frame1.set_payload({1, 2, 3, 4, 5});
    
    auto data = frame1.serialize();
    EthernetFrame frame2;
    bool success = frame2.deserialize(data);
    
    if (success && frame2.get_header().src_mac == mac1) {
        std::cout << "✓ Ethernet roundtrip test PASSED" << std::endl;
    } else {
        std::cout << "✗ Ethernet roundtrip test FAILED" << std::endl;
        exit(1);
    }
}

void test_ip_packet() {
    std::cout << "\nTesting IP packet creation..." << std::endl;
    
    IPv4Packet packet;
    std::array<uint8_t, 4> src_ip = {192, 168, 1, 1};
    std::array<uint8_t, 4> dst_ip = {10, 0, 0, 1};
    
    packet.set_source_ip(src_ip);
    packet.set_destination_ip(dst_ip);
    packet.set_protocol(IPv4Packet::PROTOCOL_TCP);
    packet.set_ttl(64);
    packet.set_payload({0x54, 0x65, 0x73, 0x74}); // "Test"
    
    auto data = packet.serialize();
    
    if (data.size() >= 20) {
        std::cout << "✓ IP packet test PASSED (size: " << data.size() << " bytes)" << std::endl;
    } else {
        std::cout << "✗ IP packet test FAILED" << std::endl;
        exit(1);
    }
}

void test_tcp_segment() {
    std::cout << "\nTesting TCP segment creation..." << std::endl;
    
    TCPSegment segment;
    segment.set_source_port(8080);
    segment.set_dest_port(80);
    segment.set_sequence_number(123456);
    segment.set_ack_number(654321);
    segment.set_flags(TCPSegment::SYN | TCPSegment::ACK);
    segment.set_window_size(65535);
    segment.set_payload({0x48, 0x65, 0x6C, 0x6C, 0x6F}); // "Hello"
    
    auto data = segment.serialize();
    
    if (data.size() >= 20) {
        std::cout << "✓ TCP segment test PASSED (size: " << data.size() << " bytes)" << std::endl;
        
        // Test flags
        auto header = segment.get_header();
        bool has_syn = header.flags & TCPSegment::SYN;
        bool has_ack = header.flags & TCPSegment::ACK;
        
        if (has_syn && has_ack) {
            std::cout << "✓ TCP flags test PASSED (SYN+ACK set correctly)" << std::endl;
        } else {
            std::cout << "✗ TCP flags test FAILED" << std::endl;
            exit(1);
        }
    } else {
        std::cout << "✗ TCP segment test FAILED" << std::endl;
        exit(1);
    }
}

int main() {
    std::cout << "=========================================" << std::endl;
    std::cout << "    Manual TCP/IP Stack Tests" << std::endl;
    std::cout << "=========================================" << std::endl;
    
    test_checksum();
    test_ethernet_roundtrip();
    test_ip_packet();
    test_tcp_segment();
    
    std::cout << "\n=========================================" << std::endl;
    std::cout << "✓ ALL TESTS PASSED!" << std::endl;
    std::cout << "=========================================" << std::endl;
    
    return 0;
}
