#include "stack.h"
#include <iostream>
#include <pcap.h>

TCPIPStack::TCPIPStack(const std::string& interface) : interface_(interface) {}

TCPIPStack::~TCPIPStack() {
    stop();
}

bool TCPIPStack::start() {
    if (running_) {
        std::cout << "Stack is already running" << std::endl;
        return false;
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(interface_.c_str(), BUFSIZ, 1, 1000, errbuf);
    
    if (handle == nullptr) {
        std::cerr << "Couldn't open device " << interface_ << ": " << errbuf << std::endl;
        return false;
    }
    
    pcap_close(handle);
    
    running_ = true;
    capture_thread_ = std::thread(&TCPIPStack::capture_loop, this);
    
    std::cout << "TCP/IP Stack started on interface: " << interface_ << std::endl;
    return true;
}

void TCPIPStack::stop() {
    if (!running_) return;
    
    running_ = false;
    if (capture_thread_.joinable()) {
        capture_thread_.join();
    }
    
    std::cout << "TCP/IP Stack stopped" << std::endl;
}

void TCPIPStack::capture_loop() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(interface_.c_str(), BUFSIZ, 1, 1000, errbuf);
    
    if (handle == nullptr) {
        std::cerr << "Couldn't open device in capture thread: " << errbuf << std::endl;
        return;
    }
    
    struct pcap_pkthdr header;
    const u_char* packet;
    
    std::cout << "Capture thread started" << std::endl;
    
    while (running_) {
        packet = pcap_next(handle, &header);
        if (packet != nullptr) {
            std::vector<uint8_t> packet_data(packet, packet + header.len);
            process_packet(packet_data);
        }
    }
    
    pcap_close(handle);
}

void TCPIPStack::process_packet(const std::vector<uint8_t>& packet_data) {
    // Basic packet processing - just print size for now
    std::cout << "Received packet: " << packet_data.size() << " bytes" << std::endl;
    
    // Here you would:
    // 1. Parse as Ethernet frame
    // 2. Check EtherType and process accordingly
    // 3. Handle IP packets, TCP segments, etc.
}
