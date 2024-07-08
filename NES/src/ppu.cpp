#include "ppu.hpp"
#include "rom.hpp"
#include <bits/fs_fwd.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <vector>

ppu::ppu(const std::shared_ptr<ROM> rom) : m_cart(rom) {
  m_CHR_ROM = m_cart->getCHR();
};

uint8_t ppu::cpu_read(uint8_t reg) { return 0; }
void ppu::cpu_write(uint8_t reg, uint8_t val) {
  reg &= 0x8;
  switch (reg) {
  case 0:
    break;
  case 1:
    break;
  case 2:
    break;
  case 3:
    break;
  case 4:
    break;
  case 5:
    break;
  case 6:
    break;
  case 7:
    break;
  default:
    return;
  }
}

uint8_t ppu::ppu_read(uint16_t addr) {
  uint8_t mirroring = m_cart->getMirroring();
  addr &= 0x3FFF;
  switch (addr) {
  case 0x0000 ... 0x1FFF: {
    addr &= 0x1FFF;
    return m_CHR_ROM[addr];
  };
  case 0x2000 ... 0x3EFF: {
    addr &= 0x0FFF;
    // Horizontal
    if (mirroring == 0) {
      if ((addr >= 0x0000 && addr <= 0x03FF) ||
          (addr >= 0x0800 && addr <= 0x0BFF)) {
        return vram[addr & 0x0EFF];
      } else if ((addr >= 0x0400 && addr <= 0x07FF) ||
                 (addr >= 0x0C00 && addr <= 0x0FFF)) {
        return vram[1024 + addr & 0x0EFF];
      }

      // Vertical
    } else if (mirroring == 1) {

      if ((addr >= 0x0000 && addr <= 0x03FF) ||
          (addr >= 0x0400 && addr <= 0x07FF)) {

        return vram[addr & 0x0EFF];
      } else if ((addr >= 0x0800 && addr <= 0x0BFF) ||
                 (addr >= 0x0C00 && addr <= 0x0FFF)) {

        return vram[1024 + addr & 0x0EFF];
      }
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

bool ppu::getFrameComplete() { return frame_complete; }

void ppu::setFrameComplete(bool val) { frame_complete = val; }

std::array<uint32_t, 61440> ppu::getVdisplayCopy() { return virt_display; }

void ppu::tick(uint8_t cycles) {
  // Run cycles to catch up with cpu cycles
  for (int i = 0; i < cycles; ++i) {
    printf("Running cycle %i\n", i);
    // Visible and pre-render scanlines - this is the normal operations during a
    // scnaline
    dot++;

    if (scanLine >= 0 & scanLine <= 239 || scanLine == 261) {
      if (dot >= 1 && dot <= 257 || dot >= 321 && dot <= 337) {
        // Visible pixels
        if (scanLine >= 0 && scanLine <= 239) {
          if (dot >= 2 && dot <= 257) {
            virt_display[scanLine * 255 + dot] = colors[rand() % 0x40];
          }
        }
        dot = 0;
        scanLine++;
      }
      // Normal operations
    } else if (scanLine == 240) {
      // post-render scnaline
      if (dot >= 341) {
        dot = 0;
        scanLine++;
      }
    } else if (scanLine >= 241 && scanLine <= 260) {
      // VBlank scanlines
      if (dot >= 341) {
        dot = 0;
        scanLine++;
        if (scanLine >= 261) {
          scanLine = 0;
          frame_complete = true;
        }
      }
    } else if (scanLine >= 261) {
      // Pre-render scanline (Moved because was causing issues with the
      // placeholder display rendering, should move back once I implement to
      // correct rendering function)
    }
  }
  return;
}
