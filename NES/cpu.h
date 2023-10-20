// Stack pointer indexes into 256-byte stack at 0x0100-0x01FF
#include "bus.h"
#include "controller.h"
#include "ppu.h"
#include <cstdint>

class Chip {
public:
  Chip(Mapper *, PPU *, controller *);
  ~Chip();
  void emulateCycle();
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

private:
  bus Bus;
  uint16_t opcode, I;
  uint8_t A, X, Y;
  uint16_t pc;
  uint8_t sp = 0xFE;
  uint8_t S;

  addressing mode;

  uint16_t get_addr(addressing);
  void setNegative(bool val);
  void setOverflow(bool val);
  void setBreak4(bool val);
  void setBreak5(bool val);
  void setDecimal(bool val);
  void setInterruptDisable(bool val);
  void setZero(bool val);
  void setCarry(bool val);

  // Opcodes:
  void ADC(addressing);
  void AND(addressing);
  void ASL(addressing);
  void BCC(addressing);
  void BCS(addressing);
  void BEQ(addressing);
  void BIT(addressing);
  void BMI(addressing);
  void BNE(addressing);
  void BPL(addressing);
  void BRK(addressing);
  void BVC(addressing);
  void BVS(addressing);
  void CLC(addressing);
  void CLD(addressing);
  void CLI(addressing);
  void CLV(addressing);
  void CMP(addressing);
  void CPX(addressing);
  void CPY(addressing);
  void DEC(addressing);
  void DEX(addressing);
  void DEY(addressing);
  void EOR(addressing);
  void INC(addressing);
  void INX(addressing);
  void INY(addressing);
  void JMP(addressing);
  void JSR(addressing);
  void LDA(addressing);
  void LDX(addressing);
  void LDY(addressing);
  void LSR(addressing);
  void NOP(addressing);
  void ORA(addressing);
  void PHA(addressing);
  void PHP(addressing);
  void PLA(addressing);
  void PLP(addressing);
  void ROL(addressing);
  void ROR(addressing);
  void RTI(addressing);
  void RTS(addressing);
  void SBC(addressing);
  void SEC(addressing);
  void SED(addressing);
  void SEI(addressing);
  void STA(addressing);
  void STX(addressing);
  void STY(addressing);
  void TAX(addressing);
  void TAY(addressing);
  void TSX(addressing);
  void TXA(addressing);
  void TXS(addressing);
  void TYA(addressing);
};
