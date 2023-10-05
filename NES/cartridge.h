#include <cstdint>
#include <vector>

enum Mirroring {
    VERTICAL,
    HORIZONTAL,
    FOUR_SCREEN,
};

class Cart {
public:
  void init();
  Cart();
  ~Cart();
  const std::vector<uint8_t> &getROM();
  const std::vector<uint8_t> &getVROM();
  uint8_t mapper;
  Mirroring screen_mirroring;

private:
  std::vector<uint8_t> PRG_ROM;
  std::vector<uint8_t> CHR_ROM;
};
