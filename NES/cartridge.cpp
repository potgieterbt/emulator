#include "cartridge.h"
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>

enum NameTableMirroring {
  Horizontal = 0,
  Vertical = 1,
  FourScreen = 8,
  OneScreenLower,
  OneScreenHigher,
};

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
  if (!romFile.read(reinterpret_cast<char *>(&header[0]), 0x10)) {
    std::cout << "Reading iNES header failed.\n";
    return false;
  }

  if (std::string{&header[0], &header[4]} != "NES\x1A") {
    std::cout << "Not a valid iNES image. Magic number: " << std::hex
              << header[0] << " " << header[1] << " " << header[2] << " "
              << int(header[3]) << "\n"
              << "Valid magic number : N E S 1a"
              << "\n";
    return false;
  }

  std::cout << "Reading header, dictates: \n";

  uint8_t banks = header[4];
  std::cout << "16KB PRG-ROM Banks: " << +banks << "\n";
  if (!banks) {
    std::cout << "ROM has no PRG-ROM banks. Loading ROM failed\n";
  }

  if (header[6] & 0x8) {
    nameTableMirroring = NameTableMirroring::FourScreen;
    std::cout << "Name Table Mirroring: FourScreen\n";
  } else {
    nameTableMirroring = header[6] & 0x1;
    std::cout << "Name Table Mirroring: "
              << (nameTableMirroring == 0 ? "Horizontal" : "Vertical") << "\n";
  }

  mapperNumber = ((header[6]>> 4) & 0xf) | (header[7] & 0xf0);
  std::cout << "Mapper #: " << +mapperNumber << "\n";

  return true;
}
