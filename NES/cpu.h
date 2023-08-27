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
};
