#include "ppu.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

ppu::ppu(const std::vector<uint8_t> &chr) : m_CHR_ROM(chr) {

  for (int i = 0; i < 61440; ++i) {
    virt_display[i] = colors[rand() % 64];
  }
};
uint8_t ppu::cpu_read(uint8_t reg) { return 0; }
void ppu::cpu_write(uint8_t reg, uint8_t val) {}

uint8_t ppu::ppu_read(uint16_t addr) {
  switch (addr) {
  case 0x0000 ... 0x1FFF: {
    uint8_t val = m_CHR_ROM[addr];
    return 0;
  }
  case 0x2000 ... 0x3EFF: {
    uint16_t index = addr % 0xEFF;

    uint16_t normAddr = addr;
    if (normAddr > 0x3000) {
      normAddr -= 0x1000;
    }
    return 0;
  }
  case 0x3F00 ... 0x3FFF:
    return 0;
  default:
    return 0;
  }
  return 0;
}

void ppu::ppu_write(uint8_t reg, uint8_t val) {}

// I don't even know if I need the mapper in the ppu but I can remove in not
// needed
void ppu::setMapper(uint8_t mapNum) { m_mapper = mapNum; }

void *ppu::getVdisplay() { return &virt_display; }

std::array<uint32_t, 61440> ppu::getVdisplayCopy() { return virt_display; }

void ppu::tick(uint8_t cycles) {
  // Run cycles to catch up with cpu cycles
  for (int i = 0; i < cycles; ++i) {
    printf("Running cycle %i\n", i);
    // Visible and pre-render scanlines - this is the normal operations during a
    // scnaline

    // Random color value
    virt_display[scanLine * dot] = colors[rand() % 0x40];

    if (scanLine >= 0 & scanLine <= 239 || scanLine == 261) {
      if (dot >= 0 && dot <= 256) {
        // Visible pixels

      } else if (dot >= 257 && dot <= 320) {
        // Tile data of sprite for the next line fetched
      } else if (dot >= 321 && dot <= 336) {
        // First 2 tiles of next scanline fetched and loaded into shift
        // registers
      } else if (dot >= 337 && dot <= 340) {
        // 2 unused Nametable fetches
      } else if (dot >= 341) {
        dot = 0;
        scanLine++;
        if (scanLine >= 261) {
          scanLine = -1;
          frame_complete = true;
        }
      }
      // Normal operations
    } else if (scanLine == 240) {
      // post-render scnaline
    } else if (scanLine >= 241 && scanLine <= 260) {
      // VBlank scanlines
    }
  }
  return;
}
