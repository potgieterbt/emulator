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
  uint8_t write(uint16_t addr, uint8_t val);
  void reset();

  bool loadROM(const std::string path);
  std::vector<uint8_t> getCHR();
  std::vector<uint8_t> getPRG();
  uint8_t m_nameTableMirroring;
  uint8_t m_mapperNumber;
  bool m_extendedRAM;
  bool m_chrRAM;

  uint8_t S;
  uint16_t PC;
  uint8_t A;
  uint8_t X;
  uint8_t Y;
  uint8_t P;
};