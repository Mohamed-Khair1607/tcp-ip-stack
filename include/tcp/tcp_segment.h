#pragma once
#include <cstdint>
#include <vector>

struct TCPHeader {
    uint16_t source_port;
    uint16_t dest_port;
    uint32_t sequence_number;
    uint32_t acknowledgment_number;
    uint8_t data_offset;
    uint8_t flags;
    uint16_t window_size;
    uint16_t checksum;
    uint16_t urgent_pointer;
};

class TCPSegment {
public:
    TCPSegment() = default;
    
    void set_source_port(uint16_t port);
    void set_dest_port(uint16_t port);
    void set_sequence_number(uint32_t seq);
    void set_ack_number(uint32_t ack);
    void set_flags(uint8_t flags);
    void set_window_size(uint16_t window);
    void set_payload(const std::vector<uint8_t>& payload);
    
    // TCP Flags
    static constexpr uint8_t FIN = 0x01;
    static constexpr uint8_t SYN = 0x02;
    static constexpr uint8_t RST = 0x04;
    static constexpr uint8_t PSH = 0x08;
    static constexpr uint8_t ACK = 0x10;
    static constexpr uint8_t URG = 0x20;
    
    std::vector<uint8_t> serialize() const;
    bool deserialize(const std::vector<uint8_t>& data);
    
    uint16_t calculate_checksum(const std::array<uint8_t, 4>& source_ip, 
                               const std::array<uint8_t, 4>& dest_ip) const;
    
    const TCPHeader& get_header() const { return header_; }
    const std::vector<uint8_t>& get_payload() const { return payload_; }

private:
    TCPHeader header_;
    std::vector<uint8_t> payload_;
};
