#include "ppu.hpp"
#include <cstdint>

void ppu::tick(uint8_t cycles) {
  // Only called for cycle 1+
  if (_cycle <= 256) {
    LoadTileInfo();

    if (_prevRenderingEnabled && (_cycle & 0x07) == 0) {
      IncHorizontalScrolling();
      if (_cycle == 256) {
        IncVerticalScrolling();
      }
    }

    if (_scanline >= 0) {
      ((T *)this)->DrawPixel();
      ShiftTileRegisters();

      //"Secondary OAM clear and sprite evaluation do not occur on the
      //pre-render line"
      ProcessSpriteEvaluation();
    } else if (_cycle < 9) {
      // Pre-render scanline logic
      if (_cycle == 1) {
        _statusFlags.VerticalBlank = false;
        _console->GetCpu()->ClearNmiFlag();
      }
      if (_spriteRamAddr >= 0x08 && IsRenderingEnabled() &&
          !_settings->GetNesConfig().DisableOamAddrBug) {
        // This should only be done if rendering is enabled (otherwise
        // oam_stress test fails immediately) "If OAMADDR is not less than eight
        //when rendering starts, the eight bytes starting at OAMADDR & 0xF8 are
        //copied to the first eight bytes of OAM"
        WriteSpriteRam(_cycle - 1,
                       ReadSpriteRam((_spriteRamAddr & 0xF8) + _cycle - 1));
      }
    }
  } else if (_cycle >= 257 && _cycle <= 320) {
    if (_cycle == 257) {
      _spriteIndex = 0;
      memset(_hasSprite, 0, sizeof(_hasSprite));
      if (_prevRenderingEnabled) {
        // copy horizontal scrolling value from t
        _videoRamAddr = (_videoRamAddr & ~0x041F) | (_tmpVideoRamAddr & 0x041F);
      }
    }
    if (IsRenderingEnabled()) {
      //"OAMADDR is set to 0 during each of ticks 257-320 (the sprite tile
      //loading interval) of the pre-render and visible scanlines." (When
      //rendering)
      _spriteRamAddr = 0;

      switch ((_cycle - 257) % 8) {
      // Garbage NT sprite fetch (257, 265, 273, etc.) - Required for proper
      // MC-ACC IRQs (MMC3 clone)
      case 0:
        ReadVram(GetNameTableAddr());
        break;

      // Garbage AT sprite fetch
      case 2:
        ReadVram(GetAttributeAddr());
        break;

      // Cycle 260, 268, etc.  This is an approximation (each tile is actually
      // loaded in 8 steps (e.g from 257 to 264))
      case 4:
        LoadSpriteTileInfo();
        break;
      }

      if (_scanline == -1 && _cycle >= 280 && _cycle <= 304) {
        // copy vertical scrolling value from t
        _videoRamAddr = (_videoRamAddr & ~0x7BE0) | (_tmpVideoRamAddr & 0x7BE0);
      }
      // Load the extra sprites before tile loading starts (which matters for
      // MMC5) The CHR banks are switched back to the bg tileset in LoadTileInfo
      // on cycle 321 and the extra sprites will potentially read from the wrong
      // banks
      if (_cycle == 320) {
        LoadExtraSprites();
      }
    }
  } else if (_cycle >= 321 && _cycle <= 336) {
    LoadTileInfo();

    if (_cycle == 321) {
      if (IsRenderingEnabled()) {
        _oamCopybuffer = _secondarySpriteRam[0];
      }
    } else if (_prevRenderingEnabled && (_cycle == 328 || _cycle == 336)) {
      _lowBitShift <<= 8;
      _highBitShift <<= 8;
      IncHorizontalScrolling();
    }
  } else if (_cycle == 337 || _cycle == 339) {
    if (IsRenderingEnabled()) {
      _tile.TileAddr = ReadVram(GetNameTableAddr());

      if (_scanline == -1 && _cycle == 339 && (_frameCount & 0x01) &&
          _region == ConsoleRegion::Ntsc &&
          GetPpuModel() == PpuModel::Ppu2C02) {
        // This behavior is NTSC-specific - PAL frames are always the same
        // number of cycles "With rendering enabled, each odd PPU frame is one
        //PPU clock shorter than normal" (skip from 339 to 0, going over 340)
        _cycle = 340;
      }
    }
  }
}

