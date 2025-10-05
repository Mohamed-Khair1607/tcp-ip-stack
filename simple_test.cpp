#include <iostream>
#include "ip/checksum.h"

int main() {
    std::cout << "Simple compilation test..." << std::endl;
    
    std::vector<uint8_t> data = {0x00, 0x01, 0xf2, 0x03};
    uint16_t result = calculate_checksum(data);
    
    std::cout << "Checksum: 0x" << std::hex << result << std::dec << std::endl;
    std::cout << "✓ Basic compilation works!" << std::endl;
    
    return 0;
}
