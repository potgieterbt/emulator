#include "ppu.hpp"
#include <cstdint>
#include <vector>

ppu::ppu(const std::vector<uint8_t> &chr) : m_CHR_ROM(chr){};

void ppu::tick() {
  // Visible and pre-render scanlines - this is the normal operations during a
  // scnaline
  if (scanLine >= 0 & scanLine <= 239 || scanLine == 261) {
    if (dot >= 0 && dot <= 256) {
      // Visible pixels
    } else if (dot >= 257 && dot <= 320) {
      // Tile data of sprite for the next line fetched
    } else if (dot >= 321 && dot <= 336) {
      // First 2 tiles of next scanline fetched and loaded into shift registers
    } else if (dot >= 337 && dot <= 340) {
      // 2 unused Nametable fetches
    }
    // Normal operations
  } else if (scanLine == 240) {
    // post-render scnaline
  } else if (scanLine >= 241 && scanLine <= 260) {
    // VBlank scanlines
  }
  return;
}
