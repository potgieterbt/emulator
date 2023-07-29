
const int prog_start = 0x200;
const int ETI_start = 0x600;
const int ram_end = 0xFFF;
unsigned short opcode, I, PC, sp;
unsigned char memory[4096];
unsigned char V[16];
unsigned char gfx[64 * 32];
unsigned char delay_timer, sound_timer;
unsigned short stack[16];
unsigned char key[16];
