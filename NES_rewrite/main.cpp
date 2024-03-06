#include "cartridge.h"
#include "cpu.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: ./NES <whole path the ROM>"
              << "\n";
  }
  std::string file = argv[1];
  std::cout << file << "\n";
  Cart rom;
  rom.loadFromFile(file);
  std::vector<uint8_t> prg = rom.getROM();
  CPU chip = CPU(prg);
  chip.emulateCycle();
  for (int i = 0; i < 5; ++i) {
    chip.emulateCycle();
  }
  return 0;
}
