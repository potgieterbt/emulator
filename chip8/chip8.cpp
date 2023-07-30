
#include <cstdio>
#include <string>
#include <vector>

class chip8 {
public:
  void initialize();
  void loadGame(std::string game);
  void emulateCycle();

private:
  const int prog_start = 0x200;
  const int ETI_start = 0x600;
  const int ram_end = 0xFFF;
  unsigned short opcode, I, pc, sp;
  unsigned char memory[4096];
  unsigned char V[16];
  unsigned char gfx[64 * 32];
  unsigned char delay_timer, sound_timer;
  unsigned short stack[16];
  unsigned char key[16];
};

void chip8::initialize() {
  pc = 0x200;
  opcode = 0;
  I = 0;
  sp = 0;
  for (int i = 0; i < 80; ++i) {
    memory[i] = chip8_fontset[i];
  }
}

void chip8::loadGame(std::string game) {
  std::vector<char> buffer;
  int bufferSize = buffer.size();
  for (int i = 0; i < bufferSize; ++i) {
    memory[i + 512] = buffer[i];
  }
}

void chip8::emulateCycle() {
  opcode = memory[pc] << 8 | memory[pc + 1];
  switch (opcode & 0xF000) {

  case 0x0000:
    switch (opcode & 0x000F) {
    case 0x0000:
      printf("clear screen");
      break;
    case 0x000e:
      printf("return from sub");
      break;
    }

  case 0x1000:
    I = opcode & 0x0FFF;
    printf("jump to 0x%X", I);
    pc += 2;
    break;
  case 0x2000:
    I = opcode & 0x0FFF;
    printf("Call sub at 0x%X", I);
    pc += 2;
    break;
  case 0x3000:
    printf("compare value in V%X to %X", (opcode & 0x0F00), (opcode & 0x00FF));
    if ((opcode & 0x0F00) == (opcode & 0x00FF)) {
      pc += 4;
    } else {
      pc += 2;
    }
    break;
  case 0x4000:
    printf("compare value in V%X to %X", (opcode & 0x0F00), (opcode & 0x00FF));
    if ((opcode & 0x0F00) != (opcode & 0x00FF)) {
      pc += 4;
    } else {
      pc += 2;
    }
    break;
  case 0x5000:
    printf("compare value in V%X with value in V%X", (opcode & 0x0F00),
           (opcode & 0x00F0));
    if ((opcode & 0x0F00) == (0x00F0)) {
      pc += 4;
    } else {
      pc += 2;
    }
    break;
  case 0x6000:
    printf("set value of V%X to %X", (opcode & 0x0F00), (opcode & 0x00FF));
    V[opcode & 0x0F00] = (opcode & 0x00FF);
    break;
  case 0x7000:
    printf("Set Vx = Vx + kk");
    V[opcode & 0x0F00] = V[opcode & 0x0F00] + (opcode & 0x00FF);
    break;
  case 0x8000:
    switch (opcode & 0x000F) {
    case 0x0000:
      printf("Set Vx = Vy");
      V[opcode & 0x0F00] = V[opcode & 0x00F0];
      break;
    case 0x0001:
      printf("Set Vx to Vx or Vy");
      V[opcode & 0x0F00] = V[opcode & 0x0F00] | V[opcode & 0x00F0];
      break;
    case 0x0002:
      printf("set Vx to Vx and Vy");
      V[opcode & 0x0F00] = V[opcode & 0x0F00] & V[opcode & 0x00F0];
      break;
    case 0x0003:
      printf("Set Vx to Vx XOR Vy");
      V[opcode & 0x0F00] = V[opcode & 0x0F00] ^ V[opcode & 0x00F0];
      break;
    case 0x0004:
      printf("Set Vx to Vx + Vy, VF = carry");
      V[opcode & 0x0F00] = V[opcode & 0x0F00] + V[opcode & 0x00F0];
      if (V[opcode & 0x0F00] + V[opcode & 0x00F0] > 255) {
        V[0xF] = 1;
      }
      break;
    case 0x0005:
      printf("Set Vx to Vx - Vy, if Vx > Vy set VF to 1, else 0");
      V[opcode & 0x0F00] = V[opcode & 0x0F00] - V[opcode & 0x00F0];
      if (V[opcode & 0x0F00] > V[opcode & 0x00F0]) {
        V[0xF] = 1;
      }
      break;
    case 0x0006:
      break;
    case 0x0007:
      break;
    case 0x000E:
      break;
    }
  case 0x9000:
    break;
  case 0xA000:
    I = opcode & 0x0FFF;
    pc += 2;
    break;
  case 0xB000:
    printf("jump to location 0x%X", (opcode & 0x0FFF));
    I = opcode & 0x0FFF;
    pc = I;
    break;
  case 0xC000:
    //===========================================
    break;
    case :
  default:
    std::printf("Unknown opcode: 0x%X\n", opcode);
    break;
  }

  if (delay_timer > 0)
    --delay_timer;

  if (sound_timer > 0) {
    if (sound_timer == 1)
      std::printf("BEEP!\n");
    --sound_timer;
  }
}
