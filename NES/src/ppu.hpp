#pragma once
#include "rom.hpp"
#include <array>
#include <cstdint>
#include <memory>
#include <vector>

class ppu {
private:
  const int ScanlineCycleLength = 341;
  const int ScanlineEndCycle = 340;
  const int VisibleScanlines = 240;
  const int ScanlineVisibleDots = 256;
  const int FrameEndScanline = 261;
  void fetchTiles();
  void emitPixel();
  void evalSprites();
  void incrementY();
  void incrementX();
  void loadShifters();

public:
  ppu(const std::shared_ptr<ROM>);
  void tick(uint8_t cycles);
  uint8_t peakregister(uint8_t reg);
  uint8_t cpu_read(uint8_t reg);
  void cpu_write(uint8_t reg, uint8_t val);
  uint8_t ppu_read(uint16_t addr);
  void ppu_write(uint16_t addr, uint8_t val);
  void setMapper(uint8_t mapNum);
  void *getVdisplay();
  std::array<uint32_t, 61440> getVdisplayCopy();
  bool getFrameComplete();
  void setFrameComplete(bool val);
  bool genNMI();
  int frames_complete = 0;

  struct SpriteRenderEntity {
    uint8_t lo;
    uint8_t hi;
    uint8_t attr;
    uint8_t counter;
    uint8_t id;
    bool flipHorizontally;
    bool flipVertically;
    int shifted = 0;

    void shift() {
      if (shifted == 8) {
        return;
      }

      if (flipHorizontally) {
        lo >>= 1;
        hi >>= 1;
      } else {
        lo <<= 1;
        hi <<= 1;
      }

      shifted++;
    }
  };
  std::vector<SpriteRenderEntity> spriteRenderEntities;
  SpriteRenderEntity out;

private:
  bool w = false;
  uint8_t x;
  uint8_t nametableByte;
  uint8_t attributetableByte;
  uint8_t quadrabnt_num;
  uint8_t patternLow;
  uint8_t patternHigh;
  uint16_t bgShiftRegLo;
  uint16_t bgShiftRegHi;
  uint16_t attrShiftReg1;
  uint16_t attrShiftReg2;
  bool inRange = false;
  int spriteHeight = 8;
  int inRangeCycles = 8;

  uint16_t spritePatternLowAddr;
  uint16_t spritePatternHighAddr;

  int pixelIndex = 0;
  bool frame_complete;
  bool nmiOccured;
  bool rendering;
  uint8_t read_buffer;
  uint8_t read_buffer_cpy;
  std::shared_ptr<ROM> m_cart;
  std::vector<uint8_t> m_CHR_ROM;
  std::array<uint8_t, 2048> vram;
  std::array<uint8_t, 256> oam_data;

  uint8_t m_tbl_palette[32];
  std::array<uint32_t, 61440> bgPatternTable = {0};
  std::array<uint32_t, 61440> virt_display = {0};

  struct Sprite {
    uint8_t y;
    uint8_t tileNum;
    uint8_t attr;
    uint8_t x;
    uint8_t id;
  };

  bool isUninit(const Sprite &);
  uint16_t getSpritePatternAddress(const Sprite &, bool);

  int OAMCursor = 0;
  int secondaryOAMCursor = 0;
  Sprite OAM[64];
  Sprite secondaryOAM[8];
  Sprite tmpOAM;

public:
  uint8_t *pOAM = (uint8_t *)OAM;

private:
  uint8_t m_mapper;

  int scanLine = 0;
  int dot = 0;
  bool odd = false;

  enum Mirroring {
    Horizontal = 0,
    Vertical = 1,
    FourScreen = 8,
    OneScreenHigher,
    OneScreenLower,
  };

  uint32_t palette[64] = {
      4283716692, 4278197876, 4278718608, 4281335944, 4282646628, 4284219440,
      4283696128, 4282128384, 4280297984, 4278729216, 4278206464, 4278205440,
      4278202940, 4278190080, 4278190080, 4278190080, 4288190104, 4278734020,
      4281348844, 4284227300, 4287108272, 4288681060, 4288160288, 4286069760,
      4283718144, 4280840704, 4278746112, 4278220328, 4278216312, 4278190080,
      4278190080, 4278190080, 4293717740, 4283210476, 4286086380, 4289749740,
      4293154028, 4293679284, 4293683812, 4292118560, 4288719360, 4285842432,
      4283224096, 4281912428, 4281906380, 4282137660, 4278190080, 4278190080,
      4293717740, 4289252588, 4290559212, 4292129516, 4293701356, 4293701332,
      4293702832, 4293182608, 4291613304, 4290043512, 4289258128, 4288209588,
      4288730852, 4288717472, 4278190080, 4278190080};

  const uint32_t colors[64] = {
      0x666666ff, 0x002a88ff, 0x1412a7ff, 0x3b00a4ff, 0x5c007eff, 0x6e0040ff,
      0x6c0600ff, 0x561d00ff, 0x333500ff, 0x0b4800ff, 0x005200ff, 0x004f08ff,
      0x00404dff, 0x000000ff, 0x000000ff, 0x000000ff, 0xadadadff, 0x155fd9ff,
      0x4240ffff, 0x7527feff, 0xa01accff, 0xb71e7bff, 0xb53120ff, 0x994e00ff,
      0x6b6d00ff, 0x388700ff, 0x0c9300ff, 0x008f32ff, 0x007c8dff, 0x000000ff,
      0x000000ff, 0x000000ff, 0xfffeffff, 0x64b0ffff, 0x9290ffff, 0xc676ffff,
      0xf36affff, 0xfe6eccff, 0xfe8170ff, 0xea9e22ff, 0xbcbe00ff, 0x88d800ff,
      0x5ce430ff, 0x45e082ff, 0x48cddeff, 0x4f4f4fff, 0x000000ff, 0x000000ff,
      0xfffeffff, 0xc0dfffff, 0xd3d2ffff, 0xe8c8ffff, 0xfbc2ffff, 0xfec4eaff,
      0xfeccc5ff, 0xf7d8a5ff, 0xe4e594ff, 0xcfef96ff, 0xbdf4abff, 0xb3f3ccff,
      0xb5ebf2ff, 0xb8b8b8ff, 0x000000ff, 0x000000ff,
  };

  union {
    struct {
      unsigned nametable_x : 1;
      unsigned nametable_y : 1;
      unsigned vramIncrement : 1;
      unsigned spritePatternTable : 1;
      unsigned bgPatternTable : 1;
      unsigned spriteSize : 1;
      unsigned ppuMasterSlaveSelect : 1;
      unsigned genNMI : 1;
    };
    uint8_t val;
  } PPUCTRL;

  union {
    struct {
      unsigned greyscale : 1;
      unsigned showLeftBackground : 1;
      unsigned showLeftSprites : 1;
      unsigned showBackground : 1;
      unsigned showSprites : 1;
      unsigned emphasizeRed : 1;
      unsigned emphasizeGreen : 1;
      unsigned emphasizeBlue : 1;
    };
    uint8_t val;
  } PPUMASK;

  union {
    struct {
      unsigned leastSignificantBits : 5;
      unsigned spriteOverflow : 1;
      unsigned spriteZeroHit : 1;
      unsigned vBlank : 1;
    };
    uint8_t val;
  } PPUSTATUS;

  union {
    struct {
      unsigned leastSignificantBits : 5;
      unsigned spriteOverflow : 1;
      unsigned spriteZeroHit : 1;
      unsigned vBlank : 1;
    };
    uint8_t val;
  } PPUSTATUS_COPY;

  union loopy_register {
    struct {
      uint16_t coarse_x : 5;
      uint16_t coarse_y : 5;
      uint16_t nametable_x : 1;
      uint16_t nametable_y : 1;
      uint16_t fine_y : 3;
      uint16_t unused : 1;
    };
    uint16_t reg;
  };

  loopy_register PPUVADDR;
  loopy_register PPUTADDR;

  uint8_t OAMADDR;
  uint8_t OAMDATA;
  uint8_t PPUSCROLL;
  uint8_t PPUDATA;
  uint8_t OAMDMA;
};
