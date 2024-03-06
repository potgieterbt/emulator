#pragma once

#include <cstdint>
class Bus {
public:
  Bus();
  ~Bus();
  uint8_t read(uint16_t addr);
  void write(uint16_t addr, uint8_t val);

private:
  uint8_t ram[2048];
};
