// TODO:
// - implement a step variable that can be set to true at anytime to to start
// stepping if needed in a situation for debugging
// - Implement a save state to be able to save the state at a certain time and
// load that same state to better implement debug Instruction
#include "cpu.hpp"
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <string>
#include <sys/types.h>
#include <vector>

void cpu::reset() {
  PC = ((read(0xFFFD) << 8) | read(0xFFFC));
  A = 0;
  S = 0xFF;
  A = 0;
  X = 0;
  Y = 0;
  P = 0;
}

uint8_t cpu::read(uint16_t addr) {
  switch (addr) {
  case 0x0000 ... 0x0FFF:
    printf("%X\n", addr);
    printf("%X\n", RAM[addr]);
    return RAM[addr % 0x0800];
  case 0x2000 ... 0x3FFF:
    return 0;
  case 0x4000 ... 0x4017:
    return 0;
  case 0x4018 ... 0x401F:
    return 0;
  case 0x4020 ... 0xFFFF: {
    uint16_t address = addr % 0x8000;
    printf("%X\n", address);
    printf("%X\n", m_PRG_ROM[address]);
    return m_PRG_ROM[address];
  }
  default:
    return 0;
  }
}

void cpu::write(uint16_t addr, uint8_t val) {
  RAM[(addr % 0x2000)] = val;
  return;
}

uint8_t cpu::fetchInstruction() { return read(PC); }

void cpu::runCycle() {
  uint8_t op = fetchInstruction();
  printf("Instr: %x\n", op);
  executeOpcode(op);
  ++PC;
}

void cpu::executeOpcode(uint8_t op) {
  switch (op) {
  case 0x20:
    JSR(addressing::absolute);
    break;
  case 0x48:
    PHA(addressing::implied);
    break;
  case 0x4A:
    LSR(addressing::accumulator);
    break;
  case 0x4C:
    JMP(addressing::absolute);
    break;
  case 0x65:
    ADC(addressing::immediate);
    break;
  case 0x68:
    PLA(addressing::implied);
    break;
  case 0x69:
    ADC(addressing::immediate);
    break;
  case 0x6C:
    JMP(addressing::indirect);
    break;
  case 0x6D:
    ADC(addressing::absolute);
    break;
  case 0x70:
    BVS(addressing::relative);
    break;
  case 0x8d:
    STA(addressing::absolute);
    break;
  case 0x9D:
    STA(addressing::absoluteX);
    break;
  case 0xA2:
    LDX(addressing::immediate);
    break;
  case 0xA9:
    LDA(addressing::immediate);
    break;
  case 0xBD:
    LDA(addressing::absoluteX);
    break;
  case 0xC9:
    CMP(addressing::immediate);
    break;
  case 0xE8:
    INX(addressing::implied);
    break;
  case 0xD0:
    BNE(addressing::relative);
    break;
  case 0xF0:
    BEQ(addressing::relative);
    break;
  default:
    printf("%X", op);
    std::cin.get();
  }
}

// Instructions

void cpu::ADC(addressing mode) {
  uint16_t sum;
  uint8_t val;
  switch (mode) {
  case addressing::absolute: {
    uint8_t addr_high = read(++PC);
    uint8_t addr_low = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    val = read(addr);
    sum = A + (val + (P & 1));
    break;
  }
  case addressing::immediate: {
    val = read(++PC);
    sum = A + (val + (P & 1));
    break;
  }
  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    val = read(0x00FF & zaddr);
    sum = A + (val + (P & 1));
    break;
  }
  default:
    break;
  }
  A = sum;
  // checks

  // Carry bit
  if (sum & 0x100) {
    P ^= 1;
  }
  // Will need to confirm that this is correct
  // Overflow bit
  if ((A ^ sum) & (val ^ sum) & 0x80) {
    P ^= 0b01000000;
  }

  // Zero bit
  if (A == 0) {
    P &= 0b00000010;
  }

  // Negative bit
  P &= (A & 0b10000000);
}

void cpu::BNE(addressing mode) {
  switch (mode) {
  case addressing::relative: {
    uint8_t zero = (P & 0b00000010) >> 1;
    // Read before the if statement to ensure that the program counter
    // increments correctly
    int8_t jmp_val = (int8_t)read(++PC);
    if (!zero) {
      // added - 1 due to the way that I increment PC after the Instruction in
      // executed
      PC += jmp_val;
    }
    break;
  }
  default:
    printf("Instruction should not take anything other than a relative "
           "addressing");
    abort();
  }
}

void cpu::BEQ(addressing mode) {
  switch (mode) {
  case addressing::relative: {
    uint8_t zero = (P & 0b00000010) >> 1;
    // Read before the if statement to ensure that the program counter
    // increments correctly
    int8_t jmp_val = (int8_t)read(++PC);
    if (zero) {
      // added - 1 due to the way that I increment PC after the Instruction in
      // executed
      PC += jmp_val;
    }
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           "BEQ", mode);
    abort();
  }
}

void cpu::BVS(addressing mode) {
  switch (mode) {
  case addressing::relative: {
    uint8_t overflow = (P >> 6) & 1;
    uint8_t jmp_val = read(++PC);
    if (overflow) {
      // added - 1 due to the way that I increment PC after the Instruction in
      // executed
      PC += jmp_val - 1;
    }
    break;
  }
  default:
    printf("Instruction should not take anything other than a relative "
           "addressing");
    abort();
  }
}

void cpu::CMP(addressing mode) {
  switch (mode) {
  case immediate: {
    uint8_t val = read(++PC);
    if (A >= val) {
      P |= 1;
    }
    if (A == val) {
      P |= 0b00000010;
    }
    P |= ((A - val) & 0b10000000);
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           "JMP", mode);
    abort();
  }
}

void cpu::INX(addressing mode) {
  switch (mode) {
  case addressing::implied:
    ++X;
    if (X == 0) {
      P |= 0b00000010;
    }
    P |= (X & 0b01000000);
    break;
  default:
    printf("Instruction should not take anything other than a implied "
           "addressing");
    abort();
  }
}

void cpu::JMP(addressing mode) {
  switch (mode) {
  case absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    PC = addr - 1;
    break;
  }
  case indirect: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    uint8_t jaddr_low = read(addr);
    uint8_t jaddr_high = read(++addr);
    uint16_t jmp_addr = (jaddr_high << 8) | jaddr_low;
    PC = jmp_addr - 1;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           "JMP", mode);
    abort();
  }
}

void cpu::JSR(addressing mode) {
  switch (mode) {
  case absolute: {
    // I don't think this is working properly will have to test
    printf("%X", PC);
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    Stack[S] = (PC >> 8);
    --S;
    Stack[S] = (PC & 0xFF);
    --S;
    PC = addr;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           "JMP", mode);
    abort();
  }
}

void cpu::LDA(addressing mode) {
  switch (mode) {
  case addressing::immediate: {
    uint8_t val = read(++PC);
    A = val;
    if (A == 0) {
      P &= 0b00000010;
    }
    P &= (A & 0b10000000);
    break;
  }
  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    addr += X;
    uint8_t val = read(addr);
    A = val;
    if (A == 0) {
      P &= 0b00000010;
    }
    P &= (A & 0b10000000);
    break;
  }
  default:
    break;
  }
}

void cpu::LDX(addressing mode) {
  switch (mode) {
  case addressing::immediate: {
    uint8_t val = read(++PC);
    X = val;
    if (X == 0) {
      P &= 0b00000010;
    }
    P &= (X & 0b10000000);
  }
  default:
    break;
  }
}

void cpu::LSR(addressing mode) {
  switch (mode) {
  case accumulator:
    P |= (A & 1);
    A >>= 1;
    if (A == 0) {
      P |= 0b00000010;
    }
    P |= (A & 0b10000000);
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           "LSR", mode);
  }
}

void cpu::PLA(addressing mode) {
  switch (mode) {
  case implied:
    A = Stack[S];
    if (A == 0) {
      P |= 0b00000010;
    }
    P |= (A & 0b10000000);

    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           "PLA", mode);
  }
}

void cpu::STA(addressing mode) {
  switch (mode) {
  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    write(addr, A);
    break;
  }
  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = ((addr_high << 8) | addr_low) + X;
    write(addr, A);
    break;
  }
  default:
    break;
  }
}

void cpu::PHA(addressing mode) {
  switch (mode) {
  case addressing::implied:
    Stack[(0x100 & S) % 0x1FF] = A;
    S--;
    break;
  default:
    printf("Instruction should not take anything other than a implied "
           "addressing");
    abort();
  }
}

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
