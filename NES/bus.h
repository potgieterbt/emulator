#pragma once

#include <cstdint>

class Bus {
public:
  Bus();
  ~Bus();
  uint8_t Read(const uint16_t);
  void Write(const uint16_t, const uint8_t);
  uint16_t Read_16(const uint16_t);
  void Write_16(const uint16_t, const uint16_t);

private:
  uint8_t cpu_ram[2048];
};
