#!/bin/bash

echo "Simple TCP/IP Stack Builder"

# Create necessary directories
mkdir -p demo tests

# Compile all source files
echo "Compiling source files..."
g++ -std=c++17 -Iinclude -c src/ethernet/ethernet_frame.cpp -o src/ethernet/ethernet_frame.o
g++ -std=c++17 -Iinclude -c src/ip/ipv4_packet.cpp -o src/ip/ipv4_packet.o
g++ -std=c++17 -Iinclude -c src/ip/checksum.cpp -o src/ip/checksum.o
g++ -std=c++17 -Iinclude -c src/tcp/tcp_segment.cpp -o src/tcp/tcp_segment.o
g++ -std=c++17 -Iinclude -c src/tcp/tcp_state_machine.cpp -o src/tcp/tcp_state_machine.o
g++ -std=c++17 -Iinclude -c src/stack.cpp -o src/stack.o

# Build demo
echo "Building demo..."
g++ -std=c++17 -Iinclude -c demo/simple_demo.cpp -o demo/simple_demo.o
g++ -o demo/simple_demo demo/simple_demo.o src/ethernet/ethernet_frame.o src/ip/ipv4_packet.o src/ip/checksum.o src/tcp/tcp_segment.o src/tcp/tcp_state_machine.o src/stack.o -lpcap

# Build state machine demo
echo "Building state machine demo..."
g++ -std=c++17 -Iinclude -c demo/state_machine_demo.cpp -o demo/state_machine_demo.o
g++ -o demo/state_machine_demo demo/state_machine_demo.o src/ethernet/ethernet_frame.o src/ip/ipv4_packet.o src/ip/checksum.o src/tcp/tcp_segment.o src/tcp/tcp_state_machine.o src/stack.o -lpcap

# Build tests
echo "Building tests..."
g++ -std=c++17 -Iinclude -c tests/manual_test.cpp -o tests/manual_test.o
g++ -o tests/manual_test tests/manual_test.o src/ethernet/ethernet_frame.o src/ip/ipv4_packet.o src/ip/checksum.o src/tcp/tcp_segment.o src/tcp/tcp_state_machine.o src/stack.o -lpcap

if [ -f "demo/simple_demo" ]; then
    echo ""
    echo "✅ Build successful!"
    echo ""
    echo "Run: ./demo/simple_demo"
else
    echo "❌ Build failed"
fi
