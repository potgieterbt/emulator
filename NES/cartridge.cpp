#include "cartridge.h"
#include "mapper.h"
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

Mapper *Cart::getMapper() { return &mapper; }

uint8_t Cart::getNameTableMirroring() { return nameTableMirroring; }

bool Cart::hasExtendedRAM() { return extendedRAM; }

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

  uint8_t vbanks = header[5];
  std::cout << "8KB CHR-ROM Banks: " << +vbanks << "\n";

  if (header[6] & 0x8) {
    nameTableMirroring = NameTableMirroring::FourScreen;
    std::cout << "Name Table Mirroring: FourScreen\n";
  } else {
    nameTableMirroring = header[6] & 0x1;
    std::cout << "Name Table Mirroring: "
              << (nameTableMirroring == 0 ? "Horizontal" : "Vertical") << "\n";
  }

  uint8_t mapperNumber = ((header[6] >> 4) & 0xf) | (header[7] & 0xf0);
  Mapper mapper = Mapper(mapperNumber, PRG_ROM, CHR_ROM, nameTableMirroring);
  std::cout << "Mapper #: " << +mapperNumber << "\n";

  if (header[6] & 0x4) {
    std::cout << "Trainer is not supported.\n";
    return false;
  }

  if ((header[0xA] & 0x3) == 0x2 || (header[0xA] & 0x1)) {
    std::cout << "PAL ROM not supported.\n";
    return false;
  } else {
    std::cout << "ROM is NTSC compatible.\n";
  }

  PRG_ROM.resize(0x4000 * banks);
  if (!romFile.read(reinterpret_cast<char *>(&PRG_ROM[0]), 0x4000 * banks)) {
    std::cout << "Reading PRG-ROM from image file failed.\n";
    return false;
  }

  if (vbanks) {
    CHR_ROM.resize(0x2000 * vbanks);
    if (!romFile.read(reinterpret_cast<char *>(&CHR_ROM[0]), 0x2000 * vbanks)) {
      std::cout << "Reading CHR-ROM from image file failed.\n";
      return false;
    }
  } else {
    std::cout << "Cartridge with CHR-RAM.\n";
  }
  return true;
}
