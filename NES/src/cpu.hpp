#include <array>
#include <cstdint>
#include <string>
#include <vector>

class cpu {
public:
  std::array<uint8_t, 0xFFFF> RAM;

  std::vector<uint8_t> loadROM(const std::string path);

  std::vector<uint8_t> ROM;
};
