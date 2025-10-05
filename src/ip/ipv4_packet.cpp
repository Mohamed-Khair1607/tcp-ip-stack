#include "ip/ipv4_packet.h"
#include "ip/checksum.h"
#include <cstring>
#include <iostream>
#include <cstddef>

void IPv4Packet::set_version_ihl(uint8_t version, uint8_t ihl) {
    header_.version_ihl = (version << 4) | (ihl & 0x0F);
}

void IPv4Packet::set_protocol(uint8_t protocol) {
    header_.protocol = protocol;
}

void IPv4Packet::set_source_ip(const std::array<uint8_t, 4>& ip) {
    header_.source_ip = ip;
}

void IPv4Packet::set_destination_ip(const std::array<uint8_t, 4>& ip) {
    header_.dest_ip = ip;
}

void IPv4Packet::set_payload(const std::vector<uint8_t>& payload) {
    payload_ = payload;
    header_.total_length = 20 + payload_.size(); // 20 bytes header + payload
}

void IPv4Packet::set_ttl(uint8_t ttl) {
    header_.ttl = ttl;
}

uint16_t IPv4Packet::calculate_checksum() const {
    // Make a copy of the header for checksum calculation
    IPv4Header temp_header = header_;
    temp_header.header_checksum = 0; // Must be zero for calculation
    
    std::vector<uint8_t> header_bytes;
    header_bytes.reserve(20); // IPv4 header is 20 bytes without options
    
    // Serialize header for checksum calculation
    header_bytes.push_back(temp_header.version_ihl);
    header_bytes.push_back(temp_header.dscp_ecn);
    header_bytes.push_back(static_cast<uint8_t>((temp_header.total_length >> 8) & 0xFF));
    header_bytes.push_back(static_cast<uint8_t>(temp_header.total_length & 0xFF));
    header_bytes.push_back(static_cast<uint8_t>((temp_header.identification >> 8) & 0xFF));
    header_bytes.push_back(static_cast<uint8_t>(temp_header.identification & 0xFF));
    header_bytes.push_back(static_cast<uint8_t>((temp_header.flags_fragment_offset >> 8) & 0xFF));
    header_bytes.push_back(static_cast<uint8_t>(temp_header.flags_fragment_offset & 0xFF));
    header_bytes.push_back(temp_header.ttl);
    header_bytes.push_back(temp_header.protocol);
    header_bytes.push_back(0); // checksum high byte (zero for calculation)
    header_bytes.push_back(0); // checksum low byte (zero for calculation)
    
    for (auto byte : temp_header.source_ip) {
        header_bytes.push_back(byte);
    }
    
    for (auto byte : temp_header.dest_ip) {
        header_bytes.push_back(byte);
    }
    
    return calculate_checksum(header_bytes);
}

std::vector<uint8_t> IPv4Packet::serialize() const {
    std::vector<uint8_t> packet;
    packet.reserve(20 + payload_.size());
    
    // Calculate checksum
    uint16_t checksum = calculate_checksum();
    
    // Version and IHL (assuming 20 byte header, so IHL=5)
    packet.push_back(0x45); // Version=4, IHL=5
    
    // DSCP and ECN
    packet.push_back(header_.dscp_ecn);
    
    // Total Length
    packet.push_back(static_cast<uint8_t>((header_.total_length >> 8) & 0xFF));
    packet.push_back(static_cast<uint8_t>(header_.total_length & 0xFF));
    
    // Identification
    packet.push_back(static_cast<uint8_t>((header_.identification >> 8) & 0xFF));
    packet.push_back(static_cast<uint8_t>(header_.identification & 0xFF));
    
    // Flags and Fragment Offset
    packet.push_back(static_cast<uint8_t>((header_.flags_fragment_offset >> 8) & 0xFF));
    packet.push_back(static_cast<uint8_t>(header_.flags_fragment_offset & 0xFF));
    
    // TTL
    packet.push_back(header_.ttl);
    
    // Protocol
    packet.push_back(header_.protocol);
    
    // Header Checksum
    packet.push_back(static_cast<uint8_t>((checksum >> 8) & 0xFF));
    packet.push_back(static_cast<uint8_t>(checksum & 0xFF));
    
    // Source IP
    for (auto byte : header_.source_ip) {
        packet.push_back(byte);
    }
    
    // Destination IP
    for (auto byte : header_.dest_ip) {
        packet.push_back(byte);
    }
    
    // Payload
    packet.insert(packet.end(), payload_.begin(), payload_.end());
    
    return packet;
}

bool IPv4Packet::deserialize(const std::vector<uint8_t>& data) {
    if (data.size() < 20) {
        std::cerr << "IP packet too small: " << data.size() << " bytes" << std::endl;
        return false;
    }
    
    // Version and IHL
    header_.version_ihl = data[0];
    
    // DSCP and ECN
    header_.dscp_ecn = data[1];
    
    // Total Length
    header_.total_length = (static_cast<uint16_t>(data[2]) << 8) | data[3];
    
    // Identification
    header_.identification = (static_cast<uint16_t>(data[4]) << 8) | data[5];
    
    // Flags and Fragment Offset
    header_.flags_fragment_offset = (static_cast<uint16_t>(data[6]) << 8) | data[7];
    
    // TTL
    header_.ttl = data[8];
    
    // Protocol
    header_.protocol = data[9];
    
    // Header Checksum
    header_.header_checksum = (static_cast<uint16_t>(data[10]) << 8) | data[11];
    
    // Source IP
    for (int i = 0; i < 4; ++i) {
        header_.source_ip[i] = data[12 + i];
    }
    
    // Destination IP
    for (int i = 0; i < 4; ++i) {
        header_.dest_ip[i] = data[16 + i];
    }
    
    // Payload
    if (data.size() > 20) {
        payload_.assign(data.begin() + 20, data.end());
    } else {
        payload_.clear();
    }
    
    return true;
}
