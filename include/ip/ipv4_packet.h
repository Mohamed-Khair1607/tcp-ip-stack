#pragma once
#include <cstdint>
#include <vector>
#include <array>

struct IPv4Header {
    uint8_t version_ihl;
    uint8_t dscp_ecn;
    uint16_t total_length;
    uint16_t identification;
    uint16_t flags_fragment_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t header_checksum;
    std::array<uint8_t, 4> source_ip;
    std::array<uint8_t, 4> dest_ip;
};

class IPv4Packet {
public:
    IPv4Packet() = default;
    
    void set_version_ihl(uint8_t version, uint8_t ihl);
    void set_protocol(uint8_t protocol);
    void set_source_ip(const std::array<uint8_t, 4>& ip);
    void set_destination_ip(const std::array<uint8_t, 4>& ip);
    void set_payload(const std::vector<uint8_t>& payload);
    void set_ttl(uint8_t ttl);
    
    uint16_t calculate_checksum() const;
    std::vector<uint8_t> serialize() const;
    bool deserialize(const std::vector<uint8_t>& data);
    
    const IPv4Header& get_header() const { return header_; }
    const std::vector<uint8_t>& get_payload() const { return payload_; }

    static constexpr uint8_t PROTOCOL_TCP = 6;
    static constexpr uint8_t PROTOCOL_UDP = 17;

private:
    IPv4Header header_;
    std::vector<uint8_t> payload_;
};
