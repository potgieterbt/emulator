#pragma once

#include <cstdint>
#include "cpu.h"

class Bus {
public:
  Bus();
  ~Bus();

  Chip cpu;

  uint8_t Read(const uint16_t, bool bReadOnly = false);
  void Write(const uint16_t, const uint8_t);

  uint8_t cpu_ram[2048];
};
// uint16_t Read_16(const uint16_t);
// void Write_16(const uint16_t, const uint16_t);
