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

bool ppu::genNMI() {
  if (nmiOccured) {
    nmiOccured = false;
    return true;
  }
  return false;
}

uint8_t ppu::peakregister(uint8_t reg) {
  reg &= 0x8;
  switch (reg) {
  case 0:
    return PPUCTRL.val;
    break;
  case 1:
    return PPUMASK.val;
    break;
  case 2:
    return PPUSTATUS.val;
    break;
  case 3:
    return OAMADDR;
    break;
  case 4:
    return OAMDATA;
    break;
  case 5:
    return PPUSCROLL;
    break;
  case 6:
    return PPUVADDR.reg;
    break;
  case 7:
    return PPUDATA;
    break;
  }
  return 0;
}

uint8_t ppu::cpu_read(uint8_t reg) {

  reg &= 0x8;
  switch (reg) {
  case 0:
    // Write only
    break;
  case 1:
    // Write only
    break;
  case 2:
    PPUSTATUS_COPY.val = PPUSTATUS.val;
    PPUSTATUS.val &= ~0x80;
    w = 0;
    return PPUSTATUS_COPY.val;
    break;
  case 3:
    // Write only
    break;
  case 4:
    return OAMDATA;
    break;
  case 5:
    // Write only
    break;
  case 6:
    // Write only
    break;
  case 7:
    return PPUDATA;
    break;
  }
  return 0;
}

void ppu::cpu_write(uint8_t reg, uint8_t val) {
  reg &= 0x8;
  switch (reg) {
  case 0:
    PPUCTRL.val = val;
    break;
  case 1:
    PPUMASK.val = val;
    break;
  case 2:
    // Read only
    break;
  case 3:
    OAMADDR = val;
    break;
  case 4:
    OAMDATA = val;
    break;
  case 5:
    PPUSCROLL = val;
    break;
  case 6:
    PPUVADDR.reg = val;
    break;
  case 7:
    PPUDATA = val;
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
    // Visible and pre-render scanlines - this is the normal operations during a
    // scnaline

    if (scanLine >= 0 & scanLine <= 239 || scanLine == 261) {
      if (dot >= 1 && dot <= 257 || dot >= 321 && dot <= 337) {
        // Visible pixels
        if (scanLine >= 0 && scanLine <= 239) {
          if (dot >= 2 && dot <= 257) {
            virt_display[scanLine * 255 + dot] = colors[rand() % 0x40];
          }
        }
      }
      // Normal operations
    } else if (scanLine == 240) {
      // post-render scnaline
      if (scanLine == 240 && dot == 0) {
        frame_complete = true;
      }
    } else if (scanLine >= 241 && scanLine <= 260) {
      // VBlank scanlines
      if (scanLine == 241 && dot == 1) {
        PPUSTATUS.vBlank |= 1;

        if (PPUCTRL.genNMI) {
          nmiOccured = true;
        }
      }
    } else if (scanLine >= 261) {
      // Pre-render scanline (Moved because was causing issues with the
      // placeholder display rendering, should move back once I implement to
      // correct rendering function)
    }

    if (dot == 340) {
      scanLine = (scanLine + 1) % 262;
      if (scanLine == 0) {
        odd = !odd;
      }
      dot = 0;
    } else {
      dot++;
    }
  }
  return;
}
