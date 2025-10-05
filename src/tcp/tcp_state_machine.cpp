#include "tcp/tcp_state_machine.h"
#include <iostream>

TCPStateMachine::TCPStateMachine() : current_state_(TCPState::CLOSED) {}

void TCPStateMachine::handle_syn() {
    std::cout << "State " << get_state_name() << ": Handling SYN" << std::endl;
    
    switch (current_state_) {
        case TCPState::LISTEN:
            transition_to(TCPState::SYN_RECEIVED);
            std::cout << "  -> Sent SYN-ACK" << std::endl;
            break;
        default:
            std::cout << "  -> Unexpected SYN in state: " << get_state_name() << std::endl;
            break;
    }
}

void TCPStateMachine::handle_syn_ack() {
    std::cout << "State " << get_state_name() << ": Handling SYN-ACK" << std::endl;
    
    switch (current_state_) {
        case TCPState::SYN_SENT:
            transition_to(TCPState::ESTABLISHED);
            std::cout << "  -> Sent ACK, connection established" << std::endl;
            break;
        default:
            std::cout << "  -> Unexpected SYN-ACK in state: " << get_state_name() << std::endl;
            break;
    }
}

void TCPStateMachine::handle_ack() {
    std::cout << "State " << get_state_name() << ": Handling ACK" << std::endl;
    
    switch (current_state_) {
        case TCPState::SYN_RECEIVED:
            transition_to(TCPState::ESTABLISHED);
            std::cout << "  -> Connection established" << std::endl;
            break;
        default:
            std::cout << "  -> ACK processed in state: " << get_state_name() << std::endl;
            break;
    }
}

void TCPStateMachine::handle_fin() {
    std::cout << "State " << get_state_name() << ": Handling FIN" << std::endl;
    
    switch (current_state_) {
        case TCPState::ESTABLISHED:
            transition_to(TCPState::CLOSE_WAIT);
            std::cout << "  -> Sent ACK for FIN" << std::endl;
            break;
        case TCPState::FIN_WAIT_1:
            transition_to(TCPState::CLOSING);
            std::cout << "  -> Simultaneous close" << std::endl;
            break;
        case TCPState::FIN_WAIT_2:
            transition_to(TCPState::TIME_WAIT);
            std::cout << "  -> Connection closing" << std::endl;
            break;
        default:
            std::cout << "  -> Unexpected FIN in state: " << get_state_name() << std::endl;
            break;
    }
}

void TCPStateMachine::handle_rst() {
    std::cout << "State " << get_state_name() << ": Handling RST" << std::endl;
    transition_to(TCPState::CLOSED);
    std::cout << "  -> Connection reset" << std::endl;
}

void TCPStateMachine::send_syn() {
    if (current_state_ == TCPState::CLOSED) {
        transition_to(TCPState::SYN_SENT);
        std::cout << "State " << get_state_name() << ": Sent SYN" << std::endl;
    }
}

void TCPStateMachine::send_ack() {
    std::cout << "State " << get_state_name() << ": Sent ACK" << std::endl;
}

void TCPStateMachine::send_fin() {
    switch (current_state_) {
        case TCPState::ESTABLISHED:
            transition_to(TCPState::FIN_WAIT_1);
            std::cout << "State " << get_state_name() << ": Sent FIN" << std::endl;
            break;
        case TCPState::CLOSE_WAIT:
            transition_to(TCPState::LAST_ACK);
            std::cout << "State " << get_state_name() << ": Sent FIN" << std::endl;
            break;
        default:
            std::cout << "  -> Cannot send FIN in state: " << get_state_name() << std::endl;
            break;
    }
}

const char* TCPStateMachine::get_state_name() const {
    static const char* names[] = {
        "CLOSED", "LISTEN", "SYN_SENT", "SYN_RECEIVED", "ESTABLISHED",
        "FIN_WAIT_1", "FIN_WAIT_2", "CLOSE_WAIT", "CLOSING", "LAST_ACK", "TIME_WAIT"
    };
    return names[static_cast<int>(current_state_)];
}

void TCPStateMachine::transition_to(TCPState new_state) {
    std::cout << "TCP State transition: " << get_state_name() << " -> " 
              << names[static_cast<int>(new_state)] << std::endl;
    current_state_ = new_state;
}
