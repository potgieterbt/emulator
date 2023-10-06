#include "cartridge.h"
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <vector>

const std::vector<uint8_t> &Cart::getROM() { return PRG_ROM; }

const std::vector<uint8_t> &Cart::getVROM() { return CHR_ROM; }

uint8_t Cart::getMapper() { return mapperNumber; }

uint8_t Cart::getNameTableMirroring() { return nameTableMirroring; }

bool Cart::hasExtendedRAM() { return true; }

bool Cart::loadFromFile(std::string path) {
  std::ifstream romFile(path, std::ios_base::binary | std::ios_base::in);
  if (!romFile) {
    std::cout << "Could not open ROM file from path: " << path << "\n";
    return false;
  }

  std::vector<uint8_t> header;
  std::cout << "Reading ROM from path: " << path << "\n";

  header.resize(0x10);
  if (!romFile.read(reinterpret_cast<char*>(&header[0]), 0x10)) {

  }

  return true;
}
