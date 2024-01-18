#include "cartridge.h"
#include "controller.h"
#include "cpu.h"
#include "ppu.h"
#include <array>
#include <cstdint>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  std::string file = argv[3];
  Cart rom;
  rom.loadFromFile(file);
  Mapper *mapper = rom.getMapper();
  PPU ppu = PPU(mapper);
  controller Cont;
  Chip chip = Chip(*mapper, ppu, Cont);
  return 0;
}
