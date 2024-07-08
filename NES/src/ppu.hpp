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

public:
  ppu(const std::shared_ptr<ROM>);
  void tick(uint8_t cycles);
  uint8_t cpu_read(uint8_t reg);
  void cpu_write(uint8_t reg, uint8_t val);
  uint8_t ppu_read(uint16_t addr);
  void ppu_write(uint8_t addr, uint8_t val);
  void setMapper(uint8_t mapNum);
  void *getVdisplay();
  std::array<uint32_t, 61440> getVdisplayCopy();
  bool getFrameComplete();
  void setFrameComplete(bool val);
  bool genNMI();

private:
  bool frame_complete;
  bool nmiOccured;
  bool rendering;
  std::shared_ptr<ROM> m_cart;
  std::vector<uint8_t> m_CHR_ROM;
  std::array<uint8_t, 2048> vram;
  std::array<uint8_t, 256> oam_data;

  std::array<uint32_t, 61440> bgPatternTable = {0};
  std::array<uint32_t, 61440> virt_display = {0};

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
      unsigned nametableAddress : 2;
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

  union loopy_register {
    struct {
      uint16_t course_x : 5;
      uint16_t course_y : 5;
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
