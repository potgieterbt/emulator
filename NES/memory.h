#include <cstdint>

// This is only the RAM for the CPU
class RAM {
private:
  // uint8_t memory[65536];
  uint8_t memory[2048];

public:
  RAM();
  void Write(uint16_t add, uint8_t val);
  void Write_16(uint16_t add, uint16_t val);
  uint8_t Read(uint16_t add);
  uint16_t Read_16(uint16_t add);
};
