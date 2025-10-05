#include <iostream>
#include "tcp/tcp_state_machine.h"

void demo_tcp_handshake() {
    std::cout << "=== TCP 3-Way Handshake Demo ===" << std::endl;
    
    TCPStateMachine sm;
    
    std::cout << "\n1. Client sends SYN" << std::endl;
    sm.send_syn();
    
    std::cout << "\n2. Server receives SYN, sends SYN-ACK" << std::endl;
    sm.handle_syn_ack();
    
    std::cout << "\n3. Client sends ACK" << std::endl;
    sm.handle_ack();
    
    std::cout << "\n4. Connection is now ESTABLISHED" << std::endl;
    std::cout << "Current state: " << sm.get_state_name() << std::endl;
}

void demo_tcp_teardown() {
    std::cout << "\n=== TCP Connection Teardown Demo ===" << std::endl;
    
    TCPStateMachine sm;
    
    // Simulate established connection
    std::cout << "\nStarting from ESTABLISHED state..." << std::endl;
    // We'll manually set up the state for demo purposes
    // In real code, this would happen through the state transitions
    
    std::cout << "\n1. Client sends FIN" << std::endl;
    sm.send_fin();
    
    std::cout << "\n2. Server acknowledges FIN" << std::endl;
    sm.handle_ack();
    
    std::cout << "\n3. Server sends its own FIN" << std::endl;
    sm.handle_fin();
    
    std::cout << "\n4. Client sends final ACK" << std::endl;
    sm.send_ack();
}

int main() {
    std::cout << "=========================================" << std::endl;
    std::cout << "    TCP State Machine Demonstration" << std::endl;
    std::cout << "=========================================" << std::endl;
    
    demo_tcp_handshake();
    demo_tcp_teardown();
    
    std::cout << "\n=========================================" << std::endl;
    std::cout << "✓ State machine demo completed!" << std::endl;
    std::cout << "=========================================" << std::endl;
    return 0;
}
