#include "tcp/tcp_segment.h"
#include "ip/checksum.h"
#include <cstring>
#include <iostream>
#include <cstddef>

void TCPSegment::set_source_port(uint16_t port) {
    header_.source_port = port;
}

void TCPSegment::set_dest_port(uint16_t port) {
    header_.dest_port = port;
}

void TCPSegment::set_sequence_number(uint32_t seq) {
    header_.sequence_number = seq;
}

void TCPSegment::set_ack_number(uint32_t ack) {
    header_.acknowledgment_number = ack;
}

void TCPSegment::set_flags(uint8_t flags) {
    header_.flags = flags;
}

void TCPSegment::set_window_size(uint16_t window) {
    header_.window_size = window;
}

void TCPSegment::set_payload(const std::vector<uint8_t>& payload) {
    payload_ = payload;
}

std::vector<uint8_t> TCPSegment::serialize() const {
    std::vector<uint8_t> segment;
    size_t header_size = 20; // TCP header without options
    segment.reserve(header_size + payload_.size());
    
    // Source Port
    segment.push_back(static_cast<uint8_t>((header_.source_port >> 8) & 0xFF));
    segment.push_back(static_cast<uint8_t>(header_.source_port & 0xFF));
    
    // Destination Port
    segment.push_back(static_cast<uint8_t>((header_.dest_port >> 8) & 0xFF));
    segment.push_back(static_cast<uint8_t>(header_.dest_port & 0xFF));
    
    // Sequence Number
    segment.push_back(static_cast<uint8_t>((header_.sequence_number >> 24) & 0xFF));
    segment.push_back(static_cast<uint8_t>((header_.sequence_number >> 16) & 0xFF));
    segment.push_back(static_cast<uint8_t>((header_.sequence_number >> 8) & 0xFF));
    segment.push_back(static_cast<uint8_t>(header_.sequence_number & 0xFF));
    
    // Acknowledgment Number
    segment.push_back(static_cast<uint8_t>((header_.acknowledgment_number >> 24) & 0xFF));
    segment.push_back(static_cast<uint8_t>((header_.acknowledgment_number >> 16) & 0xFF));
    segment.push_back(static_cast<uint8_t>((header_.acknowledgment_number >> 8) & 0xFF));
    segment.push_back(static_cast<uint8_t>(header_.acknowledgment_number & 0xFF));
    
    // Data Offset and Reserved (Data Offset = 5 for 20 byte header)
    segment.push_back(0x50); // Data Offset = 5, Reserved = 0
    
    // Flags
    segment.push_back(header_.flags);
    
    // Window Size
    segment.push_back(static_cast<uint8_t>((header_.window_size >> 8) & 0xFF));
    segment.push_back(static_cast<uint8_t>(header_.window_size & 0xFF));
    
    // Checksum (will be calculated later, set to 0 for now)
    segment.push_back(0);
    segment.push_back(0);
    
    // Urgent Pointer
    segment.push_back(static_cast<uint8_t>((header_.urgent_pointer >> 8) & 0xFF));
    segment.push_back(static_cast<uint8_t>(header_.urgent_pointer & 0xFF));
    
    // Payload
    segment.insert(segment.end(), payload_.begin(), payload_.end());
    
    return segment;
}

bool TCPSegment::deserialize(const std::vector<uint8_t>& data) {
    if (data.size() < 20) {
        std::cerr << "TCP segment too small: " << data.size() << " bytes" << std::endl;
        return false;
    }
    
    // Source Port
    header_.source_port = (static_cast<uint16_t>(data[0]) << 8) | data[1];
    
    // Destination Port
    header_.dest_port = (static_cast<uint16_t>(data[2]) << 8) | data[3];
    
    // Sequence Number
    header_.sequence_number = (static_cast<uint32_t>(data[4]) << 24) |
                             (static_cast<uint32_t>(data[5]) << 16) |
                             (static_cast<uint32_t>(data[6]) << 8) |
                             data[7];
    
    // Acknowledgment Number
    header_.acknowledgment_number = (static_cast<uint32_t>(data[8]) << 24) |
                                   (static_cast<uint32_t>(data[9]) << 16) |
                                   (static_cast<uint32_t>(data[10]) << 8) |
                                   data[11];
    
    // Data Offset and Reserved
    header_.data_offset = data[12] >> 4;
    
    // Flags
    header_.flags = data[13];
    
    // Window Size
    header_.window_size = (static_cast<uint16_t>(data[14]) << 8) | data[15];
    
    // Checksum
    header_.checksum = (static_cast<uint16_t>(data[16]) << 8) | data[17];
    
    // Urgent Pointer
    header_.urgent_pointer = (static_cast<uint16_t>(data[18]) << 8) | data[19];
    
    // Payload
    size_t header_size = header_.data_offset * 4;
    if (data.size() > header_size) {
        payload_.assign(data.begin() + header_size, data.end());
    } else {
        payload_.clear();
    }
    
    return true;
}

uint16_t TCPSegment::calculate_checksum(const std::array<uint8_t, 4>& source_ip, 
                                       const std::array<uint8_t, 4>& dest_ip) const {
    std::vector<uint8_t> pseudo_header;
    
    // Pseudo Header for TCP checksum calculation
    // Source IP
    for (auto byte : source_ip) {
        pseudo_header.push_back(byte);
    }
    
    // Destination IP
    for (auto byte : dest_ip) {
        pseudo_header.push_back(byte);
    }
    
    // Zero byte and Protocol
    pseudo_header.push_back(0);
    pseudo_header.push_back(6); // TCP protocol
    
    // TCP Length
    uint16_t tcp_length = 20 + payload_.size();
    pseudo_header.push_back(static_cast<uint8_t>((tcp_length >> 8) & 0xFF));
    pseudo_header.push_back(static_cast<uint8_t>(tcp_length & 0xFF));
    
    // Get TCP segment without checksum
    auto tcp_data = serialize();
    
    // Set checksum field to zero in the TCP data
    tcp_data[16] = 0;
    tcp_data[17] = 0;
    
    // Combine pseudo header and TCP data
    std::vector<uint8_t> checksum_data;
    checksum_data.reserve(pseudo_header.size() + tcp_data.size());
    checksum_data.insert(checksum_data.end(), pseudo_header.begin(), pseudo_header.end());
    checksum_data.insert(checksum_data.end(), tcp_data.begin(), tcp_data.end());
    
    return calculate_checksum(checksum_data);
}
