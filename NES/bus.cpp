#include "bus.h"
#include <cstdint>
#include <cstring>

const uint16_t RAM = 0x0000;
const uint16_t RAM_MIRRORS_END = 0x1FFF;
const uint16_t PPU_REGISTERS = 0x2000;
const uint16_t PPU_REGISTERS_MIRRORS_END = 0x3FFF;

Bus::Bus() {
  cpu.conBus(this);
  memset(cpu_ram, 0, sizeof(cpu_ram));
}

Bus::~Bus() {}

uint8_t Bus::Read(const uint16_t addr, bool bReadOnly) {
  if (addr >= 0x0000 && addr <= 0xFFFF) {
    return cpu_ram[addr];
  }
  return 0x00;
}

void Bus::Write(const uint16_t addr, const uint8_t val) {
    cpu_ram[addr] = val;
}
