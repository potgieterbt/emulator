#include "cpu.hpp"
#include <cstdint>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << "Usage: ./NES <path/to/rom>";
    return 1;
  }

  const std::string path = argv[1];

  cpu CPU;
  bool res = CPU.loadROM(path);

  std::vector<uint8_t> test = CPU.getPRG();
  std::cout << test[0];
  std::cout << "test";
  for (uint8_t ins : test) {
    if (test[ins] != 0) {
      std::cout << "0x" << std::setw(8) << std::setfill('0') << std::hex << ins;
      std::cout << "\n";
    }
  }
  std::cout << std::dec << "\n";

  return 0;
}
