#include "cartridge.h"
#include "cpu.h"
#include "mapper.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: ./NES <whole path the ROM>" << "\n";
  }
  std::string file = argv[1];
  std::cout << file << "\n";
  Cart rom;
  rom.loadFromFile(file);
  Mapper mapper = Mapper(rom.getMapper());
  std::cout << &mapper << "\n";
  // CPU chip = CPU(*mapper);
  return 0;
}
