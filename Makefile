CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -g -Wall
LDFLAGS = -lpcap

# Source files
SRCS = \
	src/ethernet/ethernet_frame.cpp \
	src/ip/ipv4_packet.cpp \
	src/ip/checksum.cpp \
	src/tcp/tcp_segment.cpp \
	src/tcp/tcp_state_machine.cpp \
	src/stack.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Demo files
DEMO_SRCS = demo/simple_demo.cpp demo/state_machine_demo.cpp
DEMO_OBJS = $(DEMO_SRCS:.cpp=.o)
DEMO_EXES = demo/simple_demo demo/state_machine_demo

# Test files
TEST_SRCS = tests/manual_test.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)
TEST_EXES = tests/manual_test

# Main targets
all: $(OBJS) $(DEMO_EXES) $(TEST_EXES)

# Build object files first
$(OBJS): %.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Demo executables
demo/simple_demo: demo/simple_demo.o $(OBJS)
	@mkdir -p $(@D)
	$(CXX) -o $@ $^ $(LDFLAGS)

demo/state_machine_demo: demo/state_machine_demo.o $(OBJS)
	@mkdir -p $(@D)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Test executable
tests/manual_test: tests/manual_test.o $(OBJS)
	@mkdir -p $(@D)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Pattern rule for demo/test object files
demo/%.o: demo/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests/%.o: tests/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJS) $(DEMO_OBJS) $(TEST_OBJS) $(DEMO_EXES) $(TEST_EXES)

# Run demos
run-demo: demo/simple_demo
	./demo/simple_demo

run-state-demo: demo/state_machine_demo
	./demo/state_machine_demo

run-tests: tests/manual_test
	./tests/manual_test

.PHONY: all clean run-demo run-state-demo run-tests
