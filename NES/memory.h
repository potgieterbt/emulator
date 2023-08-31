


#include <cstdint>

class RAM {
private:
  uint8_t memory[65536];

public:
  void Write();
  uint8_t Read();
};
