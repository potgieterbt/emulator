#pragma once

#include "mirroring.h"
#include "mapper.h"
#include <cstdint>
#include <sys/types.h>
#include <vector>

class PPU {
public:
  PPU(Mapper *mapper);

  struct Sprite {
    uint8_t Y;
    uint8_t tilenum;
    uint8_t attribute;
    uint8_t X;
  };

  // Methods

  uint8_t Read(uint16_t addr);
  void Write(uint16_t addr, uint8_t val);

  void tick();
  void init();
  void reset();
  bool genNMI();


  // Register methods

  uint8_t registerRead(uint16_t addr);
  void registerWrite(uint16_t addr, uint8_t val);

  // Variables

  bool NMI_occurred = false;

  const uint16_t scanlineCycleLength = 341;
  const uint16_t scanlineEndCycle = 340;
  const uint8_t visibleScanLines = 240;
  const uint16_t scanlineVisibleDots = 256;
  const uint16_t frameEndScanline = 261;

  const int attributeOffset = 0x3C0;

private:
  // Methods

  uint8_t readOAM(uint8_t addr);
  void writeOAM(uint8_t addr, uint8_t value);

  // Variables

  uint16_t scanline = 0;

  enum State {
    preRender,
    Render,
    postRender,
    verticalBlank
  };

  const std::vector<uint8_t> chr_rom;
  uint8_t vram[2048];
  // Sprite oam_data[64];

  uint8_t tblName[2][1024];
  uint8_t tblAttribute[4][64];
  // uint8_t tblPattern[2][4096];
  uint8_t tblPattern[2][16];
  uint8_t tblPalette[32];

  // Internal Registers
  uint16_t v;
  uint16_t t;
  uint8_t x;
  int w;

  //NameTableMirroring
  NameTableMirroring mirroring;

  // Registers:
  // Controller
  // Access: write
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
  // Access: write
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
  // Access: read
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
  // Access: write
  uint8_t oamaddr = 0;

  // OAM Data
  // Access: read, write
  uint8_t oamdata;

  // Scroll
  // Access: write twice
  uint8_t ppuscroll = 0;

  // Address
  // Access: write twice
  uint8_t ppuaddr = 0;

  // Data
  // Access: read, write
  uint8_t ppudata = 0;

  // OAM DMA
  // Access: write
  uint8_t oamdma = 0;

};
