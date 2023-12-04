#pragma once

#include "mapper.h"
#include <cstdint>
#include <sys/types.h>
#include <vector>

enum Mirroring {};

class PPU {
public:
  PPU(Mapper *mapper);
  uint8_t Read(uint16_t addr);
  void Write(uint16_t addr, uint8_t val);

private:
  std::vector<uint8_t> chr_rom;
  uint8_t vram[2048];
  uint8_t oam_data[256];

  uint8_t tblName[2][1024];
  uint8_t tblPattern[2][4096];
  uint8_t tblPalette[32];


  //Mirroring
  Mirroring mirroring;

  // Registers:
  // Controller
  union {
    struct {
      uint8_t nametable_x : 1;
      uint8_t nametable_y : 1;
      uint8_t increment_mode : 1;
      uint8_t pattern_sprite : 1;
      uint8_t pattern_background : 1;
      uint8_t sprite_size : 1;
      uint8_t slave_mode : 1;
      uint8_t enable_nmi : 1;
    };
    uint8_t val;
  } ppuctrl;

  // Mask
  union {
    struct {
      uint8_t greyscale: 1;
      uint8_t showBGLeft : 1;
      uint8_t showSpriteLeft : 1;
      uint8_t showBG : 1;
      uint8_t showSprites : 1;
      uint8_t emphasizeRed : 1;
      uint8_t emphasizeGreen : 1;
      uint8_t emphasizeBlue : 1;
    };
    uint8_t val;
  } ppumask;

  // Status Read-only
  union {
    struct {
      uint8_t unused : 5;
      uint8_t spriteOverflow : 1;
      uint8_t spriteZeroHit : 1;
      uint8_t vBlank : 1;
    };
    uint8_t val;
  } ppustatus;

  // OAM Address
  uint8_t oamaddr = 0;

  // OAM Data
  uint8_t oamdata;

  // Scroll
  uint8_t ppuscroll = 0;

  // Address
  uint8_t ppuaddr = 0;

  // Data
  uint8_t ppudata = 0;

  // OAM DMA
  uint8_t oamdma = 0;

};
