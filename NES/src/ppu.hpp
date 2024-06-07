#include <array>
#include <cstdint>
#include <vector>

class ppu {
public:
  const uint8_t height = 242;
  const uint16_t width = 283;

  uint8_t PPUCTRL;
  uint8_t PPUMASK;
  uint8_t PPUSTATUS;
  uint8_t OAMADDR;
  uint8_t OAMDATA;
  uint8_t PPUSCROLL;
  uint8_t PPUADDR;
  uint8_t PPUDATA;
  uint8_t OAMDMA;

  std::vector<uint8_t> m_CHR_ROM;
  std::array<uint8_t, 32> pallete_table;
  std::array<uint8_t, 2048> vram;
  std::array<uint8_t, 256> oam_data;

  enum Mirroring {
    Horizontal = 0,
    Vertical = 1,
    FourScreen = 8,
    OneScreenHigher,
    OneScreenLower,
  };

  void tick();
};
