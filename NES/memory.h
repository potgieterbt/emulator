#include <cstdint>

class RAM {
private:
  uint8_t memory[65536];

public:
  void Write(uint16_t add, uint8_t val);
  uint8_t Read(uint16_t add);
};
