#include "cpu.hpp"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <sys/types.h>
#include <vector>

cpu::cpu(const std::shared_ptr<ROM> rom, std::shared_ptr<ppu> PPU)
    : m_cart(rom), m_ppu(PPU) {
  m_PRG_ROM = m_cart->getPRG();
}

void cpu::reset() {
  PC = ((read(0xFFFD) << 8) | read(0xFFFC));
  A = 0;
  S = 253;
  A = 0;
  X = 0;
  Y = 0;
  P = 0;
}

uint8_t cpu::read(uint16_t addr) {
  switch (addr) {
  case 0x0000 ... 0x1FFF:
    // printf("%X\n", addr);
    // printf("%X\n", RAM[addr % 0x0800]);
    return RAM[addr % 0x0800];
  case 0x2000 ... 0x3FFF:
    return m_ppu->cpu_read(addr);
  case 0x4000 ... 0x4017:
    return 0;
  case 0x4018 ... 0x401F:
    return 0;
  case 0x4020 ... 0xFFFF: {
    uint8_t PRG_banks = m_cart->getPRGBanks();
    addr %= (PRG_banks >= 2 ? 0x8000 : 0x4000);
    // printf("%X\n", addr);
    // printf("%X\n", m_PRG_ROM[addr]);
    return m_PRG_ROM[addr];
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
    printf("PPU Write: %i, %i", (addr % 8), val);
    m_ppu->cpu_write(addr, val);
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
  printf("PC: %X\n", PC);
  printf("Instr: %x\n", op);
  executeOpcode(op);
  ++PC;
  m_ppu->tick(cycles * 3);
  // std::cin.get();
}

void cpu::executeOpcode(uint8_t op) {
  switch (op) {
  case 0x00:
    BRK(addressing::implied);
    break;
  case 0x01:
    ORA(addressing::indirectX);
    break;
  case 0x05:
    ORA(addressing::zero);
    break;
  case 0x06:
    ASL(addressing::zero);
    break;
  case 0x08:
    PHP(addressing::implied);
    break;
  case 0x09:
    ORA(addressing::immediate);
    break;
  case 0x0A:
    ASL(addressing::accumulator);
    break;
  case 0x0D:
    ORA(addressing::absolute);
    break;
  case 0x0E:
    ASL(addressing::absolute);
    break;
  case 0x10:
    BPL(addressing::relative);
    break;
  case 0x11:
    ORA(addressing::indirectY);
    break;
  case 0x15:
    ORA(addressing::zeroX);
    break;
  case 0x16:
    ASL(addressing::zeroX);
    break;
  case 0x18:
    CLC(addressing::implied);
    break;
  case 0x19:
    ORA(addressing::absoluteY);
    break;
  case 0x1D:
    ORA(addressing::absoluteX);
    break;
  case 0x1E:
    ASL(addressing::absoluteX);
    break;
  case 0x20:
    JSR(addressing::absolute);
    break;
  case 0x21:
    AND(addressing::indirectX);
    break;
  case 0x24:
    BIT(addressing::zero);
    break;
  case 0x25:
    AND(addressing::zero);
    break;
  case 0x26:
    ROL(addressing::zero);
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
  case 0x2D:
    AND(addressing::absolute);
    break;
  case 0x2E:
    ROL(addressing::absolute);
    break;
  case 0x30:
    BMI(addressing::relative);
    break;
  case 0x31:
    AND(addressing::indirectY);
    break;
  case 0x35:
    AND(addressing::zeroX);
    break;
  case 0x36:
    ROL(addressing::zeroX);
    break;
  case 0x38:
    SEC(addressing::implied);
    break;
  case 0x39:
    AND(addressing::absoluteY);
    break;
  case 0x3D:
    AND(addressing::absoluteX);
    break;
  case 0x3E:
    ROL(addressing::absoluteX);
    break;
  case 0x40:
    RTI(addressing::implied);
    break;
  case 0x41:
    EOR(addressing::indirectX);
    break;
  case 0x45:
    EOR(addressing::zero);
    break;
  case 0x46:
    LSR(addressing::zero);
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
  case 0x4D:
    EOR(addressing::absolute);
    break;
  case 0x4E:
    LSR(addressing::absolute);
    break;
  case 0x50:
    BVC(addressing::relative);
    break;
  case 0x51:
    EOR(addressing::indirectY);
    break;
  case 0x55:
    EOR(addressing::zeroX);
    break;
  case 0x56:
    LSR(addressing::zeroX);
    break;
  case 0x58:
    CLI(addressing::implied);
    break;
  case 0x59:
    EOR(addressing::absoluteY);
    break;
  case 0x5D:
    EOR(addressing::absoluteX);
    break;
  case 0x5E:
    LSR(addressing::absoluteX);
    break;
  case 0x60:
    RTS(addressing::implied);
    break;
  case 0x61:
    ADC(addressing::indirectX);
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
  case 0x6E:
    ROR(addressing::absolute);
    break;
  case 0x70:
    BVS(addressing::relative);
    break;
  case 0x71:
    ADC(addressing::indirectY);
    break;
  case 0x75:
    ADC(addressing::zeroX);
    break;
  case 0x76:
    ROR(addressing::zeroX);
    break;
  case 0x78:
    SEI(addressing::implied);
    break;
  case 0x79:
    ADC(addressing::absoluteY);
    break;
  case 0x7D:
    ADC(addressing::absoluteX);
    break;
  case 0x7E:
    ROR(addressing::absoluteX);
    break;
  case 0x81:
    STA(addressing::indirectX);
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
  case 0x8A:
    TXA(addressing::implied);
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
  case 0x94:
    STY(addressing::zeroX);
    break;
  case 0x95:
    STA(addressing::zeroX);
    break;
  case 0x96:
    STX(addressing::zeroY);
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
  case 0xA1:
    LDA(addressing::indirectX);
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
  case 0xB4:
    LDY(addressing::zeroX);
    break;
  case 0xB5:
    LDA(addressing::zeroX);
    break;
  case 0xB6:
    LDX(addressing::zeroY);
    break;
  case 0xB8:
    CLV(addressing::implied);
    break;
  case 0xB9:
    LDA(addressing::absoluteY);
    break;
  case 0xBA:
    TSX(addressing::implied);
    break;
  case 0xBC:
    LDY(addressing::absoluteX);
    break;
  case 0xBD:
    LDA(addressing::absoluteX);
    break;
  case 0xBE:
    LDX(addressing::absoluteY);
    break;
  case 0xC0:
    CPY(addressing::immediate);
    break;
  case 0xC1:
    CMP(addressing::indirectX);
    break;
  case 0xC4:
    CPY(addressing::zero);
    break;
  case 0xC5:
    CMP(addressing::zero);
    break;
  case 0xC6:
    DEC(addressing::zero);
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
  case 0xCC:
    CPY(addressing::absolute);
    break;
  case 0xCD:
    CMP(addressing::absolute);
    break;
  case 0xCE:
    DEC(addressing::absolute);
    break;
  case 0xD0:
    BNE(addressing::relative);
    break;
  case 0xD1:
    CMP(addressing::indirectY);
    break;
  case 0xD5:
    CMP(addressing::zeroX);
    break;
  case 0xD6:
    DEC(addressing::zeroX);
    break;
  case 0xD8:
    CLD(addressing::implied);
    break;
  case 0xD9:
    CMP(addressing::absoluteY);
    break;
  case 0xDD:
    CMP(addressing::absoluteX);
    break;
  case 0xDE:
    DEC(addressing::absoluteX);
    break;
  case 0xE0:
    CPX(addressing::immediate);
    break;
  case 0xE1:
    SBC(addressing::indirectX);
    break;
  case 0xE4:
    CPX(addressing::zero);
    break;
  case 0xE5:
    SBC(addressing::zero);
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
  case 0xEA:
    NOP(addressing::implied);
    break;
  case 0xEC:
    CPX(addressing::absolute);
    break;
  case 0xEE:
    INC(addressing::absolute);
    break;
  case 0xED:
    SBC(addressing::absolute);
    break;
  case 0xF0:
    BEQ(addressing::relative);
    break;
  case 0xF1:
    SBC(addressing::indirectY);
    break;
  case 0xF5:
    SBC(addressing::zeroX);
    break;
  case 0xF6:
    INC(addressing::zeroX);
    break;
  case 0xF8:
    SED(addressing::implied);
    break;
  case 0xF9:
    SBC(addressing::absoluteY);
    break;
  case 0xFD:
    SBC(addressing::absoluteX);
    break;
  case 0xFE:
    INC(addressing::absoluteX);
    break;
  default:
    printf("Unimplemented/Incorrent OP: %X\n", op);
    std::cin.get();
  }
}

void cpu::NMI() {
  SEI(addressing::implied);
  printf("NMI: %X, PPUVADDR: %X", PC, m_ppu->peakregister(6));
  uint8_t LSB = (PC & 0xFF);
  uint8_t MSB = (PC >> 8);
  Stack[S] = MSB;
  --S;
  Stack[S] = LSB;
  --S;
  Stack[S] = P;
  --S;

  uint8_t lsb = read(0xFFFA);
  uint8_t msb = read(0xFFFB);
  PC = (msb << 8) + lsb;
  return;
}

// Instructions

void cpu::ADC(addressing mode) {
  uint16_t sum;
  uint8_t val;
  switch (mode) {
  case addressing::immediate: {
    val = read(++PC);
    sum = A + (val + (P & 1));
    cycles = 2;
    break;
  }

  case addressing::absolute: {
    uint8_t addr_high = read(++PC);
    uint8_t addr_low = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    val = read(addr);
    sum = A + (val + (P & 1));
    cycles = 4;
    break;
  }

  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + X;

    val = read(addr);
    sum = A + (val + (P & 1));
    cycles = 4;
    pageCross(raddr, addr);
    break;
  }

  case addressing::absoluteY: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + Y;

    val = read(addr);
    sum = A + (val + (P & 1));
    cycles = 4;
    pageCross(raddr, addr);
    break;
  }

  case addressing::indirectX: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read((temp_addr + X) & 0x00FF);
    uint8_t addr_high = read((temp_addr + X + 1) & 0x00FF);
    uint16_t addr = (addr_high << 8) | addr_low;

    val = read(addr);
    sum = A + (val + (P & 1));
    cycles = 6;
    break;
  }

  case addressing::indirectY: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read(temp_addr & 0x00FF);
    uint8_t addr_high = read((temp_addr + 1) & 0x00FF);

    uint16_t raddr = ((addr_high << 8) | addr_low);
    uint16_t addr = raddr + Y;

    val = read(addr);
    sum = A + (val + (P & 1));
    cycles = 5;
    pageCross(raddr, addr);
    break;
  }

  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    val = read(0x00FF & zaddr);
    sum = A + (val + (P & 1));
    cycles = 3;
    break;
  }

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));

    val = read(addr);
    sum = A + (val + (P & 1));
    cycles = 4;
    break;
  }

  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
    break;
  }
  // checks

  // Carry bit
  setFlag(0, sum & 0x100);
  // Will need to confirm that this is correct
  // Overflow bit
  setFlag(6, (A ^ sum) & (val ^ sum) & 0x80);

  // Zero bit
  setFlag(1, sum == 0);

  // Negative bit
  setFlag(7, sum & 0b10000000);

  A = sum;
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

  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    val = read(addr);
    A = A & val;
    cycles = 4;
    break;
  }

  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + X;
    val = read(addr);
    A = A & val;
    cycles = 4;
    cycles += pageCross(raddr, addr);
    break;
  }

  case addressing::absoluteY: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + Y;
    val = read(addr);
    A = A & val;
    cycles = 4;
    cycles += pageCross(raddr, addr);
    break;
  }

  case addressing::indirectX: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read((temp_addr + X) & 0x00FF);
    uint8_t addr_high = read((temp_addr + X + 1) & 0x00FF);

    uint16_t addr = (addr_high << 8) | addr_low;
    val = read(addr);
    A = A & val;
    cycles = 6;
    break;
  }

  case addressing::indirectY: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read(temp_addr & 0x00FF);
    uint8_t addr_high = read((temp_addr + 1) & 0x00FF);

    uint16_t raddr = ((addr_high << 8) | addr_low);
    uint16_t addr = raddr + Y;
    val = read(addr);
    A = A & val;
    cycles = 5;
    cycles += pageCross(raddr, addr);
    break;
  }

  case addressing::zero: {
    uint8_t addr = read(++PC);
    val = read(0x00FF & addr);
    A = A & val;
    cycles = 3;
    break;
  }

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));
    val = read(addr);
    A = A & val;
    cycles = 4;
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
  case addressing::accumulator: {
    setFlag(0, (A & 0b10000000) >> 7);
    A = A << 1;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 5;
    break;
  }

  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    uint8_t val = read(addr);
    setFlag(0, (val & 0b10000000) >> 7);
    uint8_t res = val << 1;

    write(addr, res);

    // Zero bit
    setFlag(1, res == 0);

    // Negative bit
    setFlag(7, res & 0b10000000);
    cycles = 6;
    break;
  }

  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + X;
    uint8_t val = read(addr);

    setFlag(0, (val & 0b10000000) >> 7);
    uint8_t res = val << 1;

    write(addr, res);

    // Zero bit
    setFlag(1, res == 0);

    // Negative bit
    setFlag(7, res & 0b10000000);
    cycles = 7;
    break;
  }

  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    uint8_t val = read(0x00FF & zaddr);
    setFlag(0, (val & 0b10000000) >> 7);
    uint8_t res = val << 1;

    write(0x00FF & zaddr, res);

    // Zero bit
    setFlag(1, res == 0);

    // Negative bit
    setFlag(7, res & 0b10000000);
    cycles = 5;
    break;
  }

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));
    uint8_t val = read(addr);

    setFlag(0, (val & 0b10000000) >> 7);
    uint8_t res = val << 1;

    write(addr, res);

    // Zero bit
    setFlag(1, res == 0);

    // Negative bit
    setFlag(7, res & 0b10000000);
    cycles = 6;
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

    // Zero bit
    setFlag(1, (A & val) == 0);

    // Negative bit
    setFlag(6, (val) & 0b01000000);

    // Negative bit
    setFlag(7, (val) & 0b10000000);
    cycles = 4;
    break;
  }
  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    uint8_t val = read(0x00FF & zaddr);

    // Zero bit
    setFlag(1, (A & val) == 0);

    // Negative bit
    setFlag(6, (val >> 6) & 1);

    // Negative bit
    setFlag(7, (val >> 7) & 1);
    cycles = 3;
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
    uint8_t neg = (P >> 7) & 1;
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

void cpu::BRK(addressing mode) {
  switch (mode) {
  case addressing::implied: {
    uint8_t LSB = (PC & 0xFF);
    uint8_t MSB = (PC >> 8);
    Stack[S] = MSB;
    --S;
    Stack[S] = LSB;
    --S;
    Stack[S] = P;
    --S;

    setFlag(4, 1);
    PC = ((read(0xFFFF) << 8) | read(0xFFFE));
    cycles = 7;
    break;
  }
  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::BVC(addressing mode) {
  switch (mode) {
  case addressing::relative: {
    uint8_t overflow = (P >> 6) & 1;
    uint8_t jmp_val = read(++PC);
    cycles = 2;
    if (!overflow) {
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

void cpu::CLV(addressing mode) {
  switch (mode) {
  case implied: {
    setFlag(6, 0);
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
    setFlag(7, ((A - val) >> 7) & 1);
    cycles = 2;
    break;
  }

  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;

    uint8_t val = read(addr);
    // Carry
    setFlag(0, A >= val);

    // Zero bit
    setFlag(1, A == val);

    // Negative bit
    setFlag(7, ((A - val) >> 7) & 1);
    cycles = 4;
    break;
  }

  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + X;

    uint8_t val = read(addr);
    // Carry
    setFlag(0, A >= val);

    // Zero bit
    setFlag(1, A == val);

    // Negative bit
    setFlag(7, ((A - val) >> 7) & 1);
    cycles = 4;
    pageCross(raddr, addr);
    break;
  }

  case addressing::absoluteY: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + Y;

    uint8_t val = read(addr);
    // Carry
    setFlag(0, A >= val);

    // Zero bit
    setFlag(1, A == val);

    // Negative bit
    setFlag(7, ((A - val) >> 7) & 1);
    cycles = 4;
    pageCross(raddr, addr);
    break;
  }

  case addressing::indirectX: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read((temp_addr + X) & 0x00FF);
    uint8_t addr_high = read((temp_addr + X + 1) & 0x00FF);

    uint16_t addr = (addr_high << 8) | addr_low;

    uint8_t val = read(addr);
    // Carry
    setFlag(0, A >= val);

    // Zero bit
    setFlag(1, A == val);

    // Negative bit
    setFlag(7, ((A - val) >> 7) & 1);
    cycles = 6;
    break;
  }

  case addressing::indirectY: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read(temp_addr & 0x00FF);
    uint8_t addr_high = read((temp_addr + 1) & 0x00FF);

    uint16_t raddr = ((addr_high << 8) | addr_low);
    uint16_t addr = raddr + Y;

    uint8_t val = read(addr);
    // Carry
    setFlag(0, A >= val);

    // Zero bit
    setFlag(1, A == val);

    // Negative bit
    setFlag(7, ((A - val) >> 7) & 1);
    cycles = 5;
    pageCross(raddr, addr);
    break;
  }

  case addressing::zero: {
    uint8_t addr = read(++PC);
    uint8_t val = read(0x00FF & addr);

    // Carry
    setFlag(0, A >= val);

    // Zero bit
    setFlag(1, A == val);

    // Negative bit
    setFlag(7, ((A - val) >> 7) & 1);
    cycles = 3;
    break;
  }

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));

    uint8_t val = read(0x00FF & addr);

    // Carry
    setFlag(0, A >= val);

    // Zero bit
    setFlag(1, A == val);

    // Negative bit
    setFlag(7, ((A - val) >> 7) & 1);
    cycles = 4;
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

  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;

    uint8_t val = read(addr);
    // Carry
    setFlag(0, X >= val);

    // Zero bit
    setFlag(1, X == val);

    // Negative bit
    setFlag(7, (X - val) & 0b10000000);
    cycles = 4;
    break;
  }

  case addressing::zero: {
    uint8_t addr = read(++PC);
    uint8_t val = read(0x00FF & addr);

    // Carry
    setFlag(0, X >= val);

    // Zero bit
    setFlag(1, X == val);

    // Negative bit
    setFlag(7, (X - val) & 0b10000000);
    cycles = 3;
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

  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;

    uint8_t val = read(addr);
    // Carry
    setFlag(0, Y >= val);

    // Zero bit
    setFlag(1, Y == val);

    // Negative bit
    setFlag(7, (Y - val) & 0b10000000);
    cycles = 4;
    break;
  }

  case addressing::zero: {
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

void cpu::DEC(addressing mode) {
  switch (mode) {
  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    uint8_t val = read(0x00FF & zaddr);
    val--;
    write(0x00FF & zaddr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val & 0b10000000);
    cycles = 5;
    break;
  }

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));
    uint8_t val = read(0x00FF & addr);
    val--;
    write(0x00FF & zaddr, val);
    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val & 0b10000000);
    cycles = 6;
    break;
  }

  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    uint8_t val = read(addr);
    val--;
    write(addr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val & 0b10000000);
    cycles = 6;
    break;
  }

  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + X;
    uint8_t val = read(addr);
    val++;
    write(addr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val & 0b10000000);
    cycles = 7;
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

  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;

    uint8_t val = read(addr);
    A ^= val;

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
    A ^= val;

    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 4;
    pageCross(raddr, addr);
    break;
  }

  case addressing::absoluteY: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + Y;

    uint8_t val = read(addr);
    A ^= val;

    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 4;
    pageCross(raddr, addr);
    break;
  }

  case addressing::indirectX: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read((temp_addr + X) & 0x00FF);
    uint8_t addr_high = read((temp_addr + X + 1) & 0x00FF);

    uint16_t addr = (addr_high << 8) | addr_low;

    uint8_t val = read(addr);
    A ^= val;

    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 6;
    break;
  }

  case addressing::indirectY: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read(temp_addr & 0x00FF);
    uint8_t addr_high = read((temp_addr + 1) & 0x00FF);

    uint16_t raddr = ((addr_high << 8) | addr_low);
    uint16_t addr = raddr + Y;

    uint8_t val = read(addr);
    A ^= val;

    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A & 0b10000000);
    cycles = 5;
    pageCross(raddr, addr);
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

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));

    uint8_t val = read(addr);
    A ^= val;

    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
    cycles = 4;
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
    setFlag(7, val >> 7);
    cycles = 6;
    break;
  }

  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + X;

    uint8_t val = read(addr);
    val++;
    write(addr, val);
    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
    cycles = 7;
    break;
  }

  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    uint8_t val = read(0x00FF & zaddr);
    val++;
    write(0x00FF & zaddr, val);
    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
    cycles = 5;
    break;
  }

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));

    uint8_t val = read(0x00FF & addr);
    val++;
    write(0x00FF & addr, val);
    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
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
    setFlag(7, X >> 7);
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
    setFlag(7, X >> 7);
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
  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    printf("JADDR: %X\n", addr);
    PC = addr - 1;
    cycles = 3;
    break;
  }
  case addressing::indirect: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    printf("indirect addr: %X", addr);

    uint8_t jaddr_low = read(addr);
    uint8_t jaddr_high = read(++addr);
    uint16_t jmp_addr = (jaddr_high << 8) | jaddr_low;
    printf("JADDR: %X\n", jmp_addr);
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
    Stack[S] = MSB;
    --S;
    Stack[S] = LSB;
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
    setFlag(7, A >> 7);
    cycles = 2;
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
    setFlag(7, A >> 7);
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
    setFlag(7, A >> 7);
    cycles = 4;
    cycles += pageCross(raddr, addr);
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
    setFlag(7, A >> 7);
    cycles = 4;
    cycles += pageCross(raddr, addr);
    break;
  }

  case addressing::indirectX: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read((temp_addr + X) & 0x00FF);
    uint8_t addr_high = read((temp_addr + X + 1) & 0x00FF);

    uint16_t addr = (addr_high << 8) | addr_low;
    uint8_t val = read(addr);
    A = val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
    cycles = 6;
    break;
  }

  case addressing::indirectY: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read(temp_addr);
    uint8_t addr_high = read((temp_addr + 1) & 0x00FF);

    uint16_t raddr = ((addr_high << 8) | addr_low);
    uint16_t addr = raddr + Y;
    uint8_t val = read(addr);
    printf("%X, %X, %X, %X\n", addr_low, addr_high, addr, val);
    A = val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
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
    setFlag(7, A >> 7);
    cycles = 3;
    break;
  }

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));
    uint8_t val = read(addr);
    A = val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
    cycles = 4;
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
    setFlag(7, X >> 7);
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
    setFlag(7, X >> 7);
    cycles = 4;
    break;
  }

  case addressing::absoluteY: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + Y;

    uint8_t val = read(addr);
    X = val;
    // Zero bit
    setFlag(1, X == 0);

    // Negative bit
    setFlag(7, X >> 7);
    cycles = 4;
    cycles += pageCross(raddr, addr);
    break;
  }

  case addressing::zero: {
    uint16_t zaddr = read(++PC);

    uint8_t val = read(zaddr);
    X = val;
    // Zero bit
    setFlag(1, X == 0);

    // Negative bit
    setFlag(7, X >> 7);
    cycles = 3;
    break;
  }

  case addressing::zeroY: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + Y));

    uint8_t val = read(addr);
    X = val;
    // Zero bit
    setFlag(1, X == 0);

    // Negative bit
    setFlag(7, X >> 7);
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

void cpu::LDY(addressing mode) {
  switch (mode) {
  case addressing::immediate: {
    uint8_t val = read(++PC);
    Y = val;
    // Zero bit
    setFlag(1, Y == 0);

    // Negative bit
    setFlag(7, Y >> 7);
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
    setFlag(7, Y >> 7);
    cycles = 4;
    break;
  }

  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + X;

    uint8_t val = read(addr);
    Y = val;
    // Zero bit
    setFlag(1, Y == 0);

    // Negative bit
    setFlag(7, Y >> 7);
    cycles = 4;
    pageCross(raddr, addr);
    break;
  }

  case addressing::zero: {
    uint8_t zaddr = read(++PC);

    uint8_t val = read(0x00FF & zaddr);
    Y = val;
    // Zero bit
    setFlag(1, Y == 0);

    // Negative bit
    setFlag(7, Y >> 7);
    cycles = 3;
    break;
  }

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));

    uint8_t val = read(addr);
    Y = val;
    // Zero bit
    setFlag(1, Y == 0);

    // Negative bit
    setFlag(7, Y >> 7);
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

void cpu::LSR(addressing mode) {
  switch (mode) {
  case accumulator:
    setFlag(0, A & 1);
    A >>= 1;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
    cycles = 2;
    break;

  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;
    uint8_t val = read(addr);

    setFlag(0, val & 1);

    val >>= 1;
    write(addr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
    cycles = 6;
    break;
  }

  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + X;

    uint8_t val = read(addr);

    setFlag(0, val & 1);

    val >>= 1;
    write(addr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
    cycles = 7;
    break;
  }

  case addressing::zero: {
    uint8_t addr = read(++PC);
    uint8_t val = read(0x00FF & addr);

    setFlag(0, val & 1);

    val >>= 1;
    write(0x00FF & addr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
    cycles = 5;
    break;
  }

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));

    uint8_t val = read(addr);

    setFlag(0, val & 1);

    val >>= 1;
    write(addr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
    cycles = 6;
    break;
  }

  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::NOP(addressing mode) {
  switch (mode) {
  case addressing::implied:
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
    A |= val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
    cycles = 2;
    break;
  }

  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;

    uint8_t val = read(addr);
    A |= val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
    cycles = 4;
    break;
  }

  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + X;
    uint8_t val = read(addr);

    A |= val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
    cycles = 4;
    cycles += pageCross(raddr, addr);
    break;
  }

  case addressing::absoluteY: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + Y;
    uint8_t val = read(addr);

    A |= val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
    cycles = 4;
    cycles += pageCross(raddr, addr);
    break;
  }

  case addressing::indirectX: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read((temp_addr + X) & 0x00FF);
    uint8_t addr_high = read((temp_addr + X + 1) & 0x00FF);

    uint16_t addr = (addr_high << 8) | addr_low;
    uint8_t val = read(addr);

    A |= val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
    cycles = 6;
    break;
  }

  case addressing::indirectY: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read(temp_addr & 0x00FF);
    uint8_t addr_high = read((temp_addr + 1) & 0x00FF);

    uint16_t raddr = ((addr_high << 8) | addr_low);
    uint16_t addr = raddr + Y;
    uint8_t val = read(addr);

    A |= val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
    cycles = 5;
    cycles += pageCross(raddr, addr);
    break;
  }

  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    uint8_t val = read(0x00FF & zaddr);
    A |= val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
    cycles = 3;
    break;
  }

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint8_t addr = read(0x00FF & (zaddr + X));
    uint8_t val = read(0x00FF & addr);
    A |= val;
    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
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
    setFlag(7, A >> 7);
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
    setFlag(0, A >> 7);
    A <<= 1;
    A |= carry;

    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
    cycles = 2;
    break;
  }

  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;

    uint8_t val = read(addr);
    uint8_t carry = P & 1;
    setFlag(0, val >> 7);
    val <<= 1;
    val |= carry;
    write(addr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
    cycles = 6;
    break;
  }

  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + X;

    uint8_t val = read(addr);
    uint8_t carry = P & 1;
    setFlag(0, val >> 7);
    val <<= 1;
    val |= carry;
    write(addr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
    cycles = 7;
    break;
  }

  case addressing::zero: {
    uint8_t addr = read(++PC);
    uint8_t val = read(0x00FF & addr);

    uint8_t carry = P & 1;
    setFlag(0, val >> 7);
    val <<= 1;
    val |= carry;
    write(addr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
    cycles = 5;
    break;
  }

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));

    uint8_t val = read(addr);
    uint8_t carry = P & 1;
    setFlag(0, val >> 7);
    val <<= 1;
    val |= carry;
    write(addr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
    cycles = 6;
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
    setFlag(0, A & 1);
    uint8_t res = A >> 1;
    res |= carry << 7;
    A = res;

    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
    cycles = 2;
    break;
  }

  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;

    uint8_t val = read(addr);
    uint8_t carry = P & 1;

    setFlag(0, val & 1);
    val >>= 1;
    val |= carry << 7;
    write(addr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
    cycles = 6;
    break;
  }

  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + X;

    uint8_t val = read(addr);
    uint8_t carry = P & 1;

    setFlag(0, val & 1);
    val >>= 1;
    val |= carry << 7;
    write(addr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
    cycles = 7;
    break;
  }

  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    uint8_t val = read(0x00FF & zaddr);
    uint8_t carry = P & 1;

    setFlag(0, val & 1);
    val >>= 1;
    val |= carry << 7;
    write(0x00FF & zaddr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
    cycles = 5;
    break;
  }

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));

    uint8_t val = read(addr);
    uint8_t carry = P & 1;

    setFlag(0, val & 1);
    val >>= 1;
    val |= carry << 7;
    write(addr, val);

    // Zero bit
    setFlag(1, val == 0);

    // Negative bit
    setFlag(7, val >> 7);
    cycles = 6;
    break;
  }

  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::RTI(addressing mode) {
  switch (mode) {
  case implied: {
    ++S;
    P = Stack[S];
    ++S;
    uint8_t LSB = Stack[S];
    ++S;
    uint8_t MSB = Stack[S];
    PC = ((MSB << 8) | LSB) - 1;
    cycles = 6;
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
    uint8_t LSB = Stack[S];
    ++S;
    uint8_t MSB = Stack[S];
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
    val = read(++PC) ^ 0xFF;
    carry = P & 1;
    res = A + val + carry;
    cycles = 2;
    break;
  }

  case addressing::absolute: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t addr = (addr_high << 8) | addr_low;

    val = read(addr) ^ 0xFF;
    carry = P & 1;
    res = A + val + carry;
    cycles = 4;
    break;
  }

  case addressing::absoluteX: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + X;

    val = read(addr) ^ 0xFF;
    carry = P & 1;
    res = A + val + carry;
    cycles = 4;
    cycles += pageCross(raddr, addr);
    break;
  }

  case addressing::absoluteY: {
    uint8_t addr_low = read(++PC);
    uint8_t addr_high = read(++PC);
    uint16_t raddr = (addr_high << 8) | addr_low;
    uint16_t addr = raddr + Y;

    val = read(addr) ^ 0xFF;
    carry = P & 1;
    res = A + val + carry;
    cycles = 4;
    cycles += pageCross(raddr, addr);
    break;
  }

  case addressing::indirectX: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read((temp_addr + X) & 0x00FF);
    uint8_t addr_high = read((temp_addr + X + 1) & 0x00FF);

    uint16_t addr = (addr_high << 8) | addr_low;

    val = read(addr) ^ 0xFF;
    carry = P & 1;
    res = A + val + carry;
    cycles = 6;
    break;
  }

  case addressing::indirectY: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read(temp_addr & 0x00FF);
    uint8_t addr_high = read((temp_addr + 1) & 0x00FF);

    uint16_t raddr = ((addr_high << 8) | addr_low);
    uint16_t addr = raddr + Y;

    val = read(addr) ^ 0xFF;
    carry = P & 1;
    res = A + val + carry;
    cycles = 5;
    pageCross(raddr, addr);
    break;
  }

  case addressing::zero: {
    uint8_t addr = read(++PC);

    val = read(addr) ^ 0xFF;
    carry = P & 1;
    res = A + val + carry;
    cycles = 3;
    break;
  }

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));

    val = read(addr) ^ 0xFF;
    carry = P & 1;
    res = A + val + carry;
    cycles = 4;
    break;
  }

  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
    break;
  }

  setFlag(0, res & 0xFF00);
  // Will need to confirm that this is correct
  // Overflow bit
  setFlag(6, (A ^ res) & (val ^ res) & 0x80);

  // Zero bit
  setFlag(1, (res & 0x00FF) == 0);

  // Negative bit
  setFlag(7, (res & 0x00FF) >> 7);
  A = res & 0x00FF;
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

  case addressing::indirectX: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read((temp_addr + X) & 0x00FF);
    uint8_t addr_high = read((temp_addr + X + 1) & 0x00FF);
    uint16_t addr = (addr_high << 8) | addr_low;

    write(addr, A);
    cycles = 6;
    break;
  }

  case addressing::indirectY: {
    uint8_t temp_addr = read(++PC);
    uint8_t addr_low = read(temp_addr & 0x00FF);
    uint8_t addr_high = read((temp_addr + 1) & 0x00FF);

    uint16_t raddr = ((addr_high << 8) | addr_low);
    uint16_t addr = raddr + Y;

    write(addr, A);
    printf("STA INY: %X, %X, %X\n", addr, A, Y);
    cycles = 6;
    break;
  }

  case addressing::zero: {
    uint8_t addr = read(++PC);
    write(0x00FF & addr, A);
    printf("%X, %X\n", addr, A);
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

  case addressing::zeroY: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + Y));

    write(0x00FF & addr, X);
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

void cpu::STY(addressing mode) {
  switch (mode) {
  case addressing::zero: {
    uint8_t zaddr = read(++PC);
    write(0x00FF & zaddr, Y);
    cycles = 3;
    break;
  }

  case addressing::zeroX: {
    uint8_t zaddr = read(++PC);
    uint16_t addr = read(0x00FF & (zaddr + X));

    write(0x00FF & addr, Y);
    cycles = 4;
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
    setFlag(7, X >> 7);
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
    setFlag(7, Y >> 7);
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
    setFlag(7, X >> 7);
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
    S = X - 1;
    // Zero bit
    setFlag(1, S == 0);

    // Negative bit
    setFlag(7, S >> 7);
    cycles = 2;
    break;

  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}

void cpu::TXA(addressing mode) {
  switch (mode) {
  case implied:
    A = X;

    // Zero bit
    setFlag(1, A == 0);

    // Negative bit
    setFlag(7, A >> 7);
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
    setFlag(7, A >> 7);
    cycles = 2;
    break;

  default:
    printf("Instruction called with an invalid addressing mode: %s, %i\n",
           __FUNCTION__, mode);
    abort();
  }
}
