#pragma once

#include "mapper.h"
#include <cstdint>
#include <vector>

class PPU {
public:
  PPU(Mapper *mapper);

private:
  std::vector<uint8_t> chr_rom;
  uint8_t palette_table[32];
  uint8_t vram[2048];
  uint8_t oam_data[256];

  //Mirroring

  // Registers:
  // Controller

  // Mask

  // Status Read-only

  // OAM Address

  // OAM Data

  // Scroll

  // Address

  // Data

  // OAM DMA

};
