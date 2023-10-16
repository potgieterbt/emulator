#pragma once

#include <cstdint>
#include <vector>

class Mapper {
public:
  Mapper(std::vector<uint8_t> &prgCode, std::vector<uint8_t> &chrROM,
         int mirroring);
};
