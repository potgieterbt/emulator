#include <cstdio>
#include <fstream>

int main (int argc, char *argv[]) {
  std::ifstream file("roms/PONG", std::iso::binary);
  std::istreambuf_iterator<char>(file);
  return 0;
}
