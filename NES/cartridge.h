#include <cstdint>
#include <vector>

class Cart {
public:
  Cart();
  ~Cart();
  const std::vector<uint8_t> &getROM();
  const std::vector<uint8_t> &getVROM();

private:
  std::vector<uint8_t> PRG_ROM;
  std::vector<uint8_t> CHR_ROM;
};
