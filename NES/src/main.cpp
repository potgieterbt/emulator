#include "cpu.hpp"
#include "ppu.hpp"
#include "rom.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << "Usage: ./NES <path/to/rom>";
    return 1;
  }

  const std::string path = argv[1];

  ROM cart;
  bool res = cart.loadROM(path);
  if (!res) {
    printf("Could not load file, Aborting");
    return 1;
  }

  const std::vector<uint8_t> &chr_rom = cart.getCHR();
  ppu ppu(chr_rom);
  ppu.setMapper(cart.getMapperNumber());

  const std::vector<uint8_t> &prg_rom = cart.getPRG();
  cpu CPU(prg_rom, ppu);

  std::vector<uint8_t> test = cart.getPRG();

  CPU.reset();

  while (true) {
    CPU.runCycle();
    // std::cin.get();
  }

  return 0;
}
