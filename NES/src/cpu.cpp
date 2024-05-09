#include "cpu.hpp"
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

void cpu::reset() {
  PC = (read(0xFFFD) << 8) | read(0xFFFC);
  A = 0;
  S = 0;
  A = 0;
  X = 0;
  Y = 0;
  P = 0;
}

uint8_t cpu::read(uint16_t addr) {
  PC++;

  switch (addr) {
  case 0x0000 ... 0x0FFF:
    std::cout << "RAM accessed\n";
    return RAM[addr % 0x0800];
  case 0x2000 ... 0x3FFF:
    return 0;
  case 0x4000 ... 0x4017:
    return 0;
  case 0x4018 ... 0x401F:
    return 0;
  case 0x4020 ... 0xFFFF: {
    uint16_t address = addr % 0x8000;
    return m_PRG_ROM[address];
  }
  default:
    return 0;
  }
}

void cpu::write(uint16_t addr, uint8_t val) { RAM[(addr % 0x2000)] = val; }

void cpu::runCycle() {
  uint8_t op = read(PC);
  printf("%x\n", op);
  executeOpcode(op);
}

void cpu::executeOpcode(uint8_t op) {
  switch (op) {
  case 0x00:
    return;
  case 0xa9:
    printf("%X\n", A);
    LDA(addressing::immediate);
    printf("%X\n", A);
    break;
  case 0x8d:
    STA(addressing::absolute);
  }
}

// Instructions
void cpu::LDA(addressing mode) {
  switch (mode) {
  case addressing::immediate: {
    uint8_t val = read(PC);
    A = val;
    if (A == 0) {
      P &= 0b00000010;
    }
    P &= (A & 0b10000000);
  }
  default:
    break;
  }
}

void cpu::STA(addressing mode) {
  switch (mode) {
  case addressing::absolute: {
    uint8_t addr_high = read(PC);
    uint8_t addr_low = read(PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    write(addr, A);
    break;
  }
  default:
    break;
  }
}

void cpu::PHA() {}

// These mothods will move to the Cartridge class when I implement it

std::vector<uint8_t> cpu::getCHR() { return m_CHR_ROM; }
std::vector<uint8_t> cpu::getPRG() { return m_PRG_ROM; }

bool cpu::loadROM(const std::string path) {
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
