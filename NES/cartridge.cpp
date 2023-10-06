#include "cartridge.h"
#include <cstdint>
#include <vector>

void Cart::init(){
}

const std::vector<uint8_t>& Cart::getROM(){
  return PRG_ROM;
}

const std::vector<uint8_t>& Cart::getVROM(){
  return CHR_ROM;
}
