// 64 sprites
// 25 colours of 53
// 256x240 pixel progressive picture generator
// NTSC colour encoder

#include "ppu.h"
#include "mapper.h"
#include <cstdint>
#include <vector>

PPU::PPU(Mapper *mapper) : chr_rom(mapper->getChr()) {}

bool PPU::genNMI() {
  if (NMI_occurred) {
    NMI_occurred = false;
    return true;
  }
  return false;
}

void PPU::tick() {
  if ((scanline >= 0 && scanline < visibleScanLines) ||
      scanline == frameEndScanline) {
    return;
  } else if (scanline >= 240 && scanline <= 260) {
    ppuctrl.val |= 0x80;
    if (ppuctrl.val & 0x80) {
      NMI_occurred = true;
    }
  }
}

void PPU::Write(uint16_t addr, uint8_t val) {}

uint8_t PPU::Read(uint16_t addr) {
  switch (addr) {
  case 0x0000 ... 0x1FFF:
    return 0;
  case 0x2000 ... 0x3EFF:
    return 0;
  case 0x3F00 ... 0x3FFF:
    return chr_rom[1];
  default:
    return 0;
  }
}
