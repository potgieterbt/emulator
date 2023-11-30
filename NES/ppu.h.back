#pragma once

#include "mapper.h"
#include <cstdint>

class PPU {
public:
  PPU(Mapper *mapper);
  uint8_t vram[2048];
  uint8_t oam_data[256];
  uint32_t buffer[256*240];

  uint8_t ppuctrl;
  uint8_t ppumask;
  uint8_t ppustatus;
  uint8_t ppustatus_cpy;
  uint8_t oamaddr;
  uint8_t oamdata;
  uint8_t ppuscroll;
  uint8_t ppu_read_buffer;
  uint8_t ppu_read_buffer_cpy;

  uint8_t read();
  void write();

  uint8_t ppu_read(uint16_t addr);
  void ppu_write(uint16_t addr, uint8_t val);

  uint32_t palette[64] = {
        4283716692, 4278197876, 4278718608, 4281335944, 4282646628, 4284219440, 4283696128, 4282128384,
        4280297984, 4278729216, 4278206464, 4278205440, 4278202940, 4278190080, 4278190080, 4278190080,
        4288190104, 4278734020, 4281348844, 4284227300, 4287108272, 4288681060, 4288160288, 4286069760,
        4283718144, 4280840704, 4278746112, 4278220328, 4278216312, 4278190080, 4278190080, 4278190080,
        4293717740, 4283210476, 4286086380, 4289749740, 4293154028, 4293679284, 4293683812, 4292118560,
        4288719360, 4285842432, 4283224096, 4281912428, 4281906380, 4282137660, 4278190080, 4278190080,
        4293717740, 4289252588, 4290559212, 4292129516, 4293701356, 4293701332, 4293702832, 4293182608,
        4291613304, 4290043512, 4289258128, 4288209588, 4288730852, 4288717472, 4278190080, 4278190080};

  uint8_t bg_palette[16] = {0};
  uint16_t v = 0, t = 0;
  uint8_t x = 0;
  int w = 0;

enum NameTableMirroring {
  Horizontal = 0,
  Vertical = 1,
  FourScreen = 8,
  OneScreenLower,
  OneScreenHigher,
};

  NameTableMirroring mirroring;
private:
};
