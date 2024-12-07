#pragma once
#include <cstdint>
#include <string>
#include <vector>

class ROM {
  std::vector<uint8_t> m_CHR_ROM;
  std::vector<uint8_t> m_PRG_ROM;

private:
  uint8_t m_nameTableMirroring;
  uint8_t m_mapperNumber;
  bool m_extendedRAM;
  bool m_chrRAM;
  uint8_t m_banks;
  uint8_t m_vbanks;

public:
  bool loadROM(const std::string path);
  std::vector<uint8_t> getCHR();
  std::vector<uint8_t> getPRG();
  uint8_t getPRGBanks();
  uint8_t getCHRBanks();
  uint8_t getMapperNumber();
  uint8_t getMirroring();
};
