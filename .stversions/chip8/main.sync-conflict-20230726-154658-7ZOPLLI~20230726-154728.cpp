
#include <iostream>
#include <vector>
const int prog_start = 0x200;
const int ETI_start = 0x600;
const int ram_end = 0xFFF;
const int max_sub = 16;
const char keys[16] = {'1', '2', '3', 'C', '4', '5', '6', 'D',
                       '7', '8', '9', 'E', 'A', '0', 'B', 'F'};

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

int main(int argc, char *argv[]) {
  int x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, xA, xB, xC, xD, xE, xF, I, VF, PC,
      SP;
  int Stack[16];
  return 0;
}

int handle_instruction(int inst) {
  switch (inst) {
  case 0x00E0:
    std::cout << "Clear Display"
              << "\n";
    break;
  case 0x00EE:
    std::cout << "Return subroutine"
              << "\n";
    break;

  default:

    break;
  }
  return 0;
}
