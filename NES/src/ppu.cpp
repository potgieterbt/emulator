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
};

// I don't even know if I need the mapper in the ppu but I can remove if not
// needed
void ppu::setMapper(uint8_t mapNum) { m_mapper = mapNum; }

bool ppu::getFrameComplete() { return frame_complete; }

void ppu::setFrameComplete(bool val) { frame_complete = val; }

std::array<uint32_t, 61440> ppu::getVdisplayCopy() { return virt_display; }

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
  // std::cin.get();
  reg %= 0x8;
  PPUSTATUS.leastSignificantBits = val & 0b00011111;
  switch (reg) {
  case 0:
    PPUCTRL.val = val;
    PPUTADDR.nametable_x = PPUCTRL.nametable_x;
    PPUTADDR.nametable_y = PPUCTRL.nametable_y;
    break;
  case 1:
    printf("Mask write: %i", val);
    PPUMASK.val = val;
    printf("Mask: %b, SB: %i", val, PPUMASK.showBackground);
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
      PPUTADDR.coarse_x = val >> 3;
      w = 1;
    } else {
      PPUTADDR.fine_y = val & 0x07;
      PPUTADDR.coarse_y = val >> 3;
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
    printf("VRAM write: Addr: %X, val: %i\n", PPUVADDR.reg, val);
    ppu_write(PPUVADDR.reg, val);
    PPUVADDR.reg += (PPUCTRL.vramIncrement ? 32 : 1);
    break;
  default:
    return;
  }
}

uint8_t ppu::ppu_read(uint16_t addr) {
  addr &= 0x3FFF;
  switch (addr) {
  case 0x0000 ... 0x1FFF: {
    return m_CHR_ROM[addr];
  }
  case 0x2000 ... 0x3EFF: {
    uint8_t mirroring = m_cart->getMirroring();
    addr &= 0x0FFF;
    // Horizontal
    if (mirroring == 0) {
      if ((addr >= 0x0000 && addr <= 0x03FF) ||
          (addr >= 0x0800 && addr <= 0x0BFF)) {
        return vram[addr & 0x03FF];
      } else if ((addr >= 0x0400 && addr <= 0x07FF) ||
                 (addr >= 0x0C00 && addr <= 0x0FFF)) {
        return vram[1024 + (addr & 0x03FF)];
      }

      // Vertical
    } else if (mirroring == 1) {

      if ((addr >= 0x0000 && addr <= 0x03FF) ||
          (addr >= 0x0400 && addr <= 0x07FF)) {
        return vram[addr & 0x03FF];

      } else if ((addr >= 0x0800 && addr <= 0x0BFF) ||
                 (addr >= 0x0C00 && addr <= 0x0FFF)) {
        return vram[1024 + (addr & 0x03FF)];
      }
    }
    return 0;
  }
  case 0x3F00 ... 0x3FFF:
    addr &= 0x001F;
    printf("addr: %X\n", addr);
    if (addr == 0x0010 || addr == 0x0014 || addr == 0x0018 || addr == 0x001C) {
      addr &= 0X000F;
    }
    return m_tbl_palette[addr];
  }
  return 0;
}

// PPU Communicate with PPUBus
void ppu::ppu_write(uint16_t addr, uint8_t val) {
  uint8_t mirroring = m_cart->getMirroring();
  addr &= 0x3FFF;
  switch (addr) {
  case 0x0000 ... 0x1FFF: {
    m_CHR_ROM[addr] = val;
    break;
  };
  case 0x2000 ... 0x3EFF: {
    addr &= 0x0FFF;
    printf("VRAM write");
    // Horizontal
    if (mirroring == 0) {
      if ((addr >= 0x0000 && addr <= 0x03FF) ||
          (addr >= 0x0800 && addr <= 0x0BFF)) {
        vram[addr & 0x03FF] = val;
        break;
      } else if ((addr >= 0x0400 && addr <= 0x07FF) ||
                 (addr >= 0x0C00 && addr <= 0x0FFF)) {
        vram[1024 + addr & 0x03FF] = val;
        break;
      }

      // Vertical
    } else if (mirroring == 1) {

      if ((addr >= 0x0000 && addr <= 0x03FF) ||
          (addr >= 0x0400 && addr <= 0x07FF)) {
        vram[addr & 0x03FF] = val;
        break;

      } else if ((addr >= 0x0800 && addr <= 0x0BFF) ||
                 (addr >= 0x0C00 && addr <= 0x0FFF)) {

        vram[1024 + addr & 0x03FF] = val;
        break;
      }
    }
    return;
  }
  case 0x3F00 ... 0x3FFF:
    addr &= 0x001F;
    if (addr == 0x0010 || addr == 0x0014 || addr == 0x0018 || addr == 0x001C) {
      addr &= 0X000F;
    }
    m_tbl_palette[addr] = val;
    break;
  default:
    return;
  }
  return;
}

bool ppu::isUninit(const Sprite &sprite) {
  return ((sprite.attr == 0xFF) && (sprite.tileNum == 0xFF) &&
          (sprite.x == 0xFF) && (sprite.y == 0xFF)) ||
         ((sprite.x == 0) && (sprite.y == 0) && (sprite.attr == 0) &&
          (sprite.tileNum == 0));
}

void ppu::reloadShiftersAndShift() {
  if (!(PPUMASK.showSprites || PPUMASK.showBackground)) {
    return;
  }

  bgShiftRegHi <<= 1;
  bgShiftRegLo <<= 1;
  attrShiftReg1 <<= 1;
  attrShiftReg2 <<= 1;

  if ((dot & 8) == 1) {
    uint8_t attrbits1 = (attributetableByte >> quadrant_num) & 1;
    uint8_t attrbits2 = (attributetableByte >> quadrant_num) & 2;
    attrShiftReg1 |= attrbits1 ? 255 : 0;
    attrShiftReg2 |= attrbits2 ? 255 : 0;
    bgShiftRegLo = patternLow;
    bgShiftRegHi = patternHigh;
  }
}

void ppu::incrementX() {
  if (!(PPUMASK.showSprites || PPUMASK.showBackground)) {
    return;
  }
  if (PPUVADDR.coarse_x == 31) {
    PPUVADDR.coarse_x = 0;
    PPUVADDR.nametable_x = ~PPUVADDR.nametable_x;
  } else {
    PPUVADDR.coarse_x++;
  }
}

void ppu::incrementY() {
  if (PPUVADDR.fine_y != 7) {
    PPUVADDR.fine_y++;

  } else {
    PPUVADDR.fine_y = 0;

    if (PPUVADDR.coarse_y == 29) {
      PPUVADDR.coarse_y = 0;
      PPUVADDR.nametable_y = ~PPUVADDR.nametable_y;

    } else if (PPUVADDR.coarse_y == 31) {
      PPUVADDR.coarse_y = 0;

    } else {
      PPUVADDR.coarse_y++;
    }
  }
}

void ppu::fetchTiles() {
  if (!(PPUMASK.showSprites || PPUMASK.showBackground)) {
    return;
  }

  switch (dot % 8) {

  // NameTable
  case 1:
    nametableByte = ppu_read(0x2000 | (PPUVADDR.reg & 0x0FFF));
    printf("NT: %X\n", nametableByte);
    break;

  // AttributeTable
  case 3:
    attributetableByte = ppu_read(
        0x23C0 | (PPUVADDR.nametable_y << 11) | (PPUVADDR.nametable_x << 10) |
        (PPUVADDR.coarse_y >> 2) << 3 | (PPUVADDR.coarse_x >> 2));
    printf("AT: %X\n", attributetableByte);
    quadrant_num =
        (((PPUVADDR.reg & 0x2) >> 1) | ((PPUVADDR.reg & 0x64) >> 5)) * 2;
    break;

    // For both BG bytes:
    // PPUVADDR.reg & 0x7000 is fetching the fine_y scroll
    // PPUVADDR.reg << 4 only allows coarse x and y into the address
  // BG lsbits
  case 5:
    patternLow = ppu_read((PPUCTRL.bgPatternTable << 12) |
                          ((uint16_t)nametableByte << 4) | (PPUVADDR.fine_y));
    break;

  // BG msbits
  case 7:
    patternHigh =
        ppu_read((PPUCTRL.bgPatternTable << 12) |
                 ((uint16_t)nametableByte << 4) | (PPUVADDR.fine_y + 8));
    break;

  case 0:
    if (dot == 256) {
      incrementY();
      printf("%X\n", PPUVADDR.reg);
    }
    incrementX();
    printf("%X\n", PPUVADDR.reg);
    break;

  default:
    break;
  }
}

void ppu::emitPixel() {
  if (!(PPUMASK.showSprites || PPUMASK.showBackground)) {
    pixelIndex++;
    return;
  }
  uint16_t fine_sel = 0x8000 >> x;
  uint16_t pixel1 = (bgShiftRegLo & fine_sel) << x;
  uint16_t pixel2 = (bgShiftRegHi & fine_sel) << x;
  uint16_t pixel3 = (attrShiftReg1 & fine_sel) << x;
  uint16_t pixel4 = (attrShiftReg1 & fine_sel) << x;
  uint8_t bg_bit_12 = (pixel2 >> 14) | (pixel1 >> 15);

  uint8_t paletteIndex =
      0 | (pixel4 >> 12) | (pixel3 >> 13) | (pixel2 >> 14) | (pixel1 >> 15);

  if (!PPUMASK.showBackground) {
    paletteIndex = 0;
  }

  uint8_t pindex = ppu_read(0x3F00 | (paletteIndex)) % 64;

  uint8_t p = PPUMASK.greyscale ? (pindex & 0x30) : pindex;

  if (dot <= 9 || dot >= 249 || scanLine <= 7 || scanLine >= 232) {
    p = 13;
  }

  virt_display[pixelIndex++] = colors[p];
}

void ppu::tick(uint8_t cycles) {
  for (int cycle = 0; cycle < cycles; ++cycle) {
    switch (scanLine) {

    case -1 ... 239: {
      if (scanLine == -1) {
        if (dot == 1) {
          pixelIndex = 0;
          PPUSTATUS.vBlank = 0;
          PPUSTATUS.spriteOverflow = 0;
          PPUSTATUS.spriteZeroHit = 0;
        }

        if (dot >= 280 && dot <= 304) {
          // Copy Vertical bits
          if (PPUMASK.showSprites || PPUMASK.showBackground) {
            PPUVADDR.reg = (PPUVADDR.reg & ~0x7BE0) | (PPUTADDR.reg & 0x7BE0);
          }
        }
      }

      if (scanLine == 0 && dot == 0 && odd) {
        dot = 1;
      }

      if (scanLine >= 0 && scanLine <= 239) {
        if (PPUMASK.showSprites || PPUMASK.showBackground) {
          // printf("TODO: Eval Sprites\n");
        }
        // TODO
        // Evaluate Sprites
        // evalSprites();
      }

      if (dot == 257) {
        // Copy Horizontal Bits
        if (PPUMASK.showSprites || PPUMASK.showBackground) {
          PPUVADDR.reg = (PPUVADDR.reg & ~0x41F) | (PPUTADDR.reg & 0x41F);
        }
      }

      if ((dot >= 1 && dot <= 257) || (dot >= 321 && dot <= 337)) {
        if ((dot >= 1 && dot <= 257) || (dot >= 322 && dot <= 337)) {
          // Reload shift registers and shift
          reloadShiftersAndShift();
        }

        if (scanLine >= 0 && scanLine <= 239) {
          if (dot >= 2 && dot <= 257) {
            if (scanLine > 0) {
              if (PPUMASK.showSprites || PPUMASK.showBackground) {
                // printf("TODO: Decrement Sprite Counter\n");
              }
              // TODO
              // Decrement Sprite Counters
              // decrementSpriteCounter();
            }
            // Emit Pixel
            emitPixel();
          }
        }
        // Fetch NameTable, AttributeTable, Pattern Low & High
        fetchTiles();
      }
      break;
    }

    case 240: {
      // Idle
      break;
    }

    case 241 ... 260: {
      // PPU does not access memory here so freely accessed by program

      if (scanLine == 241 && dot == 1) {
        PPUSTATUS.vBlank = 1;
        frame_complete = true;
        printf("frame\n");
        if (PPUCTRL.genNMI) {
          nmiOccured = true;
        }
      }

      break;
    }

    default:
      printf("Reached unreachable code, scanLine: %i", scanLine);
      break;
    }

    if (dot == 340) {
      dot = 0;
      ++scanLine;
      if (scanLine == 261) {
        scanLine = -1;
      } else if (scanLine == 0) {
        odd = !odd;
      }
    } else {
      ++dot;
    }
  }
  return;
}
