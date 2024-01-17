#pragma once

#include <cstdint>
#include <vector>

class Mapper {
public:
  Mapper(uint8_t type, std::vector<uint8_t> &prgCode,
         std::vector<uint8_t> &chrROM, int mirroring);
  const std::vector<uint8_t> &getChr();
  const std::vector<uint8_t> &getPrg();

private:
  std::vector<uint8_t> _prgCode;
};
