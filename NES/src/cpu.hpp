#include "ppu.hpp"
#include <array>
#include <cstdint>
#include <vector>

class cpu {
public:
  cpu(const std::vector<uint8_t> &, const ppu &);
  void reset();
  void runCycle();

private:
  uint8_t read(uint16_t addr);
  void write(uint16_t addr, uint8_t val);
  uint8_t fetchInstruction();
  void setFlag(uint8_t flag, bool val);
  void executeOpcode(uint8_t op);
  uint8_t pageCross(uint16_t oldPC, uint16_t newPC);

  uint8_t Stack[0xFF];
  std::array<uint8_t, 0xFFFF> RAM;
  std::vector<uint8_t> m_PRG_ROM;

  ppu m_ppu;
  bool Step = false;
  uint16_t last_jump;

  uint8_t cycles;
  uint8_t S;
  uint16_t PC;
  uint8_t A;
  uint8_t X;
  uint8_t Y;
  uint8_t P;

  enum addressing {
    absolute,
    absoluteX,
    absoluteY,
    accumulator,
    immediate,
    implied,
    indirect,
    indirectY,
    relative,
    zero,
    zeroX,
  };

  // Instructions
  void ADC(addressing mode);
  void AND(addressing mode);
  void ASL(addressing mode);
  void BCC(addressing mode);
  void BCS(addressing mode);
  void BEQ(addressing mode);
  void BIT(addressing mode);
  void BNE(addressing mode);
  void BMI(addressing mode);
  void BPL(addressing mode);
  void BVS(addressing mode);
  void CLD(addressing mode);
  void CLC(addressing mode);
  void CLI(addressing mode);
  void CMP(addressing mode);
  void CPX(addressing mode);
  void CPY(addressing mode);
  void DEX(addressing mode);
  void DEY(addressing mode);
  void EOR(addressing mode);
  void INC(addressing mode);
  void INX(addressing mode);
  void INY(addressing mode);
  void JMP(addressing mode);
  void JSR(addressing mode);
  void LDA(addressing mode);
  void LDX(addressing mode);
  void LDY(addressing mode);
  void LSR(addressing mode);
  void ORA(addressing mode);
  void PHA(addressing mode);
  void PHP(addressing mode);
  void PLA(addressing mode);
  void PLP(addressing mode);
  void ROL(addressing mode);
  void ROR(addressing mode);
  void RTS(addressing mode);
  void SBC(addressing mode);
  void SEC(addressing mode);
  void SED(addressing mode);
  void SEI(addressing mode);
  void STA(addressing mode);
  void STX(addressing mode);
  void STY(addressing mode);
  void TAX(addressing mode);
  void TAY(addressing mode);
  void TSX(addressing mode);
  void TXS(addressing mode);
  void TYA(addressing mode);
};
