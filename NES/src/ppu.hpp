#pragma once
#include <array>
#include <cstdint>
#include <vector>

class ppu {
private:
  const int ScanlineCycleLength = 341;
  const int ScanlineEndCycle = 340;
  const int VisibleScanlines = 240;
  const int ScanlineVisibleDots = 256;
  const int FrameEndScanline = 261;

public:
  ppu(const std::vector<uint8_t> &);
  void tick(uint8_t cycles);
  uint8_t cpu_read(uint8_t reg);
  void cpu_write(uint8_t reg, uint8_t val);
  uint8_t ppu_read(uint8_t addr);
  void ppu_write(uint8_t addr, uint8_t val);
  void setMapper(uint8_t mapNum);

private:
  bool rendering;
  std::vector<uint8_t> m_CHR_ROM;
  std::array<uint8_t, 32> pallete_table;
  std::array<uint8_t, 2048> vram;
  std::array<uint8_t, 256> oam_data;

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

  uint8_t OAMADDR;
  uint8_t OAMDATA;
  uint8_t PPUSCROLL;
  uint8_t PPUADDR;
  uint8_t PPUDATA;
  uint8_t OAMDMA;
};
