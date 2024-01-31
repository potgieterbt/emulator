#include "mapper.h"
#include <cstdint>
#include <string>
#include <vector>

class Cart {
public:
  Cart();
  bool loadFromFile(std::string path);
  const std::vector<uint8_t> &getROM();
  const std::vector<uint8_t> &getVROM();
  uint8_t getMapper();
  uint8_t getNameTableMirroring();
  bool hasExtendedRAM();
  Mapper mapper;

private:
  std::vector<uint8_t> PRG_ROM;
  std::vector<uint8_t> CHR_ROM;
  uint8_t nameTableMirroring;
  bool extendedRAM;
  bool chrRAM;
};
