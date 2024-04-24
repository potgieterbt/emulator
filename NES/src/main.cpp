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
  std::vector<uint8_t> test = CPU.loadROM(path);
  std::cout << std::string{&test[0], &test[4]} << "\n";

    return 0;
}
