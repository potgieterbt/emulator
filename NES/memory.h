#include <cstdint>

class RAM {
private:
  uint8_t memory[65536];

public:
  void Write(uint16_t add, uint8_t val);
  void Write_16(uint16_t add, uint16_t val);
  uint8_t Read(uint16_t add);
  uint16_t Read_16(uint16_t add);
};
