#include "ppu.hpp"
#include "rom.hpp"
#include <bits/fs_fwd.h>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
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
    addr &= 0x001F;
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
        vram[addr & 0x0EFF] = val;
        break;
      } else if ((addr >= 0x0400 && addr <= 0x07FF) ||
                 (addr >= 0x0C00 && addr <= 0x0FFF)) {
        vram[1024 + addr & 0x0EFF] = val;
        break;
      }

      // Vertical
    } else if (mirroring == 1) {

      if ((addr >= 0x0000 && addr <= 0x03FF) ||
          (addr >= 0x0400 && addr <= 0x07FF)) {
        vram[addr & 0x0EFF] = val;
        break;

      } else if ((addr >= 0x0800 && addr <= 0x0BFF) ||
                 (addr >= 0x0C00 && addr <= 0x0FFF)) {

        vram[1024 + addr & 0x0EFF] = val;
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

void ppu::incrementY() {
  if ((PPUVADDR.reg & 0x7000) != 0x7000) {
    PPUVADDR.reg += 0x1000;
  } else {
    PPUVADDR.reg &= 0x7000;
    int y = PPUVADDR.coarse_y >> 5;
    if (y == 29) {
      y = 0;
      PPUVADDR.reg ^= 0x0800;
    } else if (y == 31) {
      y = 0;
    } else {
      y++;
    }
    PPUVADDR.reg = (PPUVADDR.reg & ~0x03E0) | (y << 5);
  }
}

void ppu::incrementX() {
  if (PPUVADDR.coarse_x == 31) {
    PPUVADDR.coarse_x = 0;
    PPUVADDR.nametable_x = ~PPUVADDR.nametable_x;
  } else {
    PPUVADDR.coarse_x++;
  }
}

void ppu::loadShifters() {
  if (!(PPUMASK.showBackground || PPUMASK.showSprites)) {
    return;
  }
  bgShiftRegLo <<= 1;
  bgShiftRegHi <<= 1;
  attrShiftReg1 <<= 1;
  attrShiftReg2 <<= 1;

  if (dot % 8 == 1) {
    uint8_t attr_bits1 = (attributetableByte >> quadrabnt_num) & 1;
    uint8_t attr_bits2 = (attributetableByte >> quadrabnt_num) & 2;
    attr_bits1 |= attr_bits1 ? 255 : 0;
    attr_bits2 |= attr_bits2 ? 255 : 0;
    bgShiftRegLo |= patternLow;
    bgShiftRegHi |= patternHigh;
  }
}

// I don't even know if I need the mapper in the ppu but I can remove in not
// needed
void ppu::setMapper(uint8_t mapNum) { m_mapper = mapNum; }

bool ppu::getFrameComplete() { return frame_complete; }

void ppu::setFrameComplete(bool val) { frame_complete = val; }

std::array<uint32_t, 61440> ppu::getVdisplayCopy() { return virt_display; }

bool ppu::isUninit(const Sprite &sprite) {
  return ((sprite.attr == 0xFF) && (sprite.tileNum == 0xFF) &&
          (sprite.x == 0xFF) && (sprite.y == 0xFF)) ||
         ((sprite.x == 0) && (sprite.y == 0) && (sprite.attr == 0) &&
          (sprite.tileNum == 0));
}

uint16_t ppu::getSpritePatternAddress(const Sprite &sprite,
                                      bool flipVertically) {
  uint16_t addr = 0;
  int fineOffset = scanLine - sprite.y;

  if (flipVertically) {
    fineOffset = spriteHeight - 1 - fineOffset;
  }

  if (spriteHeight == 16 && fineOffset >= 8) {
    fineOffset += 8;
  }

  if (spriteHeight == 8) {
    addr = ((uint16_t)PPUCTRL.spritePatternTable << 12) |
           ((uint16_t)sprite.tileNum << 4) | fineOffset;
  } else {
    addr = (((uint16_t)sprite.tileNum & 1) << 12) |
           ((uint16_t)((sprite.tileNum & ~1) << 4)) | fineOffset;
  }

  return addr;
}

void ppu::evalSprites() {
  if (dot >= 1 && dot <= 64) {
    if (dot == 1) {
      secondaryOAMCursor = 0;
    }

    secondaryOAM[secondaryOAMCursor].attr = 0xFF;
    secondaryOAM[secondaryOAMCursor].tileNum = 0xFF;
    secondaryOAM[secondaryOAMCursor].x = 0xFF;
    secondaryOAM[secondaryOAMCursor].y = 0xFF;

    if (dot % 8 == 0) {
      secondaryOAMCursor++;
    }
  }

  if (dot > 65 && dot <= 256) {
    secondaryOAMCursor = 0;
    OAMCursor = 0;

    if (secondaryOAMCursor == 8) {
      return;
    }
    if (OAMCursor == 64) {
      return;
    }
    if ((dot % 2) == 1) {
      tmpOAM = OAM[OAMCursor];

      if (!isUninit(tmpOAM) &&
          ((scanLine >= tmpOAM.y) && (scanLine < (tmpOAM.y + spriteHeight)))) {
        inRangeCycles--;
        inRange = true;
      }
    } else {
      if (inRange) {
        inRangeCycles--;

        if (inRangeCycles == 0) {
          OAMCursor++;
          secondaryOAMCursor++;
          inRangeCycles = 8;
          inRange = true;
        } else {
          tmpOAM.id = OAMCursor;
          secondaryOAM[secondaryOAMCursor] = tmpOAM;
        }
      } else {
        OAMCursor++;
      }
    }
  }
  if (dot >= 257 && dot <= 320) {
    if (dot == 257) {
      secondaryOAMCursor = 0;
      spriteRenderEntities.clear();
    }
    Sprite sprite = secondaryOAM[secondaryOAMCursor];

    int cycle = (dot - 1) % 8;
    switch (cycle) {
    case 0 ... 1:
      if (!isUninit(sprite)) {
        out = SpriteRenderEntity();
      }
      break;

    case 2:
      if (!isUninit(sprite)) {
        out.attr = sprite.attr;
        out.flipHorizontally = sprite.attr & 64;
        out.flipVertically = sprite.attr & 128;
        out.id = sprite.id;
      }
      break;

    case 3:
      if (!isUninit(sprite)) {
        out.counter = sprite.x;
      }
      break;

    case 4:
      if (!isUninit(sprite)) {
        spritePatternLowAddr =
            getSpritePatternAddress(sprite, out.flipVertically);
        out.lo = ppu_read(spritePatternLowAddr);
      }

      break;
    case 5:
      break;

    case 6:
      if (!isUninit(sprite)) {
        spritePatternHighAddr = spritePatternLowAddr + 8;
        out.hi = ppu_read(spritePatternHighAddr);
      }
      break;

    case 7:
      if (!isUninit(sprite)) {
        spriteRenderEntities.push_back(out);
      }
      secondaryOAMCursor++;
      break;

    default:
      break;
    }
  }
}

void ppu::emitPixel() {
  if (!(PPUMASK.showBackground || PPUMASK.showSprites)) {
    pixelIndex++;
    return;
  }

  uint16_t fineSelect = 0x8000 >> x;
  uint16_t pixel1 = (bgShiftRegLo & fineSelect) << x;
  uint16_t pixel2 = (bgShiftRegLo & fineSelect) << x;
  uint16_t pixel3 = (attrShiftReg1 & fineSelect) << x;
  uint16_t pixel4 = (attrShiftReg2 & fineSelect) << x;
  uint8_t bgBit12 = (pixel2 >> 14) | (pixel1 >> 15);

  uint8_t spritePixel1 = 0;
  uint8_t spritePixel2 = 0;
  uint8_t spritePixel3 = 0;
  uint8_t spritePixel4 = 0;
  uint8_t spriteBit12 = 0;
  uint8_t paletteIndex =
      0 | (pixel4 >> 12) | (pixel3 >> 13) | (pixel2 >> 14) | (pixel1 >> 15);
  uint8_t spritePaletteIndex = 0;
  bool showSprite = false;
  bool spriteFound = false;

  for (auto &sprite : spriteRenderEntities) {
    if (sprite.counter == 0 && sprite.shifted != 8) {
      if (spriteFound) {
        sprite.shift();
        continue;
      }
      spritePixel1 =
          sprite.flipHorizontally ? ((sprite.lo & 1) << 7) : sprite.lo & 128;

      spritePixel2 =
          sprite.flipHorizontally ? ((sprite.hi & 1) << 7) : sprite.hi & 128;

      spritePixel3 = sprite.attr & 1;
      spritePixel4 = sprite.attr & 2;
      spriteBit12 = (spritePixel2 >> 6) | (spritePixel1 >> 7);

      if (!PPUSTATUS.spriteZeroHit && spriteBit12 && bgBit12 &&
          sprite.id == 0 && PPUMASK.showSprites && PPUMASK.showBackground &&
          dot < 256) {
        PPUSTATUS.val |= 64;
      }

      if (spriteBit12) {
        showSprite = ((bgBit12 && !(sprite.attr & 32)) || !bgBit12) &&
                     PPUMASK.showSprites;
        spritePaletteIndex =
            0x10 | (spritePixel4 << 2) | (spritePixel3 << 2) | spriteBit12;
        spriteFound = true;
      }
      sprite.shift();
    }
  }
  if (!PPUMASK.showBackground) {
    paletteIndex = 0;
  }

  uint8_t pindex =
      ppu_read(0x3F00 | (showSprite ? spritePaletteIndex : paletteIndex)) % 64;

  uint8_t p = PPUMASK.greyscale ? (pindex & 0x30) : pindex;

  if (dot <= 9 || dot >= 249 || scanLine <= 7 || scanLine >= 232) {
    showSprite = false;
    p = 13;
  }
  virt_display[pixelIndex++] = palette[p];
}

void ppu::fetchTiles() {
  if (!(PPUMASK.showBackground || PPUMASK.showSprites)) {
    return;
  }

  // printf("VADDR: %X\n", PPUVADDR.reg);
  // printf("dot: %i\n", dot);
  switch (dot % 8) {
  case 1: {
    // printf("Read Addr: %X\n ", 0x2000 | (PPUVADDR.reg & 0x0FFF));
    nametableByte = ppu_read(0x2000 | (PPUVADDR.reg & 0x0FFF));
    printf("NT: %X", nametableByte);
    // printf("NT: %i\n", nametableByte);
    break;
  }
  case 3: {
    attributetableByte = ppu_read(0x23C0 | (PPUVADDR.reg & 0x0C00) |
                                  ((PPUVADDR.reg >> 2) & 0x07));
    printf("AT: %X", attributetableByte);
    // printf("AT: %i\n", attributetableByte);
    quadrabnt_num =
        (((PPUVADDR.reg & 2) >> 1) | ((PPUVADDR.reg & 64) >> 5)) * 2;
    break;
  }
  case 5: {
    uint16_t patternAddr = ((uint16_t)PPUCTRL.bgPatternTable << 12) +
                           ((uint16_t)nametableByte << 4) +
                           ((PPUVADDR.reg & 0x7000) >> 12);
    patternLow = ppu_read(patternAddr);
    printf("PL: %X", patternLow);
    // printf("PL: %b\n", patternLow);
    // printf("Pattern Addr: %X\n", patternAddr);
    break;
  }
  case 7: {
    uint16_t patternAddr = ((uint16_t)PPUCTRL.bgPatternTable << 12) +
                           ((uint16_t)nametableByte << 4) +
                           ((PPUVADDR.reg & 0x7000) >> 12) + 8;
    patternHigh = ppu_read(patternAddr);
    printf("PH: %X", patternHigh);
    // printf("PH: %b\n", patternHigh);
    break;
  }
  case 0: {
    if (dot == 256) {
      incrementY();
    }
    incrementX();
    break;
  }
  }
  return;
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
      }

      if (scanLine == 0 && dot == 0 && odd) {
        dot = 1;
      }

      if (scanLine >= 0 && scanLine <= 239) {
        evalSprites();
        // Evaluate Sprites
      }

      if (dot >= 280 && dot <= 304) {
        // Copy Vertical bits
        if (PPUMASK.showSprites || PPUMASK.showBackground) {
          PPUVADDR.reg = (PPUVADDR.reg & ~0x7BE0) | (PPUTADDR.reg & 0x7BE0);
        }
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
          loadShifters();
        }

        if (scanLine >= 0 && scanLine <= 239) {
          if (dot >= 2 && dot <= 257) {
            if (scanLine > 0) {
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
        if (PPUCTRL.genNMI) {
          nmiOccured = true;
        }
      }

      if (scanLine >= 261) {
        scanLine = -1;
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
