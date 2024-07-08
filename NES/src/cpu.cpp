#include "cpu.hpp"
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <string>
#include <sys/types.h>
#include <vector>

cpu::cpu(const std::shared_ptr<ROM> rom, std::shared_ptr<ppu> PPU)
    : m_cart(rom), m_ppu(PPU) {
  m_PRG_ROM = m_cart->getPRG();
}

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
    printf("%X\n", RAM[addr % 0x0800]);
    return RAM[addr % 0x0800];
  case 0x2000 ... 0x3FFF:
    m_ppu->cpu_read(addr & 8);
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
  switch (addr) {
  case 0x0000 ... 0x0FFF:
    RAM[(addr % 0x2000)] = val;
    return;
  case 0x2000 ... 0x3FFF: {
    uint8_t reg = addr % 8;
    m_ppu->cpu_write(reg, val);
    return;
  }
  case 0x4000 ... 0x4017:
    return;
  case 0x4018 ... 0x401F:
    return;
  case 0x4020 ... 0xFFFF: {
    printf("PRGROM is read-only\n");
    return;
  }
  default:
    return;
  }
}

void cpu::setFlag(uint8_t flag, bool val) {
  P &= ~(1 << flag);
  P |= (val << flag);
}

uint8_t cpu::fetchInstruction() { return read(PC); }

uint8_t cpu::pageCross(uint16_t oldPC, uint16_t newPC) {
  if ((oldPC >> 8) != (newPC >> 8)) {
    return 1;
  }
  return 0;
}

void cpu::runCycle() {
  if (m_ppu->genNMI()) {
    NMI();
  }

  uint8_t op = fetchInstruction();
  printf("Instr: %x\n", op);
  executeOpcode(op);
  ++PC;
  m_ppu->tick(cycles * 3);
  // std::cin.get();
}

void cpu::executeOpcode(uint8_t op) {
  switch (op) {
  case 0x06:
    ASL(addressing::zero);
    break;
  case 0x08:
    PHP(addressing::implied);
    break;
  case 0x09:
    ORA(addressing::immediate);
    break;
  case 0x0D:
    ORA(addressing::absolute);
    break;
  case 0x10:
    BPL(addressing::relative);
    break;
  case 0x18:
    CLC(addressing::implied);
    break;
  case 0x20:
    JSR(addressing::absolute);
    break;
  case 0x24:
    BIT(addressing::zero);
    break;
  case 0x28:
    PLP(addressing::implied);
    break;
  case 0x29:
    AND(addressing::immediate);
    break;
  case 0x2A:
    ROL(addressing::accumulator);
    break;
  case 0x2C:
    BIT(addressing::absolute);
    break;
  case 0x30:
    BMI(addressing::relative);
    break;
  case 0x38:
    SEC(addressing::implied);
    break;
  case 0x45:
    EOR(addressing::zero);
    break;
  case 0x48:
    PHA(addressing::implied);
    break;
  case 0x49:
    EOR(addressing::immediate);
    break;
  case 0x4A:
    LSR(addressing::accumulator);
    break;
  case 0x4C:
    JMP(addressing::absolute);
    break;
  case 0x58:
    CLI(addressing::implied);
    break;
  case 0x60:
    RTS(addressing::implied);
    break;
  case 0x65:
    ADC(addressing::immediate);
    break;
  case 0x66:
    ROR(addressing::zero);
    break;
  case 0x68:
    PLA(addressing::implied);
    break;
  case 0x69:
    ADC(addressing::immediate);
    break;
  case 0x6A:
    ROR(addressing::accumulator);
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
  case 0x78:
    SEI(addressing::implied);
    break;
  case 0x84:
    STY(addressing::zero);
    break;
  case 0x85:
    STA(addressing::zero);
    break;
  case 0x86:
    STX(addressing::zero);
    break;
  case 0x88:
    DEY(addressing::implied);
    break;
  case 0x8C:
    STY(addressing::absolute);
    break;
  case 0x8D:
    STA(addressing::absolute);
    break;
  case 0x8E:
    STX(addressing::absolute);
    break;
  case 0x90:
    BCC(addressing::relative);
    break;
  case 0x91:
    STA(addressing::indirectY);
    break;
  case 0x95:
    STA(addressing::zeroX);
    break;
  case 0x98:
    TYA(addressing::implied);
    break;
  case 0x99:
    STA(addressing::absoluteY);
    break;
  case 0x9A:
    TXS(addressing::implied);
    break;
  case 0x9D:
    STA(addressing::absoluteX);
    break;
  case 0xA0:
    LDY(addressing::immediate);
    break;
  case 0xA2:
    LDX(addressing::immediate);
    break;
  case 0xA4:
    LDY(addressing::zero);
    break;
  case 0xA5:
    LDA(addressing::zero);
    break;
  case 0xA6:
    LDX(addressing::zero);
    break;
  case 0xA8:
    TAY(addressing::implied);
    break;
  case 0xA9:
    LDA(addressing::immediate);
    break;
  case 0xAA:
    TAX(addressing::implied);
    break;
  case 0xAC:
    LDY(addressing::absolute);
    break;
  case 0xAD:
    LDA(addressing::absolute);
    break;
  case 0xAE:
    LDX(addressing::absolute);
    break;
  case 0xB0:
    BCS(addressing::relative);
    break;
  case 0xB1:
    LDA(addressing::indirectY);
    break;
  case 0xB9:
    LDA(addressing::absoluteY);
    break;
  case 0xBA:
    TSX(addressing::implied);
    break;
  case 0xBD:
    LDA(addressing::absoluteX);
    break;
  case 0xC0:
    CPY(addressing::immediate);
    break;
  case 0xC4:
    CPY(addressing::zero);
    break;
  case 0xC8:
    INY(addressing::implied);
    break;
  case 0xC9:
    CMP(addressing::immediate);
    break;
  case 0xCA:
    DEX(addressing::implied);
    break;
  case 0xE0:
    CPX(addressing::immediate);
    break;
  case 0xE6:
    INC(addressing::zero);
    break;
  case 0xE8:
    INX(addressing::implied);
    break;
  case 0xE9:
    SBC(addressing::immediate);
    break;
  case 0xEE:
    INC(addressing::absolute);
    break;
  case 0xD0:
    BNE(addressing::relative);
    break;
  case 0xD8:
    CLD(addressing::implied);
    break;
  case 0xF0:
    BEQ(addressing::relative);
    break;
  case 0xF8:
    SED(addressing::implied);
    break;
  default:
    printf("%X", op);
    std::cin.get();
  }
}

void cpu::NMI() { SEI(addressing::implied); }

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
    cycles = 4;
    break;
  }
  case addressing::immediate: {
    val = read(++PC);
    sum = A + (val + (P & 1));
    cycles = 2;
    break;
  }
  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    val = read(0x00FF & zaddr);
    sum = A + (val + (P & 1));
    cycles = 3;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
    break;
  }
  A = sum;
  // checks

  // Carry bit
  setFlag(0, sum & 0x100);
  // Will need to confirm that this is correct
  // Overflow bit
  setFlag(6, (A ^ sum) & (val ^ sum) & 0x80);

  // Zero bit
  setFlag(1, A == 0);

  // Negative bit
  setFlag(7, A & 0b10000000);
}

void cpu::AND(addressing mode) {
  uint8_t val;
  switch (mode) {
  case addressing::immediate: {
    val = read(++PC);
    A = A & val;
    cycles = 2;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
    break;
  }

  // Zero bit
  setFlag(1, A == 0);

  // Negative bit
  setFlag(7, A & 0b10000000);
}

void cpu::ASL(addressing mode) {
  switch (mode) {
  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    uint8_t val = read(0x00FF & zaddr);
    setFlag(0, (val & 0b10000000) >> 7);
    uint8_t res = val << 1;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 5;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::BCC(addressing mode) {
  switch (mode) {
  case addressing::relative: {
    uint8_t carry = (P & 0b00000001);
    // Read before the if statement to ensure that the program counter
    // increments correctly
    int8_t jmp_val = (int8_t)read(++PC);
    cycles = 2;
    if (!carry) {
      // added - 1 due to the way that I increment PC after the Instruction in
      // executed
      cycles += pageCross(PC, (PC + jmp_val));
      PC += jmp_val;
      cycles++;
    }
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::BCS(addressing mode) {
  switch (mode) {
  case addressing::relative: {
    uint8_t carry = (P & 0b00000001);
    // Read before the if statement to ensure that the program counter
    // increments correctly
    int8_t jmp_val = (int8_t)read(++PC);
    cycles = 2;
    if (carry) {
      // added - 1 due to the way that I increment PC after the Instruction in
      // executed
      cycles += pageCross(PC, (PC + jmp_val));
      PC += jmp_val;
      cycles++;
    }
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
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
    cycles = 2;
    if (zero) {
      // added - 1 due to the way that I increment PC after the Instruction in
      // executed
      cycles += pageCross(PC, (PC + jmp_val));
      PC += jmp_val;
      cycles++;
    }
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::BIT(addressing mode) {
  switch (mode) {
  case absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    uint8_t val = read(addr);
    uint8_t res = A & val;
    // Zero bit
    setFlag(1, res == 0);

    // Negative bit
    setFlag(7, (A - val) & 0b10000000);
    cycles = 3;
    break;
  }
  case zero: {
    uint8_t zaddr = read(++PC);
    uint8_t val = read(0x00FF & zaddr);
    uint8_t res = A & val;
    // Zero bit
    setFlag(1, res == 0);

    // Negative bit
    setFlag(7, (A - val) & 0b10000000);
    cycles = 2;
    break;
  }

  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::BMI(addressing mode) {
  switch (mode) {
  case addressing::relative: {
    uint8_t neg = (P & 0b10000000) >> 7;
    // Read before the if statement to ensure that the program counter
    // increments correctly
    int8_t jmp_val = (int8_t)read(++PC);
    cycles = 2;
    if (neg) {
      cycles += pageCross(PC, (PC + jmp_val));
      PC += jmp_val;
      cycles++;
    }
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::BPL(addressing mode) {
  switch (mode) {
  case addressing::relative: {
    uint8_t neg = (P & 0b10000000) >> 7;
    // Read before the if statement to ensure that the program counter
    // increments correctly
    int8_t jmp_val = (int8_t)read(++PC);
    cycles = 2;
    if (!neg) {
      cycles += pageCross(PC, (PC + jmp_val));
      PC += jmp_val;
      cycles++;
    }
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::BNE(addressing mode) {
  switch (mode) {
  case addressing::relative: {
    uint8_t zero = (P & 0b00000010) >> 1;
    // Read before the if statement to ensure that the program counter
    // increments correctly
    int8_t jmp_val = (int8_t)read(++PC);
    cycles = 2;
    if (!zero) {
      cycles += pageCross(PC, (PC + jmp_val));
      PC += jmp_val;
      cycles++;
    }
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::BVS(addressing mode) {
  switch (mode) {
  case addressing::relative: {
    uint8_t overflow = (P >> 6) & 1;
    uint8_t jmp_val = read(++PC);
    cycles = 2;
    if (overflow) {
      // added - 1 due to the way that I increment PC after the Instruction in
      // executed
      cycles += pageCross(PC, (PC + jmp_val));
      PC += jmp_val;
      cycles++;
    }
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::CLC(addressing mode) {
  switch (mode) {
  case implied: {
    setFlag(0, 0);
    cycles = 2;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::CLD(addressing mode) {
  switch (mode) {
  case implied: {
    setFlag(3, 0);
    cycles = 2;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::CLI(addressing mode) {
  switch (mode) {
  case implied: {
    setFlag(2, 0);
    cycles = 2;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::CMP(addressing mode) {
  switch (mode) {
  case immediate: {
    uint8_t val = read(++PC);
    // Carry
    setFlag(0, A >= val);

    // Zero bit
    setFlag(1, A == val);

    // Negative bit
    setFlag(7, (A - val) & 0b10000000);
    cycles = 2;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::CPX(addressing mode) {
  switch (mode) {
  case immediate: {
    uint8_t val = read(++PC);
    // Carry
    setFlag(0, X >= val);

    // Zero bit
    setFlag(1, X == val);

    // Negative bit
    setFlag(7, (X - val) & 0b10000000);
    cycles = 2;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::CPY(addressing mode) {
  switch (mode) {
  case immediate: {
    uint8_t val = read(++PC);
    // Carry
    setFlag(0, Y >= val);

    // Zero bit
    setFlag(1, Y == val);

    // Negative bit
    setFlag(7, (Y - val) & 0b10000000);
    cycles = 2;
    break;
  }
  case zero: {
    uint8_t zaddr = read(++PC);
    uint8_t val = read(0x00FF & zaddr);
    // Carry
    setFlag(0, Y >= val);

    // Zero bit
    setFlag(1, Y == val);

    // Negative bit
    setFlag(7, (Y - val) & 0b10000000);
    cycles = 3;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::DEX(addressing mode) {
  switch (mode) {
  case addressing::implied:
    --X;
    // Zero bit
    setFlag(1, X == 0);

    // Negative bit
    setFlag(7, X & 0b10000000);
    cycles = 2;
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::DEY(addressing mode) {
  switch (mode) {
  case addressing::implied:
    --Y;
    // Zero bit
    setFlag(1, Y == 0);

    // Negative bit
    setFlag(7, Y & 0b10000000);
    cycles = 2;
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::EOR(addressing mode) {
  switch (mode) {
  case addressing::immediate: {
    uint8_t val = read(++PC);
    A ^= val;

    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 2;
    break;
  }
  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    uint8_t val = read(0x00FF & zaddr);
    A ^= val;

    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 3;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::INC(addressing mode) {
  switch (mode) {
  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    uint8_t val = read(0x00FF & zaddr);
    val++;
    write(0x00FF & zaddr, val);
    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val & 0b10000000);
    cycles = 5;
    break;
  }
  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    uint8_t val = read(addr);
    val++;
    write(addr, val);
    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val & 0b10000000);
    cycles = 6;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::INX(addressing mode) {
  switch (mode) {
  case addressing::implied:
    ++X;
    // Zero bit
    setFlag(1, X == 0);

    // Negative bit
    setFlag(7, X & 0b10000000);
    cycles = 2;
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::INY(addressing mode) {
  switch (mode) {
  case addressing::implied:
    ++Y;
    // Zero bit
    setFlag(1, Y == 0);

    // Negative bit
    setFlag(7, Y & 0b10000000);
    cycles = 2;
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
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
    cycles = 3;
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
    cycles = 5;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::JSR(addressing mode) {
  switch (mode) {
  case absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    uint8_t LSB = (PC & 0xFF);
    uint8_t MSB = (PC >> 8);
    Stack[S] = LSB;
    --S;
    Stack[S] = MSB;
    --S;
    PC = addr - 1;
    cycles = 6;
    break;
  }

  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::LDA(addressing mode) {
  switch (mode) {
  case addressing::immediate: {
    uint8_t val = read(++PC);
    A = val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 2;
    break;
  }
  case addressing::indirectY: {
    uint8_t zaddr = read(++PC);
    uint16_t raddr = read(0x00FF & zaddr);
    uint16_t addr = raddr + Y;
    uint8_t val = read(addr);
    A = val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 5;
    cycles += pageCross((raddr >> 8), (addr >> 8));
    break;
  }
  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    uint8_t val = read(addr);
    A = val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 4;
    break;
  }
  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + X;
    uint8_t val = read(addr);
    A = val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 5;
    cycles += pageCross((raddr >> 8), (addr >> 8));
    break;
  }
  case addressing::absoluteY: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + Y;
    uint8_t val = read(addr);
    A = val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 5;
    cycles += pageCross((raddr >> 8), (addr >> 8));
    break;
  }
  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    uint8_t val = read(0x00FF & zaddr);
    A = val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 3;
    break;
  }

  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::LDX(addressing mode) {
  switch (mode) {
  case addressing::immediate: {
    uint8_t val = read(++PC);
    X = val;
    // Zero bit
    setFlag(1, X == 0);

    // Negative bit
    setFlag(7, X & 0b10000000);
    cycles = 2;
    break;
  }
  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;

    uint8_t val = read(addr);
    X = val;
    // Zero bit
    setFlag(1, X == 0);

    // Negative bit
    setFlag(7, X & 0b10000000);
    cycles = 4;
    break;
  }
  case addressing::zero: {
    uint16_t zaddr = read(++PC);

    uint8_t val = read(zaddr);
    X = val;
    // Zero bit
    setFlag(1, X == 0);

    // Negative bit
    setFlag(7, X & 0b10000000);
    cycles = 3;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
    break;
  }
}

void cpu::LDY(addressing mode) {
  switch (mode) {
  case addressing::immediate: {
    uint8_t val = read(++PC);
    Y = val;
    // Zero bit
    setFlag(1, Y == 0);

    // Negative bit
    setFlag(7, Y & 0b10000000);
    cycles = 2;
    break;
  }
  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;

    uint8_t val = read(addr);
    Y = val;
    // Zero bit
    setFlag(1, Y == 0);

    // Negative bit
    setFlag(7, Y & 0b10000000);
    cycles = 4;
    break;
  }
  case addressing::zero: {
    uint8_t zaddr = read(++PC);

    uint8_t val = read(0x00FF & zaddr);
    Y = val;
    // Zero bit
    setFlag(1, Y == 0);

    // Negative bit
    setFlag(7, Y & 0b10000000);
    cycles = 3;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
    break;
  }
}

void cpu::LSR(addressing mode) {
  switch (mode) {
  case accumulator:
    P ^= (A & 1);
    setFlag(0, A & 1);
    A >>= 1;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 2;
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::ORA(addressing mode) {
  switch (mode) {
  case addressing::immediate: {
    uint8_t val = read(++PC);
    uint8_t res = A | val;
    A = res;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 2;
    break;
  }
  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;

    uint8_t val = read(addr);
    uint8_t res = A | val;
    A = res;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 4;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
    break;
  }
}

void cpu::PHA(addressing mode) {
  switch (mode) {
  case addressing::implied:
    Stack[S] = A;
    S--;
    cycles = 3;
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::PHP(addressing mode) {
  switch (mode) {
  case addressing::implied:
    Stack[S] = P;
    S--;
    cycles = 3;
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::PLA(addressing mode) {
  switch (mode) {
  case implied:
    ++S;
    A = Stack[S];
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 4;
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::PLP(addressing mode) {
  switch (mode) {
  case implied:
    ++S;
    P = Stack[S];
    cycles = 4;
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::ROL(addressing mode) {
  switch (mode) {
  case accumulator: {
    uint8_t carry = P & 1;
    setFlag(0, A & 0b10000000);
    A = A << 1;
    A |= carry;

    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 2;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::ROR(addressing mode) {
  switch (mode) {
  case accumulator: {
    uint8_t carry = P & 1;
    setFlag(0, A & 0b10000000);
    uint8_t res = A << 1;
    res |= carry;
    A = res;

    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 2;
    break;
  }
  case zero: {
    uint8_t zaddr = read(++PC);
    uint8_t val = read(0x00FF & zaddr);
    uint8_t carry = P & 1;
    setFlag(0, val & 0b10000000);
    uint8_t res = val << 1;
    res |= carry;
    write(0x00FF & zaddr, res);

    // Zero bit
    setFlag(1, res == 0);

    // Negative bit
    setFlag(7, res & 0b10000000);
    cycles = 5;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::RTS(addressing mode) {
  switch (mode) {
  case implied: {
    ++S;
    uint8_t MSB = Stack[S];
    ++S;
    uint8_t LSB = Stack[S];
    PC = ((MSB << 8) | LSB);
    cycles = 6;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::SBC(addressing mode) {
  uint8_t val;
  uint8_t carry;
  uint16_t res;
  switch (mode) {
  case addressing::immediate: {
    val = read(++PC);
    carry = P & 1;
    res = A - val - !carry;
    cycles = 2;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
    break;
  }
  A = res;
  setFlag(0, res & 0x100);
  // Will need to confirm that this is correct
  // Overflow bit
  setFlag(6, (A ^ res) & (val ^ res) & 0x80);

  // Zero bit
  setFlag(1, A == 0);

  // Negative bit
  setFlag(7, A & 0b10000000);
}

void cpu::SEC(addressing mode) {
  switch (mode) {
  case addressing::implied: {
    setFlag(0, 1);
    cycles = 2;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
    break;
  }
}

void cpu::SED(addressing mode) {
  switch (mode) {
  case addressing::implied: {
    setFlag(3, 1);
    cycles = 2;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
    break;
  }
}

void cpu::SEI(addressing mode) {
  switch (mode) {
  case addressing::implied: {
    setFlag(2, 1);
    cycles = 2;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
    break;
  }
}

void cpu::STA(addressing mode) {
  switch (mode) {
  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    write(addr, A);
    cycles = 4;
    break;
  }
  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = ((addr_high << 8) | addr_low) + X;
    write(addr, A);
    cycles = 5;
    break;
  }
  case addressing::absoluteY: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = ((addr_high << 8) | addr_low) + Y;
    write(addr, A);
    cycles = 5;
    break;
  }
  case addressing::indirectY: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & zaddr);
    addr += Y;
    write(addr, A);
    cycles = 6;
    break;
  }
  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    uint8_t addr = read(0x00FF & zaddr);
    write(addr, A);
    cycles = 3;
    break;
  }
  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));
    write((addr % 0xFF), A);
    cycles = 4;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
    break;
  }
}

void cpu::STX(addressing mode) {
  switch (mode) {
  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    write(addr, X);
    cycles = 4;
    break;
  }
  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    write(0x00FF & zaddr, X);
    cycles = 3;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
    break;
  }
}

void cpu::STY(addressing mode) {
  switch (mode) {
  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    write(0x00FF & zaddr, Y);
    cycles = 3;
    break;
  }
  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    write(addr, Y);
    cycles = 4;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
    break;
  }
}

void cpu::TAX(addressing mode) {
  switch (mode) {
  case implied:
    X = A;
    // Zero bit
    setFlag(1, X == 0);

    // Negative bit
    setFlag(7, X & 0b10000000);
    cycles = 2;
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::TAY(addressing mode) {
  switch (mode) {
  case implied:
    Y = A;
    // Zero bit
    setFlag(1, Y == 0);

    // Negative bit
    setFlag(7, Y & 0b10000000);
    cycles = 2;
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::TSX(addressing mode) {
  switch (mode) {
  case implied:
    X = S;
    // Zero bit
    setFlag(1, X == 0);

    // Negative bit
    setFlag(7, X & 0b10000000);
    cycles = 2;
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::TXS(addressing mode) {
  switch (mode) {
  case implied:
    S = X;
    // Zero bit
    setFlag(1, S == 0);

    // Negative bit
    setFlag(7, S & 0b10000000);
    cycles = 2;
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::TYA(addressing mode) {
  switch (mode) {
  case implied:
    A = Y;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 2;
    break;
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}
