// Stack pointer indexes into 256-byte stack at 0x0100-0x01FF
#include <cstdint>

class Chip {
public:
  Chip();
  ~Chip();
  void emulateCycle();

private:
  void init();
  uint8_t memory[65536];
  uint16_t opcode, I;
  uint8_t A, X, Y;
  uint16_t pc;
  uint8_t sp;
  uint8_t S;

  enum addressing {
    Implicit,
    Accumulator,
    Immediate,
    Relative,
    Absolute,
    Absolutex,
    Absolutey,
    Zero,
    Zerox,
    Zeroy,
    Indirect,
    IdxIndirect,
    IndirectIdx,
  };
  addressing mode;

  // Opcodes:
  void ADC();
  void AND();
  void ASL();
  void BCC();
  void BCS();
  void BEQ();
  void BIT();
  void BMI();
  void BNE();
  void BPL();
  void BRK();
  void BVC();
  void BVS();
  void CLC();
  void CLD();
  void CLI();
  void CLV();
  void CMP();
  void CPX();
  void CPY();
  void DEC();
  void DEX();
  void DEY();
  void EOR();
  void INC();
  void INX();
  void INY();
  void JMP();
  void JSR();
  void LDA();
  void LDX();
  void LDY();
  void LSR();
  void NOP();
  void ORA();
  void PHA();
  void PHP();
  void PLA();
  void PLP();
  void ROL();
  void ROR();
  void RTI();
  void RTS();
  void SBC();
  void SEC();
  void SED();
  void SEI();
  void STA();
  void STX();
  void STY();
  void TAX();
  void TAY();
  void TSX();
  void TXA();
  void TXS();
  void TYA();
};
