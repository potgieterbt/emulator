#include "cpu.h"
#include <cstdint>
#include <cstring>

void Chip::emulateCycle() {
  uint8_t opcode = Bus.Read(pc);
  pc += 1;

  switch (opcode) {
  case 0x00:
    mode = Implicit;
    BRK(mode);
    break;

  case 0x01:
    mode = IdxIndirect;
    ORA(mode);
    break;

  case 0x02:
    // Illegal opcode I think
    break;

  case 0x03:
    // Illegal opcode
    break;

  case 0x04:
    mode = Implicit;
    NOP(mode);
    break;

  case 0x05:
    mode = Zero;
    ORA(mode);
    break;

  case 0x06:
    mode = Zero;
    ASL(mode);
    break;

  case 0x07:
    // Illegal opcode
    break;

  case 0x08:
    mode = Implicit;
    PHP(mode);
    break;

  case 0x09:
    mode = Immediate;
    ORA(mode);
    break;

  case 0x0A:
    mode = Implicit;
    ASL(mode);
    break;

  case 0x0B:
    // Illegal opcode
    break;

  case 0x0C:
    NOP(mode);
    break;

  case 0x0D:
    mode = Absolute;
    ORA(mode);
    break;

  case 0x0E:
    mode = Absolute;
    ASL(mode);
    break;

  case 0x0F:
    // Illegal opcode
    break;

  case 0x10:
    mode = Relative;
    BPL(mode);
    break;

  case 0x11:
    mode = IndirectIdx;
    ORA(mode);
    break;

  case 0x12:
    // Illegal opcode
    break;

  case 0x13:
    // Illegal opcode
    break;

  case 0x14:
    NOP(mode);
    break;

  case 0x15:
    mode = IdxIndirect;
    ORA(mode);
    break;

  case 0x16:
    mode = IdxIndirect;
    ASL(mode);
    break;

  case 0x17:
    // Illegal opcode
    break;

  case 0x18:
    mode = Implicit;
    CLC(mode);
    break;

  case 0x19:
    mode = Absolutey;
    ORA(mode);
    break;

  case 0x1A:
    NOP(mode);
    break;

  case 0x1B:
    // Illegal opcode
    break;

  case 0x1C:
    NOP(mode);
    break;

  case 0x1D:
    mode = Absolutex;
    ORA(mode);
    break;

  case 0x1E:
    mode = Absolutex;
    ASL(mode);
    break;

  case 0x1F:
    // Illegal opcode
    break;

  case 0x20:
    mode = Absolute;
    JSR(mode);
    break;

  case 0x21:
    mode = IndirectIdx;
    AND(mode);
    break;

  case 0x22:
    // Illegal opcode
    break;

  case 0x23:
    // Illegal opcode
    break;

  case 0x24:
    mode = Zero;
    BIT(mode);
    break;

  case 0x25:
    mode = Zero;
    AND(mode);
    break;

  case 0x26:
    mode = Zero;
    ROL(mode);
    break;

  case 0x27:
    // Illegal opcode
    break;

  case 0x28:
    mode = Implicit;
    PLP(mode);
    break;

  case 0x29:
    mode = Immediate;
    AND(mode);
    break;

  case 0x2A:
    mode = Implicit;
    ROL(mode);
    break;

  case 0x2B:
    // Illegal opcode
    break;

  case 0x2C:
    mode = Absolute;
    BIT(mode);
    break;

  case 0x2D:
    mode = Absolute;
    AND(mode);
    break;

  case 0x2E:
    mode = Absolute;
    LSR(mode);
    break;

  case 0x2F:
    // Illegal opcode
    break;

  case 0x30:
    mode = Relative;
    BMI(mode);
    break;

  case 0x31:
    mode = IndirectIdx;
    AND(mode);
    break;

  case 0x32:
    // Illegal opcode
    break;

  case 0x33:
    // Illegal opcode
    break;

  case 0x34:
    mode = Zerox;
    NOP(mode);
    break;

  case 0x35:
    mode = Zerox;
    AND(mode);
    break;

  case 0x36:
    mode = Zerox;
    ROL(mode);
    break;

  case 0x37:
    // Illegal opcode
    break;

  case 0x38:
    mode = Implicit;
    SEC(mode);
    break;

  case 0x39:
    mode = Absolutey;
    AND(mode);
    break;

  case 0x3A:
    NOP(mode);
    break;

  case 0x3B:
    // Illegal opcode
    break;

  case 0x3C:
    mode = Absolutex;
    NOP(mode);
    break;

  case 0x3D:
    mode = Absolutex;
    AND(mode);
    break;

  case 0x3E:
    mode = Absolutex;
    ROL(mode);
    break;

  case 0x3F:
    // Illegal opcode
    break;

  case 0x40:
    mode = Implicit;
    RTI(mode);
    break;

  case 0x41:
    mode = IdxIndirect;
    EOR(mode);
    break;

  case 0x42:
    // Illegal opcode
    break;

  case 0x43:
    // Illegal opcode
    break;

  case 0x44:
    mode = Zero;
    NOP(mode);
    break;

  case 0x45:
    mode = Zero;
    EOR(mode);
    break;

  case 0x46:
    mode = Zero;
    LSR(mode);
    break;

  case 0x47:
    // Illegal opcode
    break;

  case 0x48:
    mode = Implicit;
    PHA(mode);
    break;

  case 0x49:
    mode = Immediate;
    EOR(mode);
    break;

  case 0x4A:
    mode = Accumulator;
    LSR(mode);
    break;

  case 0x4B:
    // Illegal opcode
    break;

  case 0x4C:
    mode = Absolute;
    JMP(mode);
    break;

  case 0x4D:
    mode = Absolute;
    EOR(mode);
    break;

  case 0x4E:
    mode = Absolute;
    LSR(mode);
    break;

  case 0x4F:
    // Illegal opcode
    break;

  case 0x50:
    mode = Relative;
    BVC(mode);
    break;

  case 0x51:
    mode = IndirectIdx;
    EOR(mode);
    break;

  case 0x52:
    // Illegal opcode
    break;

  case 0x53:
    // Illegal opcode
    break;

  case 0x54:
    mode = IdxIndirect;
    NOP(mode);
    break;

  case 0x55:
    mode = IdxIndirect;
    EOR(mode);
    break;

  case 0x56:
    mode = IdxIndirect;
    LSR(mode);
    break;

  case 0x57:
    // Illegal opcode
    break;

  case 0x58:
    mode = Implicit;
    CLI(mode);
    break;

  case 0x59:
    mode = IndirectIdx;
    EOR(mode);
    break;

  case 0x5A:
    mode = Implicit;
    NOP(mode);
    break;

  case 0x5B:
    // Illegal opcode
    break;

  case 0x5C:
    mode = IdxIndirect;
    NOP(mode);
    break;

  case 0x5D:
    mode = IdxIndirect;
    EOR(mode);
    break;

  case 0x5E:
    mode = IdxIndirect;
    LSR(mode);
    break;

  case 0x5F:
    // Illegal opcode
    break;

  case 0x60:
    mode = Implicit;
    RTS(mode);
    break;

  case 0x61:
    mode = IdxIndirect;
    ADC(mode);
    break;

  case 0x62:
  case 0x63:
  case 0x64:
  case 0x65:
    mode = Zero;
    ADC(mode);
    break;

  case 0x66:
    mode = Zero;
    ROR(mode);
    break;

  case 0x67:
  case 0x68:
  case 0x69:
    mode = Immediate;
    ADC(mode);
    break;

  case 0x6A:
    mode = Accumulator;
    ROR(mode);
    break;

  case 0x6B:
  case 0x6C:
  case 0x6D:
    mode = Absolute;
    ADC(mode);
    break;

  case 0x6E:
    mode = Absolute;
    ROR(mode);
    break;

  case 0x6F:
  case 0x70:
    mode = Relative;
    BVS(mode);
    break;

  case 0x71:
    mode = Absolute;
    ADC(mode);
    break;

  case 0x72:
    // Illegal opcode
    break;

  case 0x73:
    // Illegal opcode
    break;

  case 0x74:
    // Illegal opcode
    break;

  case 0x75:
    mode = Zerox;
    ADC(mode);
    break;

  case 0x76:
    mode = Zerox;
    ROR(mode);
    break;

  case 0x77:
    // Illegal opcode
    break;

  case 0x78:
    mode = Implicit;
    SEI(mode);
    break;

  case 0x79:
    mode = Absolutey;
    ADC(mode);
    break;

  case 0x7A:
    // Illegal opcode
    break;

  case 0x7B:
    // Illegal opcode
    break;

  case 0x7C:
    // Illegal opcode
    break;

  case 0x7D:
    mode = Absolutex;
    ADC(mode);
    break;

  case 0x7E:
    mode = Absolutex;
    ROR(mode);
    break;

  case 0x7F:
    // Illegal opcode
    break;

  case 0x80:
    // Illegal opcode
    break;

  case 0x81:
    mode = IdxIndirect;
    STA(mode);
    break;

  case 0x82:
    // Illegal opcode
    break;

  case 0x83:
    // Illegal opcode
    break;

  case 0x84:
    mode = Zero;
    STY(mode);
    break;

  case 0x85:
    mode = Zero;
    STA(mode);
    break;

  case 0x86:
    mode = Zero;
    STX(mode);
    break;

  case 0x87:
    // Illegal opcode
    break;

  case 0x88:
    mode = Implicit;
    DEY(mode);
    break;

  case 0x89:
    // Illegal opcode
    break;

  case 0x8A:
    mode = Implicit;
    TXA(mode);
    break;

  case 0x8B:
    // Illegal opcode
    break;

  case 0x8C:
    mode = Absolute;
    STY(mode);
    break;

  case 0x8D:
    mode = Absolute;
    STA(mode);
    break;

  case 0x8E:
    mode = Absolute;
    STX(mode);
    break;

  case 0x8F:
    // Illegal opcode
    break;

  case 0x90:
    mode = Relative;
    BCC(mode);
    break;

  case 0x91:
    mode = IndirectIdx;
    STA(mode);
    break;

  case 0x92:
    // Illegal opcode
    break;

  case 0x93:
    // Illegal opcode
    break;

  case 0x94:
    mode = Zerox;
    STY(mode);
    break;

  case 0x95:
    mode = Zerox;
    STA(mode);
    break;

  case 0x96:
    mode = Zeroy;
    STX(mode);
    break;

  case 0x97:
    // Illegal opcode
    break;

  case 0x98:
    mode = Implicit;
    TYA(mode);
    break;

  case 0x99:
    mode = Absolutey;
    STA(mode);
    break;

  case 0x9A:
    mode = Implicit;
    TXS(mode);
    break;

  case 0x9B:
    // Illegal opcode
    break;

  case 0x9C:
    // Illegal opcode
    break;

  case 0x9D:
    mode = Absolutex;
    STA(mode);
    break;

  case 0x9E:
    // Illegal opcode
    break;

  case 0x9F:
    // Illegal opcode
    break;

  case 0xA0:
    mode = Immediate;
    LDY(mode);
    break;

  case 0xA1:
    mode = IdxIndirect;
    LDA(mode);
    break;

  case 0xA2:
    mode = Immediate;
    LDX(mode);
    break;

  case 0xA3:
    // Illegal opcode
    break;

  case 0xA4:
    mode = Zero;
    LDY(mode);
    break;

  case 0xA5:
    mode = Zero;
    LDA(mode);
    break;

  case 0xA6:
    mode = Zero;
    LDX(mode);
    break;

  case 0xA7:
    // Illegal opcode
    break;

  case 0xA8:
    mode = Implicit;
    TAY(mode);
    break;

  case 0xA9:
    mode = Immediate;
    LDA(mode);
    break;

  case 0xAA:
    mode = Implicit;
    TAX(mode);
    break;

  case 0xAB:
    // Illegal opcode
    break;

  case 0xAC:
    mode = Absolute;
    LDY(mode);
    break;

  case 0xAD:
    mode = Absolute;
    LDA(mode);
    break;

  case 0xAE:
    mode = Absolute;
    LDX(mode);
    break;

  case 0xAF:
    // Illegal opcode
    break;

  case 0xB0:
    mode = Relative;
    BCS(mode);
    break;

  case 0xB1:
    mode = IndirectIdx;
    LDA(mode);
    break;

  case 0xB2:
    // Illegal opcode
    break;

  case 0xB3:
    // Illegal opcode
    break;

  case 0xB4:
    mode = Zerox;
    LDY(mode);
    break;

  case 0xB5:
    mode = Zerox;
    LDA(mode);
    break;

  case 0xB6:
    mode = Zerox;
    LDX(mode);
    break;

  case 0xB7:
    // Illegal opcode
    break;

  case 0xB8:
    mode = Implicit;
    CLV(mode);
    break;

  case 0xB9:
    mode = Absolutey;
    LDA(mode);
    break;

  case 0xBA:
    mode = Implicit;
    TSX(mode);
    break;

  case 0xBB:
    // Illegal opcode
    break;

  case 0xBC:
    mode = Absolutex;
    LDY(mode);
    break;

  case 0xBD:
    mode = Absolutex;
    LDA(mode);
    break;

  case 0xBE:
    mode = Absolutey;
    LDX(mode);
    break;

  case 0xBF:
    // Illegal opcode
    break;

  case 0xC0:
    mode = Immediate;
    CPY(mode);
    break;

  case 0xC1:
    mode = IdxIndirect;
    CMP(mode);
    break;

  case 0xC2:
    // Illegal opcode
    break;

  case 0xC3:
    // Illegal opcode
    break;

  case 0xC4:
    mode = Zero;
    CPY(mode);
    break;

  case 0xC5:
    mode = Zero;
    CMP(mode);
    break;

  case 0xC6:
    mode = Zero;
    DEC(mode);
    break;

  case 0xC7:
    // Illegal opcode
    break;

  case 0xC8:
    mode = Implicit;
    INY(mode);
    break;

  case 0xC9:
    mode = Immediate;
    CMP(mode);
    break;

  case 0xCA:
    mode = Implicit;
    DEX(mode);
    break;

  case 0xCB:
    // Illegal opcode
    break;

  case 0xCC:
    mode = Absolute;
    CPY(mode);
    break;

  case 0xCD:
    mode = Absolute;
    CMP(mode);
    break;

  case 0xCE:
    mode = Absolute;
    DEC(mode);
    break;

  case 0xCF:
    // Illegal opcode
    break;

  case 0xD0:
    mode = Relative;
    BNE(mode);
    break;

  case 0xD1:
    mode = IndirectIdx;
    CMP(mode);
    break;

  case 0xD2:
    // Illegal opcode
    break;

  case 0xD3:
    // Illegal opcode
    break;

  case 0xD4:
    // Illegal opcode
    break;

  case 0xD5:
    mode = Zerox;
    CMP(mode);
    break;

  case 0xD6:
    mode = Zerox;
    DEC(mode);
    break;

  case 0xD7:
    // Illegal opcode
    break;

  case 0xD8:
    mode = Implicit;
    CLD(mode);
    break;

  case 0xD9:
    mode = Absolutey;
    CMP(mode);
    break;

  case 0xDA:
    // Illegal opcode
    break;

  case 0xDB:
    // Illegal opcode
    break;

  case 0xDC:
    // Illegal opcode
    break;

  case 0xDD:
    mode = Absolutex;
    CMP(mode);
    break;

  case 0xDE:
    mode = Absolutex;
    DEC(mode);
    break;

  case 0xDF:
    // Illegal opcode
    break;

  case 0xE0:
    mode = Immediate;
    CPX(mode);
    break;

  case 0xE1:
    mode = IdxIndirect;
    SBC(mode);
    break;

  case 0xE2:
    // Illegal opcode
    break;

  case 0xE3:
    // Illegal opcode
    break;

  case 0xE4:
    mode = Zero;
    CPX(mode);
    break;

  case 0xE5:
    mode = Zerox;
    SBC(mode);
    break;

  case 0xE6:
    mode = Zero;
    INC(mode);
    break;

  case 0xE7:
    // Illegal opcode
    break;

  case 0xE8:
    mode = Implicit;
    INX(mode);
    break;

  case 0xE9:
    mode = Immediate;
    SBC(mode);
    break;

  case 0xEA:
    mode = Implicit;
    NOP(mode);
    break;

  case 0xEB:
    // Illegal opcode
    break;

  case 0xEC:
    mode = Absolute;
    CPX(mode);
    break;

  case 0xED:
    mode = Absolute;
    SBC(mode);
    break;

  case 0xEE:
    mode = Absolute;
    INC(mode);
    break;

  case 0xEF:
    // Illegal opcode
    break;

  case 0xF0:
    mode = Relative;
    BEQ(mode);
    break;

  case 0xF1:
    mode = IndirectIdx;
    SBC(mode);
    break;

  case 0xF2:
    // Illegal opcode
    break;

  case 0xF3:
    // Illegal opcode
    break;

  case 0xF4:
    // Illegal opcode
    break;

  case 0xF5:
    mode = Zerox;
    SBC(mode);
    break;

  case 0xF6:
    mode = Zerox;
    INC(mode);
    break;

  case 0xF7:
    // Illegal opcode
    break;

  case 0xF8:
    mode = Implicit;
    SED(mode);
    break;

  case 0xF9:
    mode = Absolutey;
    SBC(mode);
    break;

  case 0xFA:
    // Illegal opcode
    break;

  case 0xFB:
    // Illegal opcode
    break;

  case 0xFC:
    // Illegal opcode
    break;

  case 0xFD:
    mode = Absolutex;
    SBC(mode);
    break;

  case 0xFE:
    mode = Absolutex;
    INC(mode);
    break;

  case 0xFF:
    // Illegal opcode
    break;

  default:
    break;
  }
}

void Chip::ADC(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  uint16_t sum = A + val + (S & 0b000000001);
  setCarry(sum > 0xFF);
  uint8_t res = sum;
  setOverflow((val ^ res) & (res ^ A) & 0x80 != 0);
  A = res;
  setNegative(A & 0x80);
  setZero(A == 0);
}

void Chip::AND(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  A &= val;
  setNegative(A & 0x80);
  setZero(A == 0);
}

void Chip::ASL(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  setCarry(val & 0x80);
  A <<= val;
  setZero(A == 0);
  setNegative(A & 0x80);
}

void Chip::BCC(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  if (!(S & 0b00000001)) {
    pc += val;
  }
}

void Chip::BCS(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  if (S & 0b00000001) {
    pc += val;
  }
}

void Chip::BEQ(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  if (S & 0b00000010) {
    pc += val;
  }
}

void Chip::BIT(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  uint8_t res = A & val;

  setZero(res == 0);
  setOverflow(res & 0b01000000);
  setNegative(res & 0b10000000);
}

void Chip::BMI(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  if (S & 0b10000000) {
    pc += val;
  }
}

void Chip::BNE(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  if (~S & 0b00000010) {
    pc += val;
  }
}

void Chip::BPL(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  if (~S & 0b10000000) {
    pc += val;
  }
}

void Chip::BRK(addressing mode) {
  ++pc;
  --sp;
  Bus.Write_16((0x0100 + sp), pc);
  uint8_t S_cp = S & 0b00110000;
  --sp;
  Bus.Write((0x0100 + sp), S_cp);
  pc = Bus.Read_16(0xFFFE);
}

void Chip::BVC(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  if (~S & 0b01000000) {
    pc += val;
  }
}

void Chip::BVS(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  if (S & 0b01000000) {
    pc += val;
  }
}

void Chip::CLC(addressing mode) { setCarry(false); }

void Chip::CLD(addressing mode) { setDecimal(false); }

void Chip::CLI(addressing mode) { setInterruptDisable(false); }

void Chip::CLV(addressing mode) { setOverflow(false); }

void Chip::CMP(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);
}

void Chip::CPX(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);
}

void Chip::CPY(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);
}

void Chip::DEC(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);
  uint8_t res = val - 1;

  Bus.Write(addr, (res));
  setZero(res == 0);
  setNegative(res & 0b10000000);
}

void Chip::DEX(addressing mode) {
  X -= 1;
  setZero(X == 0);
  setNegative(X & 0b10000000);
}

void Chip::DEY(addressing mode) {
  Y -= 1;
  setZero(Y == 0);
  setNegative(Y & 0b10000000);
}

void Chip::EOR(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  A ^= val;
  setZero(A == 0);
  setNegative(A & 0b10000000);
}

void Chip::INC(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);
  uint8_t res = val + 1;

  Bus.Write(addr, res);
  setZero(res == 0);
  setNegative(res & 0b10000000);
}

void Chip::INX(addressing mode) {
  X += 1;
  setZero(X == 0);
  setNegative(X & 0b10000000);
}

void Chip::INY(addressing mode) {
  Y += 1;
  setZero(Y == 0);
  setNegative(Y & 0b10000000);
}

void Chip::JMP(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint16_t val;
  if (mode == Indirect) {
    if ((addr & 0x00FF) == 0x00FF) {
      val = (Bus.Read(addr & 0xFF00) << 8) | Bus.Read(addr);
    } else {
      val = Bus.Read_16(addr);
    }
  } else if (mode == Absolute) {
    val = Bus.Read_16(addr);
  }

  pc = val;
}

void Chip::JSR(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint16_t val = Bus.Read_16(addr);

  --sp;
  Bus.Write_16(0x0100 + sp, pc);

  pc = val;
}

void Chip::LDA(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  A = val;

  setZero(A == 0);
  setNegative(A & 0b10000000);
}

void Chip::LDX(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  X = val;

  setZero(X == 0);
  setNegative(X & 0b10000000);
}

void Chip::LDY(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  Y = val;

  setZero(Y == 0);
  setNegative(Y & 0b10000000);
}

void Chip::LSR(addressing mode) {
  uint8_t val;
  if (mode == Accumulator) {
    val = A;
    A = val >> 1;

    setCarry(val & 1);
    setZero(A == 0);
    setNegative(A & 0b10000000);
  } else {
    uint16_t addr = get_addr(mode);
    val = Bus.Read(addr);
    uint8_t res = val >> 1;

    Bus.Write(addr, res);

    setCarry(val & 1);
    setZero(res == 0);
    setNegative(res & 0b10000000);
  }
}

void Chip::NOP(addressing mode) {}

void Chip::ORA(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);

  A |= val;

  setZero(A == 0);
  setNegative(A & 0b10000000);
}

void Chip::PHA(addressing mode) {
  uint8_t A_cp = A;

  --sp;
  Bus.Write((0x0100 + sp), A_cp);
}

void Chip::PHP(addressing mode) {
  uint8_t S_cp = S;

  --sp;
  Bus.Write((0x0100 + sp), S_cp);
}

void Chip::PLA(addressing mode) {
  A = Bus.Read((0x0100 + sp));
  ++sp;

  setZero(A == 0);
  setNegative(A & 0b10000000);
}

void Chip::PLP(addressing mode) {
  S = Bus.Read((0x0100 + sp));
  ++sp;
}

void Chip::ROL(addressing mode) {
  uint8_t val;
  if (mode == Accumulator) {
    val = A;
    A = (val << 1) + (S & 1);

    setCarry(val & 0b10000000);
    setZero(A == 0);
    setNegative(A & 0b10000000);
  } else {
    uint16_t addr = get_addr(mode);
    val = Bus.Read(addr);
    uint8_t res = (val << 1) + (S & 1);

    Bus.Write(addr, res);

    setCarry(val & 0b10000000);
    setZero(res == 0);
    setNegative(res & 0b10000000);
  }
}

void Chip::ROR(addressing mode) {
  uint8_t val;
  if (mode == Accumulator) {
    val = A;
    A = (val >> 1) + ((S & 1) << 7);

    setCarry(val & 1);
    setZero(A == 0);
    setNegative(A & 0b10000000);
  } else {
    uint16_t addr = get_addr(mode);
    val = Bus.Read(addr);
    uint8_t res = (val >> 1) + ((S & 1) << 7);

    Bus.Write(addr, res);

    setCarry(val & 1);
    setZero(res == 0);
    setNegative(res & 0b10000000);
  }
}

void Chip::RTI(addressing mode) {
  S = Bus.Read(0x0100 + sp);
  ++sp;
  pc = Bus.Read_16(0x0100 + sp);
  ++sp;
}

void Chip::RTS(addressing mode) {
  pc = Bus.Read_16(0x0100 + sp);
  ++sp;
}

void Chip::SBC(addressing mode) {
  uint16_t addr = get_addr(mode);
  uint8_t val = Bus.Read(addr);
  val = ~val - 1;

  int sum = A + val + (S & 0b000000001);
  setCarry(sum > 0xFF);
  uint8_t res = sum;
  setOverflow((val ^ res) & (res ^ A) & 0x80 != 0);
  A = res;
  setNegative(A & 0x80);
  setZero(A == 0);
}

void Chip::SEC(addressing mode) { setCarry(true); }

void Chip::SED(addressing mode) { setDecimal(true); }

void Chip::SEI(addressing mode) { setInterruptDisable(true); }

void Chip::STA(addressing mode) {
  uint16_t addr = get_addr(mode);

  Bus.Write(addr, A);
}

void Chip::STX(addressing mode) {
  uint16_t addr = get_addr(mode);

  Bus.Write(addr, X);
}

void Chip::STY(addressing mode) {
  uint16_t addr = get_addr(mode);

  Bus.Write(addr, Y);
}

void Chip::TAX(addressing mode) {
  X = A;

  setZero(X == 0);
  setNegative(X & 0b10000000);
}

void Chip::TAY(addressing mode) {
  Y = A;

  setZero(Y == 0);
  setNegative(Y & 0b10000000);
}

void Chip::TSX(addressing mode) {
  X = S;

  setZero(X == 0);
  setNegative(X & 0b10000000);
}

void Chip::TXA(addressing mode) {
  A = X;

  setZero(A == 0);
  setNegative(A & 0b10000000);
}

void Chip::TXS(addressing mode) {
  S = X;
}

void Chip::TYA(addressing mode) {
  A = Y;

  setZero(A == 0);
  setNegative(A & 0b10000000);
}

uint16_t Chip::get_addr(addressing mode) {
  switch (mode) {
  case Immediate:
    return pc;
    break;
  case Zero:
    return Bus.Read(pc);
    break;
  case Zerox:
    return (X + Bus.Read(pc)) % (0xFFFF + 1);
    break;
  case Zeroy:
    return (Y + Bus.Read(pc)) % (0xFFFF + 1);
    break;
  case Absolute:
    return Bus.Read_16(pc);
    break;
  case Absolutex:
    return (Bus.Read_16(pc) + X) % (0xFFFF + 1);
    break;
  case Absolutey:
    return (Bus.Read_16(pc) + Y) % (0xFFFF + 1);
    break;
  case IdxIndirect: {
    uint8_t base = Bus.Read(pc);
    uint8_t ptr = (base + X) % (0xFF + 1);
    return (Bus.Read(base + 1) << 8) | Bus.Read(base);
    break;
  }
  case IndirectIdx: {
    uint8_t base = Bus.Read(pc);
    return (Y +
            ((Bus.Read((base + 1) % (0xFF + 1)) << 8) | Bus.Read(base)) %
                (0xFF + 1));
    break;
  }
  default:
    return -1;
    break;
  }
}
