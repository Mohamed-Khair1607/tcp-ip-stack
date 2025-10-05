#pragma once
#include <string>
#include <memory>
#include <thread>
#include <atomic>

class TCPIPStack {
public:
    TCPIPStack(const std::string& interface);
    ~TCPIPStack();
    
    bool start();
    void stop();
    
private:
    std::string interface_;
    std::atomic<bool> running_{false};
    std::thread capture_thread_;
    
    void capture_loop();
    void process_packet(const std::vector<uint8_t>& packet_data);
};
