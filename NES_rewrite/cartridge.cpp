#include "cartridge.h"
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <string>
#include <sys/types.h>
#include <vector>

Cart::Cart() {}

const std::vector<uint8_t> &Cart::getROM() { return PRG_ROM; }

const std::vector<uint8_t> &Cart::getVROM() { return CHR_ROM; }

uint8_t Cart::getMapper() { return 0; }

uint8_t Cart::getNameTableMirroring() { return nameTableMirroring; }

bool Cart::hasExtendedRAM() { return extendedRAM; }

bool Cart::loadFromFile(std::string path) {
  std::ifstream romFile(path, std::ios_base::binary | std::ios_base::in);
  if (!romFile) {
    std::cout << "Could not open ROM file from path: " << path << std::endl;
    return false;
  }

  std::vector<uint8_t> header;
  std::cout << "Reading ROM from path: " << path << std::endl;

  // Header
  header.resize(0x10);
  if (!romFile.read(reinterpret_cast<char *>(&header[0]), 0x10)) {
    std::cout << "Reading iNES header failed." << std::endl;
    return false;
  }
  if (std::string{&header[0], &header[4]} != "NES\x1A") {
    std::cout << "Not a valid iNES image. Magic number: " << std::hex
              << header[0] << " " << header[1] << " " << header[2] << " "
              << int(header[3]) << std::endl
              << "Valid magic number : N E S 1a" << std::endl;
    return false;
  }

  std::cout << "Reading header, it dictates: \n";

  uint8_t banks = header[4];
  std::cout << "16KB PRG-ROM Banks: " << +banks << std::endl;
  if (!banks) {
    std::cout << "ROM has no PRG-ROM banks. Loading ROM failed." << std::endl;
    return false;
  }

  uint8_t vbanks = header[5];
  std::cout << "8KB CHR-ROM Banks: " << +vbanks << std::endl;

  if (header[6] & 0x8) {
    nameTableMirroring = NameTableMirroring::FourScreen;
    std::cout << "Name Table Mirroring: "
              << "FourScreen" << std::endl;
  } else {
    nameTableMirroring = header[6] & 0x1;
    std::cout << "Name Table Mirroring: "
              << (nameTableMirroring == 0 ? "Horizontal" : "Vertical")
              << std::endl;
  }

  mapperNumber = ((header[6] >> 4) & 0xf) | (header[7] & 0xf0);
  std::cout << "Mapper #: " << +mapperNumber << std::endl;

  extendedRAM = header[6] & 0x2;
  std::cout << "Extended (CPU) RAM: " << std::boolalpha << extendedRAM
            << std::endl;

  if (header[6] & 0x4) {
    std::cout << "Trainer is not supported." << std::endl;
    return false;
  }

  if ((header[0xA] & 0x3) == 0x2 || (header[0xA] & 0x1)) {
    std::cout << "PAL ROM not supported." << std::endl;
    return false;
  } else
    std::cout << "ROM is NTSC compatible.\n";

  // PRG-ROM 16KB banks
  PRG_ROM.resize(0x4000 * banks);
  if (!romFile.read(reinterpret_cast<char *>(&PRG_ROM[0]), 0x4000 * banks)) {
    std::cout << "Reading PRG-ROM from image file failed." << std::endl;
    return false;
  }

  // CHR-ROM 8KB banks
  if (vbanks) {
    CHR_ROM.resize(0x2000 * vbanks);
    if (!romFile.read(reinterpret_cast<char *>(&CHR_ROM[0]),
                      0x2000 * vbanks)) {
      std::cout << "Reading CHR-ROM from image file failed." << std::endl;
      return false;
    }
  } else
    std::cout << "Cartridge with CHR-RAM." << std::endl;
  return true;
}
