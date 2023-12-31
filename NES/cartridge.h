#include "mapper.h"
#include <cstdint>
#include <string>
#include <vector>

class Cart {
public:
  Cart();
  ~Cart();
  bool loadFromFile(std::string path);
  const std::vector<uint8_t> &getROM();
  const std::vector<uint8_t> &getVROM();
  Mapper *getMapper();
  uint8_t getNameTableMirroring();
  bool hasExtendedRAM();

private:
  std::vector<uint8_t> PRG_ROM;
  std::vector<uint8_t> CHR_ROM;
  uint8_t nameTableMirroring;
  Mapper mapper;
  bool extendedRAM;
  bool chrRAM;
};
