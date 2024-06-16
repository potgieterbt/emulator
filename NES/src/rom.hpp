#include <cstdint>
#include <string>
#include <vector>

class ROM {
  std::vector<uint8_t> m_CHR_ROM;
  std::vector<uint8_t> m_PRG_ROM;

  uint8_t m_nameTableMirroring;
  uint8_t m_mapperNumber;
  bool m_extendedRAM;
  bool m_chrRAM;

public:
  bool loadROM(const std::string path);
  std::vector<uint8_t> getCHR();
  std::vector<uint8_t> getPRG();
};
