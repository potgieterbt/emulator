#include "ppu.hpp"
#include "rom.hpp"
#include <bits/fs_fwd.h>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <vector>

ppu::ppu(const std::shared_ptr<ROM> rom) : m_cart(rom) {
  m_CHR_ROM = m_cart->getCHR();
  PPUMASK.showBackground = 1;
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

  reg %= 0x8;
  switch (reg) {
  case 0:
    // Write only
    break;
  case 1:
    // Write only
    break;

  case 2:
    PPUSTATUS_COPY.val = (PPUSTATUS.val & 0xE0) | (read_buffer_cpy & 0x1F);
    PPUSTATUS.vBlank = 0;
    w = 0;
    return PPUSTATUS_COPY.val;
    break;

  case 3:
    // Write only
    break;

  case 4:
    // Need to return the value read from OAMADDR
    return pOAM[OAMADDR];
    break;

  case 5:
    // Write only
    break;

  case 6:
    // Write only
    break;

  case 7:
    // Need to return the value read from PPUADDR
    read_buffer = read_buffer_cpy;
    read_buffer_cpy = ppu_read(PPUVADDR.reg);
    if (PPUVADDR.reg >= 0x3F00) {
      read_buffer = read_buffer_cpy;
    }
    PPUVADDR.reg += (PPUCTRL.vramIncrement ? 32 : 1);
    return read_buffer;
    break;
  }
  return 0;
}

void ppu::cpu_write(uint8_t reg, uint8_t val) {
  std::cin.get();
  reg &= 0x8;
  PPUSTATUS.leastSignificantBits = val & 0b00011111;
  switch (reg) {
  case 0:
    PPUCTRL.val = val;
    PPUTADDR.nametable_x = PPUCTRL.nametable_x;
    PPUTADDR.nametable_y = PPUCTRL.nametable_y;
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
    pOAM[OAMADDR] = val;
    break;
  case 5:
    if (w == 0) {
      x = val & 0x07;
      PPUTADDR.course_x = val >> 3;
      w = 1;
    } else {
      PPUTADDR.fine_y = val & 0x07;
      PPUTADDR.course_y = val >> 3;
      w = 0;
    }
    PPUSCROLL = val;
    break;
  case 6:
    if (w == 0) {
      PPUTADDR.reg = (uint16_t)((val & 0x3F) << 8) || (PPUTADDR.reg & 0xFF);
      w = 1;
    } else {
      PPUTADDR.reg = (PPUTADDR.reg & 0xFF00) | val;
      PPUVADDR = PPUTADDR;
      w = 0;
    }
    break;
  case 7:
    ppu_write(PPUVADDR.reg, val);
    PPUVADDR.reg += (PPUCTRL.vramIncrement ? 32 : 1);
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

// PPU Communicate with PPUBus
void ppu::ppu_write(uint8_t addr, uint8_t val) {
  switch (addr) {
  case 0x0000 ... 0x1FFF:
    break;
  }
}

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
      // printf("%i", scanLine);
      // printf("%i\n", dot);
      if (scanLine == 261) {
        if (dot == 2) {
          PPUSTATUS.vBlank = 0;
          PPUSTATUS.spriteOverflow = 0;
          PPUSTATUS.spriteZeroHit = 0;
        }

        if (dot >= 280 && dot <= 304) {
          // Copy virtical bits
        }
      }

      if (scanLine >= 0 && scanLine <= 239) {
        // Eval sprites
      }

      if (dot == 257) {
        // Copy horizontal bits
      }

      if ((dot >= 1 && dot <= 257) || (dot >= 321 && dot <= 337)) {
        if ((dot >= 2 && dot <= 257) || (dot <= 322 && dot <= 337)) {
          // Reload shift registers and shift
        }

        // Visible pixels
        if (scanLine >= 0 && scanLine <= 239) {
          if (dot >= 2 && dot <= 257) {
            // Draw the pixel from the pattern byte and NameTable byte
            virt_display[scanLine * 255 + dot] = colors[frames_complete % 64];
          }
        }
        // Fetch tiles
        if (PPUMASK.showBackground || PPUMASK.showSprites) {
          int cycle = dot % 8;
          switch (cycle) {
          case 1:
            // Fetch NameTable byte
            nametableByte = ppu_read(0x2000 | (PPUVADDR.reg & 0x0FFF));
            if (nametableByte != 0) {

              printf("\n%b\n", nametableByte);
              printf("\n%i\n", nametableByte);
              std::cin.get();
            }
            // std::cin.get();
            break;
          case 3:
            // Fetch Attribute Table byte
            attributetableByte = ppu_read(
                0x23C0 | (PPUVADDR.nametable_y << 11) |
                (PPUVADDR.nametable_x << 10) | ((PPUVADDR.course_y >> 2) << 3) |
                (PPUVADDR.course_x >> 2));
            if (attributetableByte != 0) {
              printf("\n%b\n", attributetableByte);
              printf("\n%i\n", attributetableByte);

              std::cin.get();
            }
            break;
          case 5:
            // Fetch Pattern Table Lower byte
            patternLow =
                ppu_read((PPUCTRL.bgPatternTable << 12) +
                         ((uint16_t)nametableByte << 4) + (PPUVADDR.fine_y));
            break;
          case 7:
            // Fetch Pattern Table Upper byte
            ppu_read((PPUCTRL.bgPatternTable << 12) +
                     ((uint16_t)nametableByte << 4) + (PPUVADDR.fine_y) + 8);
            break;
          case 0:
            if (dot == 256) {
              // Increment Y
            }
            // Increment X
            break;
          default:
            break;
          }
        }
      }
      // Normal operations
    } else if (scanLine == 240) {
      // std::cin.get();
      // post-render scnaline
      if (scanLine == 240 && dot == 0) {
        frame_complete = true;
        frames_complete++;
      }
    } else if (scanLine >= 241 && scanLine <= 260) {
      // VBlank scanlines
      if (scanLine == 241 && dot == 1) {
        PPUSTATUS.vBlank = 1;
        printf("VBlank set");

        if (PPUCTRL.genNMI) {
          nmiOccured = true;
        }
      }
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
