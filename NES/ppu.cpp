// 64 sprites
// 25 colours of 53
// 256x240 pixel progressive picture generator
// NTSC colour encoder

#include "ppu.h"
#include "mapper.h"
#include <cstdint>

PPU::PPU(Mapper *mapper) {}

void PPU::tick() {}

void PPU::reset() {}

void PPU::Write(uint16_t addr, uint8_t val) {}

uint8_t PPU::Read(uint16_t addr) { return 0; }

void setCTRL(uint8_t value);
void setMask(uint8_t value);
void setOAMAddress(uint8_t value);
void setOAMData(uint8_t value);
void setScroll(uint8_t value);
void setDataAddress(uint8_t value);
void setData(uint8_t value);

uint8_t getStatus();
uint8_t getOAMData();
uint8_t getData();
