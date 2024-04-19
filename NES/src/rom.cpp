#include "rom.hpp"
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <vector>

class ROM {
  ROM(const std::string path);

  // Variables
  std::vector<uint8_t> PRG_ROM;
  std::vector<uint8_t> CHR_ROM;
  uint8_t mapper_number;

  uint8_t S;
  uint16_t PC;
  uint8_t A;
  uint8_t X;
  uint8_t Y;
  uint8_t P;

  // Functions
  std::vector<uint8_t> loadROM(const std::string path);
  uint8_t GetMapperNumber();
};
