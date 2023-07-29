
#include <iostream>
#include <vector>
#include "chip8.cpp"
#include <SDL2/SDL.h>

// Display is 64x32-pixel, 0-63 across, 0-31 down.
// Uses sprites, group of bytes, upto 15 bytes for a possible sprite size of
// 8x15.

// Timers:
// Delay, active whenever delay timer register (DT) is non zero. Timer just
// subrtacts 1 from val in DT, when DT reaches 0 timer deactivates. Sound,
// avtive when sound timer register (ST) is non-zero. Decriments at 60Hz,
// however as long as ST's value is greater than 0, buzzer sounds, when reaches
// 0, buzzer deactivates.

// Original has 36 instructions, chip-48 has 46.
// each istruction is 2 bytes long and stored most significant byte first.
// First byte of each instruction should be located at an even address in
// memory. If program includes sprite data, should be padded so any instructions
// following will properly be situated in ram. In instructions: nnn or addr - 12
// bit value, lowest 12 bits of instruction. n or nibble - 4-bit value, lowest 4
// bits of instruction. x - 4-bit value, lower 4 bits of high byte of
// instruction y - 4-bit value, upper 4 bits of low byte of instruction kk or
// byte - 8-bit value, lower 8 bits of instruction.

// Basic loop: read PC, get instruction at address in PC, Fetch data in
// instruction, execute instruction, incriment PC, repeat

int main(int argc, char *argv[]) {
  setupGraphics();
  setupInput();
  chip8 myChip8;
  myChip8.initialize();
  myChip8.loadGame("pong");


  for (;;){
    myChip8.emulateCycle();

    if (myChip8.drawFlag)
      drawGraphics();

    myChip8.setKeys();
  }
  return 0;
}
