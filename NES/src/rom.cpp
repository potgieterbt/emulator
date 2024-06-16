#include "rom.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>

std::vector<uint8_t> ROM::getCHR() { return m_CHR_ROM; }
std::vector<uint8_t> ROM::getPRG() { return m_PRG_ROM; }

bool ROM::loadROM(const std::string path) {
  std::ifstream romFile(path, std::ios_base::binary | std::ios_base::in);
  if (!romFile) {
    std::cout << "Could not open file at given path\n";
    return false;
  }

  std::vector<uint8_t> header;
  header.resize(0x10);

  if (!romFile.read(reinterpret_cast<char *>(&header[0]), 0x10)) {
    std::cout << "Failed to read iNES header\n";
    return false;
  }

  if (std::string{&header[0], &header[4]} != "NES\x1A") {
    std::cout << "Not a valid iNES image. Magic number: " << std::hex
              << header[0] << " " << header[1] << " " << header[2] << " "
              << int(header[3]) << "\n"
              << "Valid magic number : N E S 1a\n";
    return false;
  }
  std::cout << "Reading header, it dictates: \n";

  uint8_t banks = header[4];
  std::cout << "16KB PRG-ROM Banks: " << +banks << "\n";
  if (!banks) {
    std::cout << "ROM has no PRG-ROM banks. Loading ROM failed.\n";
    return false;
  }

  uint8_t vbanks = header[5];
  std::cout << "8KB CHR-ROM Banks: " << +vbanks << "\n";

  if (header[6] & 0x8) {
    m_nameTableMirroring = 8;
    std::cout << "Name Table Mirroring: "
              << "FourScreen"
              << "\n";
  } else {
    m_nameTableMirroring = header[6] & 0x1;
    std::cout << "Name Table Mirroring: "
              << (m_nameTableMirroring == 0 ? "Horizontal" : "Vertical")
              << "\n";
  }

  m_mapperNumber = ((header[6] >> 4) & 0xf) | (header[7] & 0xf0);
  std::cout << "Mapper #: " << +m_mapperNumber << "\n";

  m_extendedRAM = header[6] & 0x2;
  std::cout << "Extended (CPU) RAM: " << std::boolalpha << m_extendedRAM
            << "\n";

  if (header[6] & 0x4) {
    std::cout << "Trainer is not supported."
              << "\n";
    return false;
  }

  if ((header[0xA] & 0x3) == 0x2 || (header[0xA] & 0x1)) {
    std::cout << "PAL ROM not supported."
              << "\n";
    return false;
  } else
    std::cout << "ROM is NTSC compatible.\n";

  // PRG-ROM 16KB banks
  m_PRG_ROM.resize(0x4000 * banks);
  if (!romFile.read(reinterpret_cast<char *>(&m_PRG_ROM[0]), 0x4000 * banks)) {
    std::cout << "Reading PRG-ROM from image file failed."
              << "\n";
    return false;
  }

  // CHR-ROM 8KB banks
  if (vbanks) {
    m_CHR_ROM.resize(0x2000 * vbanks);
    if (!romFile.read(reinterpret_cast<char *>(&m_CHR_ROM[0]),
                      0x2000 * vbanks)) {
      std::cout << "Reading CHR-ROM from image file failed."
                << "\n";
      return false;
    }
  } else
    std::cout << "Cartridge with CHR-RAM."
              << "\n";
  return true;
}
