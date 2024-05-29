#include <array>
#include <cstdint>
#include <string>
#include <vector>

class cpu {
public:
  std::array<uint8_t, 0xFFFF> RAM;
  std::vector<uint8_t> m_CHR_ROM;
  std::vector<uint8_t> m_PRG_ROM;

  uint8_t read(uint16_t addr);
  void write(uint16_t addr, uint8_t val);
  uint8_t fetchInstruction();
  void reset();
  void runCycle();
  void executeOpcode(uint8_t op);

  bool loadROM(const std::string path);
  std::vector<uint8_t> getCHR();
  std::vector<uint8_t> getPRG();
  uint8_t m_nameTableMirroring;
  uint8_t m_mapperNumber;
  bool m_extendedRAM;
  bool m_chrRAM;
  uint8_t Stack[0xFF];

  uint8_t S;
  uint16_t PC;
  uint8_t A;
  uint8_t X;
  uint8_t Y;
  uint8_t P;

  enum addressing {
    absolute,
    absoluteX,
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
  void BNE(addressing mode);
  void BEQ(addressing mode);
  void BVS(addressing mode);
  void CMP(addressing mode);
  void INC(addressing mode);
  void INX(addressing mode);
  void INY(addressing mode);
  void JMP(addressing mode);
  void JSR(addressing mode);
  void LDA(addressing mode);
  void LDX(addressing mode);
  void LDY(addressing mode);
  void LSR(addressing mode);
  void PHA(addressing mode);
  void PLA(addressing mode);
  void STA(addressing mode);
  void STX(addressing mode);
  void STY(addressing mode);
  void TAX(addressing mode);
};
