#include "cpu.h"
#include "memory.h"
#include <cstdint>

void Chip::init() {}

RAM memory;

void Chip::emulateCycle() {

  uint8_t opcode = memory.Read(pc);

  switch (opcode) {
  case 0x00:
    mode = Implicit;
    BRK();
    break;

  case 0x01:
    mode = IdxIndirect;
    ORA();
    break;

  case 0x02:
    // Illegal opcode I think
    break;

  case 0x03:
    // Illegal opcode
    break;

  case 0x04:
    mode = Implicit;
    NOP();
    break;

  case 0x05:
    mode = Zero;
    ORA();
    break;

  case 0x06:
    mode = Zero;
    ASL();
    break;

  case 0x07:
    // Illegal opcode
    break;

  case 0x08:
    mode = Implicit;
    PHP();
    break;

  case 0x09:
    mode = Immediate;
    ORA();
    break;

  case 0x0A:
    mode = Implicit;
    ASL();
    break;

  case 0x0B:
    // Illegal opcode
    break;

  case 0x0C:
    NOP();
    break;

  case 0x0D:
    mode = Absolute;
    ORA();
    break;

  case 0x0E:
    mode = Absolute;
    ASL();
    break;

  case 0x0F:
    // Illegal opcode
    break;

  case 0x10:
    mode = Relative;
    BPL();
    break;

  case 0x11:
    mode = IndirectIdx;
    ORA();
    break;

  case 0x12:
    // Illegal opcode
    break;

  case 0x13:
    // Illegal opcode
    break;

  case 0x14:
    NOP();
    break;

  case 0x15:
    mode = IdxIndirect;
    ORA();
    break;

  case 0x16:
    mode = IdxIndirect;
    ASL();
    break;

  case 0x17:
    // Illegal opcode
    break;

  case 0x18:
    mode = Implicit;
    CLC();
    break;

  case 0x19:
    mode = Absolutey;
    ORA();
    break;

  case 0x1A:
    NOP();
    break;

  case 0x1B:
    // Illegal opcode
    break;

  case 0x1C:
    NOP();
    break;

  case 0x1D:
    mode = Absolutex;
    ORA();
    break;

  case 0x1E:
    mode = Absolutex;
    ASL();
    break;

  case 0x1F:
    // Illegal opcode
    break;

  case 0x20:
    mode = Absolute;
    JSR();
    break;

  case 0x21:
    mode = IndirectIdx;
    AND();
    break;

  case 0x22:
    // Illegal opcode
    break;

  case 0x23:
    // Illegal opcode
    break;

  case 0x24:
    mode = Zero;
    BIT();
    break;

  case 0x25:
    mode = Zero;
    AND();
    break;

  case 0x26:
    mode = Zero;
    ROL();
    break;

  case 0x27:
    // Illegal opcode
    break;

  case 0x28:
    mode = Implicit;
    PLP();
    break;

  case 0x29:
    mode = Immediate;
    AND();
    break;

  case 0x2A:
    mode = Implicit;
    ROL();
    break;

  case 0x2B:
    // Illegal opcode
    break;

  case 0x2C:
    mode = Absolute;
    BIT();
    break;

  case 0x2D:
    mode = Absolute;
    AND();
    break;

  case 0x2E:
    mode = Absolute;
    LSR();
    break;

  case 0x2F:
    // Illegal opcode
    break;

  case 0x30:
    mode = Relative;
    BMI();
    break;

  case 0x31:
    mode = IndirectIdx;
    AND();
    break;

  case 0x32:
    // Illegal opcode
    break;

  case 0x33:
    // Illegal opcode
    break;

  case 0x34:
    mode = Zerox;
    NOP();
    break;

  case 0x35:
    mode = Zerox;
    AND();
    break;

  case 0x36:
    mode = Zerox;
    ROL();
    break;

  case 0x37:
    // Illegal opcode
    break;

  case 0x38:
    mode = Implicit;
    SEC();
    break;

  case 0x39:
    mode = Absolutey;
    AND();
    break;

  case 0x3A:
    NOP();
    break;

  case 0x3B:
    // Illegal opcode
    break;

  case 0x3C:
    mode = Absolutex;
    NOP();
    break;

  case 0x3D:
    mode = Absolutex;
    AND();
    break;

  case 0x3E:
    mode = Absolutex;
    ROL();
    break;

  case 0x3F:
    // Illegal opcode
    break;

  case 0x40:
    mode = Implicit;
    RTI();
    break;

  case 0x41:
    mode = IdxIndirect;
    EOR();
    break;

  case 0x42:
    // Illegal opcode
    break;

  case 0x43:
    // Illegal opcode
    break;

  case 0x44:
    mode = Zero;
    NOP();
    break;

  case 0x45:
    mode = Zero;
    EOR();
    break;

  case 0x46:
    mode = Zero;
    LSR();
    break;

  case 0x47:
    // Illegal opcode
    break;

  case 0x48:
    mode = Implicit;
    PHA();
    break;

  case 0x49:
    mode = Immediate;
    EOR();
    break;

  case 0x4A:
    mode = Accumulator;
    LSR();
    break;

  case 0x4B:
    // Illegal opcode
    break;

  case 0x4C:
    mode = Absolute;
    JMP();
    break;

  case 0x4D:
    mode = Absolute;
    EOR();
    break;

  case 0x4E:
    mode = Absolute;
    LSR();
    break;

  case 0x4F:
    // Illegal opcode
    break;

  case 0x50:
    mode = Relative;
    BVC();
    break;

  case 0x51:
    mode = IndirectIdx;
    EOR();
    break;

  case 0x52:
    // Illegal opcode
    break;

  case 0x53:
    // Illegal opcode
    break;

  case 0x54:
    mode = IdxIndirect;
    NOP();
    break;

  case 0x55:
    mode = IdxIndirect;
    EOR();
    break;

  case 0x56:
    mode = IdxIndirect;
    LSR();
    break;

  case 0x57:
    // Illegal opcode
    break;

  case 0x58:
    mode = Implicit;
    CLI();
    break;

  case 0x59:
    mode = IndirectIdx;
    EOR();
    break;

  case 0x5A:
    mode = Implicit;
    NOP();
    break;

  case 0x5B:
    // Illegal opcode
    break;

  case 0x5C:
    mode = IdxIndirect;
    NOP();
    break;

  case 0x5D:
    mode = IdxIndirect;
    EOR();
    break;

  case 0x5E:
    mode = IdxIndirect;
    LSR();
    break;

  case 0x5F:
    // Illegal opcode
    break;

  case 0x60:
    mode = Implicit;
    RTS();
    break;

  case 0x61:
    mode = IdxIndirect;
    ADC();
    break;

  case 0x62:
  case 0x63:
  case 0x64:
  case 0x65:
    mode = Zero;
    ADC();
    break;

  case 0x66:
    mode = Zero;
    ROR();
    break;

  case 0x67:
  case 0x68:
  case 0x69:
    mode = Immediate;
    ADC();
    break;

  case 0x6A:
    mode = Accumulator;
    ROR();
    break;

  case 0x6B:
  case 0x6C:
  case 0x6D:
    mode = Absolute;
    ADC();
    break;

  case 0x6E:
    mode = Absolute;
    ROR();
    break;

  case 0x6F:
  case 0x70:
    mode = Relative;
    BVS();
    break;

  case 0x71:
    mode = Absolute;
    ADC();
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
    ADC();
    break;

  case 0x76:
    mode = Zerox;
    ROR();
    break;

  case 0x77:
    // Illegal opcode
    break;

  case 0x78:
    mode = Implicit;
    SEI();
    break;

  case 0x79:
    mode = Absolutey;
    ADC();
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
    ADC();
    break;

  case 0x7E:
    mode = Absolutex;
    ROR();
    break;

  case 0x7F:
    // Illegal opcode
    break;

  case 0x80:
    // Illegal opcode
    break;

  case 0x81:
    mode = IdxIndirect;
    STA();
    break;

  case 0x82:
    // Illegal opcode
    break;

  case 0x83:
    // Illegal opcode
    break;

  case 0x84:
    mode = Zero;
    STY();
    break;

  case 0x85:
    mode = Zero;
    STA();
    break;

  case 0x86:
    mode = Zero;
    STX();
    break;

  case 0x87:
    // Illegal opcode
    break;

  case 0x88:
    mode = Implicit;
    DEY();
    break;

  case 0x89:
    // Illegal opcode
    break;

  case 0x8A:
    mode = Implicit;
    TXA();
    break;

  case 0x8B:
    // Illegal opcode
    break;

  case 0x8C:
    mode = Absolute;
    STY();
    break;

  case 0x8D:
    mode = Absolute;
    STA();
    break;

  case 0x8E:
    mode = Absolute;
    STX();
    break;

  case 0x8F:
    // Illegal opcode
    break;

  case 0x90:
    mode = Relative;
    BCC();
    break;

  case 0x91:
    mode = IndirectIdx;
    STA();
    break;

  case 0x92:
    // Illegal opcode
    break;

  case 0x93:
    // Illegal opcode
    break;

  case 0x94:
    mode = Zerox;
    STY();
    break;

  case 0x95:
    mode = Zerox;
    STA();
    break;

  case 0x96:
    mode = Zeroy;
    STX();
    break;

  case 0x97:
    // Illegal opcode
    break;

  case 0x98:
    mode = Implicit;
    TYA();
    break;

  case 0x99:
    mode = Absolutey;
    STA();
    break;

  case 0x9A:
    mode = Implicit;
    TXS();
    break;

  case 0x9B:
    // Illegal opcode
    break;

  case 0x9C:
    // Illegal opcode
    break;

  case 0x9D:
    mode = Absolutex;
    STA();
    break;

  case 0x9E:
    // Illegal opcode
    break;

  case 0x9F:
    // Illegal opcode
    break;

  case 0xA0:
    mode = Immediate;
    LDY();
    break;

  case 0xA1:
    mode = IdxIndirect;
    LDA();
    break;

  case 0xA2:
    mode = Immediate;
    LDX();
    break;

  case 0xA3:
    // Illegal opcode
    break;

  case 0xA4:
    mode = Zero;
    LDY();
    break;

  case 0xA5:
    mode = Zero;
    LDA();
    break;

  case 0xA6:
    mode = Zero;
    LDX();
    break;

  case 0xA7:
    // Illegal opcode
    break;

  case 0xA8:
    mode = Implicit;
    TAY();
    break;

  case 0xA9:
    mode = Immediate;
    LDA();
    break;

  case 0xAA:
    mode = Implicit;
    TAX();
    break;

  case 0xAB:
    // Illegal opcode
    break;

  case 0xAC:
    mode = Absolute;
    LDY();
    break;

  case 0xAD:
    mode = Absolute;
    LDA();
    break;

  case 0xAE:
    mode = Absolute;
    LDX();
    break;

  case 0xAF:
    // Illegal opcode
    break;

  case 0xB0:
    mode = Relative;
    BCS();
    break;

  case 0xB1:
    mode = IndirectIdx;
    LDA();
    break;

  case 0xB2:
    // Illegal opcode
    break;

  case 0xB3:
    // Illegal opcode
    break;

  case 0xB4:
    mode = Zerox;
    LDY();
    break;

  case 0xB5:
    mode = Zerox;
    LDA();
    break;

  case 0xB6:
    mode = Zerox;
    LDX();
    break;

  case 0xB7:
    // Illegal opcode
    break;

  case 0xB8:
    mode = Implicit;
    CLV();
    break;

  case 0xB9:
    mode = Absolutey;
    LDA();
    break;

  case 0xBA:
    mode = Implicit;
    TSX();
    break;

  case 0xBB:
    // Illegal opcode
    break;

  case 0xBC:
    mode = Absolutex;
    LDY();
    break;

  case 0xBD:
    mode = Absolutex;
    LDA();
    break;

  case 0xBE:
    mode = Absolutey;
    LDX();
    break;

  case 0xBF:
    // Illegal opcode
    break;

  case 0xC0:
    mode = Immediate;
    CPY();
    break;

  case 0xC1:
    mode = IdxIndirect;
    CMP();
    break;

  case 0xC2:
    // Illegal opcode
    break;

  case 0xC3:
    // Illegal opcode
    break;

  case 0xC4:
    mode = Zero;
    CPY();
    break;

  case 0xC5:
    mode = Zero;
    CMP();
    break;

  case 0xC6:
    mode = Zero;
    DEC();
    break;

  case 0xC7:
    // Illegal opcode
    break;

  case 0xC8:
    mode = Implicit;
    INY();
    break;

  case 0xC9:
    mode = Immediate;
    CMP();
    break;

  case 0xCA:
    mode = Implicit;
    DEX();
    break;

  case 0xCB:
    // Illegal opcode
    break;

  case 0xCC:
    mode = Absolute;
    CPY();
    break;

  case 0xCD:
    mode = Absolute;
    CMP();
    break;

  case 0xCE:
    mode = Absolute;
    DEC();
    break;

  case 0xCF:
    // Illegal opcode
    break;

  case 0xD0:
    mode = Relative;
    BNE();
    break;

  case 0xD1:
    mode = IndirectIdx;
    CMP();
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
    CMP();
    break;

  case 0xD6:
    mode = Zerox;
    DEC();
    break;

  case 0xD7:
    // Illegal opcode
    break;

  case 0xD8:
    mode = Implicit;
    CLD();
    break;

  case 0xD9:
    mode = Absolutey;
    CMP();
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
    CMP();
    break;

  case 0xDE:
    mode = Absolutex;
    DEC();
    break;

  case 0xDF:
    // Illegal opcode
    break;

  case 0xE0:
    mode = Immediate;
    CPX();
    break;

  case 0xE1:
    mode = IdxIndirect;
    SBC();
    break;

  case 0xE2:
    // Illegal opcode
    break;

  case 0xE3:
    // Illegal opcode
    break;

  case 0xE4:
    mode = Zero;
    CPX();
    break;

  case 0xE5:
    mode = Zerox;
    SBC();
    break;

  case 0xE6:
    mode = Zero;
    INC();
    break;

  case 0xE7:
    // Illegal opcode
    break;

  case 0xE8:
    mode = Implicit;
    INX();
    break;

  case 0xE9:
    mode = Immediate;
    SBC();
    break;

  case 0xEA:
    mode = Implicit;
    NOP();
    break;

  case 0xEB:
    // Illegal opcode
    break;

  case 0xEC:
    mode = Absolute;
    CPX();
    break;

  case 0xED:
    mode = Absolute;
    SBC();
    break;

  case 0xEE:
    mode = Absolute;
    INC();
    break;

  case 0xEF:
    // Illegal opcode
    break;

  case 0xF0:
    mode = Relative;
    BEQ();
    break;

  case 0xF1:
    mode = IndirectIdx;
    SBC();
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
    SBC();
    break;

  case 0xF6:
    mode = Zerox;
    INC();
    break;

  case 0xF7:
    // Illegal opcode
    break;

  case 0xF8:
    mode = Implicit;
    SED();
    break;

  case 0xF9:
    mode = Absolutey;
    SBC();
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
    SBC();
    break;

  case 0xFE:
    mode = Absolutex;
    INC();
    break;

  case 0xFF:
    // Illegal opcode
    break;

  default:
    break;
  }
}

void Chip::ADC(addressing mode) {
  int value;
  uint16_t addr = get_addr(mode);
  switch (mode) {
  case Immediate:
    // set value to the byte after the opcode
    A = A + Read(addr) + (S & 128);
    break;
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  case Absolutey:
    break;
  case IdxIndirect:
    break;
  case IndirectIdx:
    break;
  default:
    break;
  }
}
void Chip::AND() {
  int value;
  switch (mode) {
  case Immediate:
    // set value to the byte after the opcode
    break;
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  case Absolutey:
    break;
  case IdxIndirect:
    break;
  case IndirectIdx:
    break;
  default:
    break;
  }
}

void Chip::ASL() {
  int value;

  switch (mode) {
  case Accumulator:
    // set value to the byte after the opcode
    break;
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  default:
    break;
  }
}
void Chip::BCC() {
  int value;
  switch (mode) {
  case Relative:
    break;
  default:
    break;
  }
}
void Chip::BCS() {
  int value;
  switch (mode) {
  case Relative:
    break;
  default:
    break;
  }
}

void Chip::BEQ() {
  int value;
  switch (mode) {
  case Zero:
    break;
  case Absolute:
    break;
  default:
    break;
  }
}
void Chip::BIT() {
  int value;
  switch (mode) {
  case Relative:
    break;
  default:
    break;
  }
}
void Chip::BMI() {
  int value;
  switch (mode) {
  case Relative:
    break;
  default:
    break;
  }
}
void Chip::BNE() {
  int value;
  switch (mode) {
  case Relative:
    break;
  default:
    break;
  }
}
void Chip::BPL() {
  int value;
  switch (mode) {
  case Relative:
    break;
  default:
    break;
  }
}
void Chip::BRK() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::BVC() {
  int value;
  switch (mode) {
  case Relative:
    break;
  default:
    break;
  }
}
void Chip::BVS() {
  int value;
  switch (mode) {
  case Relative:
    break;
  default:
    break;
  }
}
void Chip::CLC() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}

void Chip::CLD() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::CLI() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::CLV() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::CMP() {
  int value;
  switch (mode) {
  case Immediate:
    break;
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  case Absolutey:
    break;
  case IdxIndirect:
    break;
  case IndirectIdx:
    break;
  default:
    break;
  }
}
void Chip::CPX() {
  int value;
  switch (mode) {
  case Immediate:
    break;
  case Zero:
    break;
  case Absolute:
    break;
  default:
    break;
  }
}
void Chip::CPY() {
  int value;
  switch (mode) {
  case Immediate:
    break;
  case Zero:
    break;
  case Absolute:
    break;
  default:
    break;
  }
}
void Chip::DEC() {
  int value;
  switch (mode) {
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  default:
    break;
  }
}
void Chip::DEX() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::DEY() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::EOR() {
  int value;
  switch (mode) {
  case Immediate:
    break;
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  case Absolutey:
    break;
  case IdxIndirect:
    break;
  case IndirectIdx:
    break;
  default:
    break;
  }
}
void Chip::INC() {
  int value;
  switch (mode) {
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  default:
    break;
  }
}
void Chip::INX() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::INY() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::JMP() {
  int value;
  switch (mode) {
  case Absolute:
    break;
  case Indirect:
    break;
  default:
    break;
  }
}
void Chip::JSR() {
  int value;
  switch (mode) {
  case Absolute:
    break;
  default:
    break;
  }
}
void Chip::LDA() {
  int value;
  switch (mode) {
  case Immediate:
    break;
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  case Absolutey:
    break;
  case IdxIndirect:
    break;
  case IndirectIdx:
    break;
  default:
    break;
  }
}
void Chip::LDX() {
  int value;
  switch (mode) {
  case Immediate:
    break;
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  default:
    break;
  }
}
void Chip::LDY() {
  int value;
  switch (mode) {
  case Immediate:
    break;
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  default:
    break;
  }
}
void Chip::LSR() {
  int value;
  switch (mode) {
  case Accumulator:
    break;
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  default:
    break;
  }
}
void Chip::NOP() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::ORA() {
  int value;
  switch (mode) {
  case Immediate:
    break;
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  case Absolutey:
    break;
  case IdxIndirect:
    break;
  case IndirectIdx:
    break;
  default:
    break;
  }
}
void Chip::PHA() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::PHP() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::PLA() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::PLP() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::ROL() {
  int value;
  switch (mode) {
  case Accumulator:
    break;
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  default:
    break;
  }
}
void Chip::ROR() {
  int value;
  switch (mode) {
  case Accumulator:
    break;
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  default:
    break;
  }
}
void Chip::RTI() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::RTS() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::SBC() {
  int value;
  switch (mode) {
  case Immediate:
    break;
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  case IdxIndirect:
    break;
  case IndirectIdx:
    break;
  default:
    break;
  }
}
void Chip::SEC() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::SED() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::SEI() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::STA() {
  int value;
  switch (mode) {
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  case Absolutex:
    break;
  case IdxIndirect:
    break;
  case IndirectIdx:
    break;
  default:
    break;
  }
}
void Chip::STX() {
  int value;
  switch (mode) {
  case Zero:
    break;
  case Zeroy:
    break;
  case Absolute:
    break;
  default:
    break;
  }
}
void Chip::STY() {
  int value;
  switch (mode) {
  case Zero:
    break;
  case Zerox:
    break;
  case Absolute:
    break;
  default:
    break;
  }
}
void Chip::TAX() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::TAY() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::TSX() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::TXA() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::TXS() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}
void Chip::TYA() {
  int value;
  switch (mode) {
  case Implicit:
    break;
  default:
    break;
  }
}

uint16_t Chip::get_addr(addressing mode) {
  switch (mode) {
  case Immediate:
    return pc;
    break;
  case Zero:
    return memory.Read(pc);
    break;
  case Zerox:
    return (X + memory.Read(pc) % 256);
    break;
  case Zeroy:
    return (Y + memory.Read(pc) % 256);
    break;
  case Absolute:
    return memory.Read_16(pc);
    break;
  case Absolutex:
    return memory.Read_16(pc);
    break;
  case Absolutey:
    return memory.Read_16(pc);
    break;
  case IdxIndirect:
    return (Y + memory.Read(pc) % 256);
    break;
  case IndirectIdx:
    return (Y + memory.Read(pc) % 256);
    break;
  default:
    break;
  }
}
