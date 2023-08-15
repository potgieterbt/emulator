#ifndef CHIP8_CHIP8EMULATOR_H
#define CHIP8_CHIP8EMULATOR_H

#include <cstdint>
#include <string>
#include <vector>

class chip8 {
public:
  chip8();
  ~chip8();
  void loadGame(std::string game);
  bool emulateCycle();
  uint8_t gfx[64 * 32];
  uint8_t key[16];
  bool draw_flag;

private:
  void init();
  unsigned int output;
  uint16_t opcode, I, pc, sp;
  uint8_t memory[4096];
  uint8_t V[16];
  unsigned char delay_timer, sound_timer;
  uint16_t stack[16];
  unsigned int n;
};
#endif // CHIP8_CHIP8EMULATOR_H
