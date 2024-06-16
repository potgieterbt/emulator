#include "ppu.hpp"
#include <cstdint>
#include <vector>

ppu::ppu(const std::vector<uint8_t>& chr) : m_CHR_ROM(chr){};


void ppu::tick() {
  if (scanLine >= 0 & scanLine <= 239 || scanLine == 261) {
  }
  return;
}
