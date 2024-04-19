#include "cpu.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << "Usage: ./NES <path/to/rom>";
    return 1;
  }

  const std::string path = argv[1];

  cpu CPU(path);
  return 0;
}
