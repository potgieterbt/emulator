#include "cartridge.h"
#include "cpu.h"
#include <array>
#include <cstdint>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  std::string file = argv[3];
  Cart rom;
  rom.loadFromFile(file);
  Mapper *mapper = rom.getMapper();
  CPU chip = CPU(*mapper);
  return 0;
}
