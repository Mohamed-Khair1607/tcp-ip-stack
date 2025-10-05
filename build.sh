#!/bin/bash

echo "Building TCP/IP Stack Project..."

# Check if required commands are available
if ! command -v g++ &> /dev/null; then
    echo "Error: g++ is not installed. Please install g++"
    exit 1
fi

if ! command -v make &> /dev/null; then
    echo "Error: make is not installed. Please install make"
    exit 1
fi

# Check for libpcap
if ! pkg-config --exists libpcap 2>/dev/null; then
    echo "Warning: libpcap might not be installed. Trying to build anyway..."
fi

# Run make
echo "Building with Make..."
make

# Check if build was successful
if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Build successful!"
    echo ""
    echo "Available executables:"
    echo "  ./demo/simple_demo         - Main demonstration"
    echo "  ./demo/state_machine_demo  - TCP state machine demo" 
    echo "  ./tests/manual_test        - Manual test suite"
    echo ""
    echo "Quick commands:"
    echo "  make run-demo       - Run main demo"
    echo "  make run-state-demo - Run state machine demo"
    echo "  make run-tests      - Run tests"
    echo "  make clean          - Clean build files"
else
    echo ""
    echo "❌ Build failed!"
    echo "Trying manual compilation..."
    manual_build
fi

# Fallback manual build function
manual_build() {
    echo "Attempting manual compilation..."
    
    # Create object files
    for src in src/ethernet/ethernet_frame.cpp src/ip/ipv4_packet.cpp src/ip/checksum.cpp src/tcp/tcp_segment.cpp src/tcp/tcp_state_machine.cpp src/stack.cpp; do
        obj=${src%.cpp}.o
        echo "Compiling $src..."
        g++ -std=c++17 -Iinclude -c $src -o $obj
    done
    
    # Compile and link demo
    echo "Building demo..."
    g++ -std=c++17 -Iinclude -c demo/simple_demo.cpp -o demo/simple_demo.o
    g++ -o demo/simple_demo demo/simple_demo.o src/ethernet/ethernet_frame.o src/ip/ipv4_packet.o src/ip/checksum.o src/tcp/tcp_segment.o src/tcp/tcp_state_machine.o src/stack.o -lpcap
    
    if [ -f "demo/simple_demo" ]; then
        echo "✅ Manual build successful!"
        echo "Run: ./demo/simple_demo"
    else
        echo "❌ Manual build failed"
    fi
}
