// 64 sprites
// 25 colours of 53
// 256x240 pixel progressive picture generator
// NTSC colour encoder

#include "ppu.h"
#include "mapper.h"
#include <cstdint>
#include <vector>

PPU::PPU(Mapper *mapper) {
  chr_rom =  mapper->getChr();
}

void PPU::tick() {}

void PPU::init() {}

void PPU::reset() {}

void PPU::Write(uint16_t addr, uint8_t val) {}

uint8_t PPU::Read(uint16_t addr) {
  switch (addr) {
    case 0x0000 ... 0x1FFF: return 0;
    case 0x2000 ... 0x3EFF: return 0;
    case 0x3F00 ... 0x3FFF: return 0;
    default: return 0;
  }
}

void setCTRL(uint8_t value) {}
void setMask(uint8_t value) {}
void setOAMAddress(uint8_t value) {}
void setOAMData(uint8_t value) {}
void setScroll(uint8_t value) {}
void setDataAddress(uint8_t value) {}
void setData(uint8_t value) {}

uint8_t getStatus() { return 0; }
uint8_t getOAMData() { return 0; }
uint8_t getData() { return 0; }
