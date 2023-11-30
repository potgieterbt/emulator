// Stack pointer indexes into 256-byte stack at 0x0100-0x01FF
#include "controller.h"
#include "ppu.h"
#include <cstdint>

class Bus;

class Chip {
public:
  Chip();
  ~Chip();
  uint8_t readMem(uint16_t addr);
  void writeMem(uint16_t addr, uint8_t val);
  uint16_t readMem_16(uint16_t addr);
  void writeMem_16(uint16_t addr, uint16_t val);
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

  uint16_t opcode;
  uint8_t A, X, Y;
  uint16_t pc;
  uint8_t sp = 0xFE;
  uint8_t S;

  void reset();
  void irq();
  void nmi();
  void clock();

  bool complete();

  void conBus(Bus *n) { bus = n; }

  enum flags {
    C = (1 << 0),
    Z = (1 << 1),
    I = (1 << 2),
    D = (1 << 3),
    B = (1 << 4),
    U = (1 << 5),
    V = (1 << 6),
    N = (1 << 7),
  };

  addressing mode;

  uint8_t GetFlag(flags f);
  void SetFlag(flags f, bool v);

  uint16_t get_addr(addressing);

  uint8_t fetched;
  uint16_t temp;
  uint16_t addr_abs;
  uint16_t addr_rel;
  uint8_t cycles;
  uint32_t clock_count;

  //  void setNegative(bool val);
  //  void setOverflow(bool val);
  //  void setBreak4(bool val);
  //  void setBreak5(bool val);
  //  void setDecimal(bool val);
  //  void setInterruptDisable(bool val);
  //  void setZero(bool val);
  //  void setCarry(bool val);

  Bus *bus = nullptr;

  uint8_t read(uint16_t addr);
  void write(uint16_t addr, uint8_t val);

  uint8_t fetch();

private:
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
