cmake_minimum_required(VERSION 3.15)
project(TCPIPStack VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED libpcap)

# Include directories
include_directories(include)

# Main library
add_library(tcp_stack
    src/ethernet/ethernet_frame.cpp
    src/ip/ipv4_packet.cpp
    src/ip/checksum.cpp
    src/tcp/tcp_segment.cpp
    src/tcp/tcp_state_machine.cpp
    src/stack.cpp
)

target_link_libraries(tcp_stack ${PCAP_LIBRARIES})

# Demo executable
add_executable(demo demo/simple_demo.cpp)
target_link_libraries(demo tcp_stack)

# State machine demo
add_executable(state_machine_demo demo/state_machine_demo.cpp)
target_link_libraries(state_machine_demo tcp_stack)

# Manual test executable
add_executable(manual_test tests/manual_test.cpp)
target_link_libraries(manual_test tcp_stack)
