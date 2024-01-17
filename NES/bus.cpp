#include "bus.h"
#include "ppu.h"
#include <cstdint>
#include <cstring>


const uint16_t RAM = 0x0000;
const uint16_t RAM_MIRRORS_END = 0x1FFF;
const uint16_t PPU_REGISTERS = 0x2000;
const uint16_t PPU_REGISTERS_MIRRORS_END = 0x3FFF;

Bus::Bus() { memset(cpu_ram, 0, sizeof(cpu_ram)); }

uint8_t Bus::Read(const uint16_t addr) {
    return cpu_ram[mirror_down_addr];
}

void Bus::Write(const uint16_t addr, const uint8_t val) {
  switch (addr) {
  case RAM ... RAM_MIRRORS_END: {
    uint16_t mirror_down_addr = addr & 0b11111111111;
    cpu_ram[mirror_down_addr] = val;
    return;
  }

  case PPU_REGISTERS ... PPU_REGISTERS_MIRRORS_END: {
    uint16_t mirror_down_addr = addr & 0b0010000000000111;
    return;
  }
  default:
    return;
  }
}
