#pragma once
#include <cstdint>

enum class TCPState {
    CLOSED,
    LISTEN,
    SYN_SENT,
    SYN_RECEIVED,
    ESTABLISHED,
    FIN_WAIT_1,
    FIN_WAIT_2,
    CLOSE_WAIT,
    CLOSING,
    LAST_ACK,
    TIME_WAIT
};

class TCPStateMachine {
public:
    TCPStateMachine();
    
    void handle_syn();
    void handle_syn_ack();
    void handle_ack();
    void handle_fin();
    void handle_rst();
    
    void send_syn();
    void send_ack();
    void send_fin();
    
    TCPState get_state() const { return current_state_; }
    const char* get_state_name() const;

private:
    TCPState current_state_ = TCPState::CLOSED;
    
    void transition_to(TCPState new_state);
};
