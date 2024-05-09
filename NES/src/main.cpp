#include "cpu.hpp"
#include <cstdint>
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

  CPU.reset();
  printf("%x\n", CPU.PC);

  CPU.runCycle();

  return 0;
}
