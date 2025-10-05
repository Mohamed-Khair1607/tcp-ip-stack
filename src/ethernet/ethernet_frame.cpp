#include "ethernet/ethernet_frame.h"
#include <cstring>
#include <iostream>
#include <cstddef>


EthernetFrame::EthernetFrame(const std::vector<uint8_t>& data) {
    deserialize(data);
}

void EthernetFrame::set_destination_mac(const std::array<uint8_t, 6>& mac) {
    header_.dest_mac = mac;
}

void EthernetFrame::set_source_mac(const std::array<uint8_t, 6>& mac) {
    header_.src_mac = mac;
}

void EthernetFrame::set_ethertype(uint16_t type) {
    header_.ethertype = type;
}

void EthernetFrame::set_payload(const std::vector<uint8_t>& payload) {
    payload_ = payload;
}

std::vector<uint8_t> EthernetFrame::serialize() const {
    std::vector<uint8_t> frame;
    frame.reserve(14 + payload_.size()); // 14 bytes header + payload
    
    // Destination MAC
    for (auto byte : header_.dest_mac) {
        frame.push_back(byte);
    }
    
    // Source MAC
    for (auto byte : header_.src_mac) {
        frame.push_back(byte);
    }
    
    // Ethertype (big-endian)
    frame.push_back(static_cast<uint8_t>((header_.ethertype >> 8) & 0xFF));
    frame.push_back(static_cast<uint8_t>(header_.ethertype & 0xFF));
    
    // Payload
    frame.insert(frame.end(), payload_.begin(), payload_.end());
    
    return frame;
}

bool EthernetFrame::deserialize(const std::vector<uint8_t>& data) {
    if (data.size() < 14) {
        std::cerr << "Ethernet frame too small: " << data.size() << " bytes" << std::endl;
        return false;
    }
    
    // Extract destination MAC
    for (int i = 0; i < 6; ++i) {
        header_.dest_mac[i] = data[i];
    }
    
    // Extract source MAC
    for (int i = 0; i < 6; ++i) {
        header_.src_mac[i] = data[i + 6];
    }
    
    // Extract ethertype
    header_.ethertype = (data[12] << 8) | data[13];
    
    // Extract payload
    if (data.size() > 14) {
        payload_.assign(data.begin() + 14, data.end());
    } else {
        payload_.clear();
    }
    
    return true;
}
