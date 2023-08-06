#include <cstdint>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

class chip8 {
public:
  void initialize();
  void loadGame(std::string game);
  bool emulateCycle();

private:
  unsigned int output;
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
  unsigned int n;
  std::vector<int> sprite;
  int loc[2] = {0, 0};
  bool p_erase;
  const unsigned char chip8_fontset[80] = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };
};

void chip8::initialize() {
  pc = 0x200;
  opcode = 0;
  I = 0;
  sp = 0;
  for (int i = 0; i < 4096; ++i) {
    memory[i] = 0;
  }
  for (int i = 0; i < 80; ++i) {
    memory[i] = chip8_fontset[i];
  }
}

void chip8::loadGame(std::string rom_path) {
  rom_path = "./PONG";
  std::vector<char> buffer;
  std::ifstream gamefile(rom_path, std::ios::binary | std::ios::in);

  char c;

  for (int i = 0x200; gamefile.get(c) && i < 4096; ++i) {
    memory[i] = (uint8_t)c;
  }
}

bool chip8::emulateCycle() {
  opcode = (memory[pc]) << 8 | memory[pc + 1];
  printf("\n%X\n", opcode);
  printf("%X, %X, %X\n", pc, I, (memory[pc]) << 8 | memory[pc + 1]);

  switch (opcode & 0xF000) {

  case 0x0000:
    switch (opcode & 0x00FF) {
    case 0x00E0:
      printf("clear screen");
      pc += 2;
      break;
    case 0x00EE:
      printf("return from sub");
      pc += 2;
      break;
    default:
      std::printf("Unknown opcode: 0x%X\n", opcode);
      return 0;
      break;
    }
    break;

  case 0x1000:
    I = opcode & 0x0FFF;
    printf("jump to 0x%X", I);
    pc = I;
    break;
  case 0x2000:
    I = opcode & 0x0FFF;
    printf("Call sub at 0x%X", I);
    stack[sp] = pc;
    ++sp;
    pc = I;
    break;
  case 0x3000:
    printf("compare value in V%X to %X", ((opcode & 0x0F00) >> 8),
           (opcode & 0x00FF));
    if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
      pc += 4;
    } else {
      pc += 2;
    }
    break;
  case 0x4000:
    printf("compare value in V%X to %X", (opcode & 0x0F00), (opcode & 0x00FF));
    if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
      pc += 4;
    } else {
      pc += 2;
    }
    break;
  case 0x5000:
    printf("compare value in V%X with value in V%X", (opcode & 0x0F00),
           (opcode & 0x00F0) >> 4);
    if (V[(opcode & 0x0F00 >> 8)] == V[(opcode & 0x00F0 >> 4)]) {
      pc += 4;
    } else {
      pc += 2;
    }
    break;
  case 0x6000:
    printf("set value of V%X to %X", ((opcode & 0x0F00) >> 8),
           (opcode & 0x00FF));
    V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
    pc += 2;
    break;
  case 0x7000:
    printf("Set Vx = Vx + kk");
    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] + (opcode & 0x00FF);
    pc += 2;
    break;
  case 0x8000:
    switch (opcode & 0x000F) {
    case 0x0000:
      printf("Set Vx = Vy");
      V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    case 0x0001:
      printf("Set Vx to Vx or Vy");
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    case 0x0002:
      printf("set Vx to Vx and Vy");
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    case 0x0003:
      printf("Set Vx to Vx XOR Vy");
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    case 0x0004:
      printf("Set Vx to Vx + Vy, VF = carry");
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];
      if (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4] > 255) {
        V[0xF] = 1;
      }
      pc += 2;
      break;
    case 0x0005:
      printf("Set Vx to Vx - Vy, if Vx > Vy set VF to 1, else 0");
      if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]) {
        V[0xF] = 1;
      }
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    case 0x0006:
      printf("If least sig bit of Vx is 1, set VF to 1, else 0. Vx = Vx/2 "
             "(bitwise shift)");
      if (V[(opcode & 0x0F00) >> 8] % 2 == 1) {
        V[0xF] = 1;
      }
      V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] >> 1;
      pc += 2;
      break;
    case 0x0007:
      printf("Set Vx to Vy - Vx. If Vy > Vx, not borrow VF = 1");
      if (V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0) >> 4]) {
        V[0xF] = 1;
      }
      V[opcode & 0x0F00 >> 8] =
          V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
      pc += 2;
      break;
    case 0x000E:
      printf("If most sig of Vx is 1, set VF to 1, else 0. Vx = Vx*2 (bit "
             "shift)");
      if (V[(opcode & 0x0F00) >> 8] > 127) {
        V[0xF] = 1;
      }
      V[opcode & 0x0F00 >> 8] = V[opcode & 0x0F00 >> 8] << 1;
      pc += 2;
      break;
    default:
      std::printf("Unknown opcode: 0x%X\n", opcode);
      break;
    }
    break;

  case 0x9000:
    printf("Skip next instruction if Vx != Vy");
    if (V[(opcode & 0x0F00) >> 8] != V[opcode & 0x00F0 >> 4]) {
      pc += 4;
    } else {
      pc += 2;
    }
    break;
  case 0xA000:
    I = opcode & 0x0FFF;
    pc += 2;
    break;
  case 0xB000:
    printf("jump to location 0x%X", (opcode & 0x0FFF));
    pc = (opcode & 0x0FFF) + V[0x0];
    break;
  case 0xC000:
    output = 0 + (rand() % static_cast<int>(255 - 0 + 1));
    V[opcode & 0x0F00] = (opcode & 0x00FF) & output;
    pc += 2;
    break;
  case 0xD000:
    printf("Draw n-byte sprite at mem loc I at (Vx, Vy), set VF = collision");
    n = opcode & 0x000F;
    //    for (int i = 0; i < n; ++i) {
    //      sprite.push_back(memory[I + i]);
    //    }
    loc[0] = (opcode & 0x0F00) >> 8;
    loc[1] = (opcode & 0x00F0) >> 4;
    printf("Display sprite at location loc");
    if (p_erase) {
      V[0xF] = 1;
    }
    pc += 2;
    break;
  case 0xE000:
    switch (opcode & 0x00FF) {
    case 0x009E:
      printf("Skip next instruction if key with value of Vx is pressed.");
      // if (get_key() == V[opcode & 0x0F00]) {
      if (1 == V[opcode & 0x0F00]) {
        pc += 4;
      } else {
        pc += 2;
      }
      break;
    case 0x00A1:
      printf("Skip next instruction if key with value of Vx is not pressed");
      // if (get_key() != V[opcode & 0x0F00]) {
      if (1 != V[opcode & 0x0F00]) {
        pc += 4;
      } else {
        pc += 2;
      }
      break;
    default:
      std::printf("Unknown opcode: 0x%X\n", opcode);
      break;
    }
    break;

  case 0xF000:
    switch (opcode & 0x00FF) {
    case 0x0007:
      printf("Set Vx = delay timer value.");
      V[opcode & 0x0F00] = delay_timer;
      pc += 2;
      break;
    case 0x000A:
      printf("Wait for a key press, store the value of the key in Vx.");
      while (true) {
        // char key = get_key();
        char key = 1;
        if (key) {
          V[opcode & 0x0F00] = key;
        }
      }
      pc += 2;
      break;
    case 0x0015:
      printf("Set delay timer = Vx.");
      delay_timer = V[opcode & 0x0F00];
      pc += 2;
      break;
    case 0x0018:
      printf("Set sound timer = Vx.");
      sound_timer = V[opcode & 0x0F00];
      pc += 2;
      break;
    case 0x001E:
      printf("Set I = I + Vx");
      I = I + V[opcode & 0x0F00];
      pc += 2;
      break;
    case 0x0029:
      printf("Set I = location of sprite for digit Vx.");
      I = (V[opcode & 0x0F00] - 1) * 5;
      pc += 2;
      break;
    case 0x0033:
      printf(" Store BCD representation of Vx in memory locations I, I+1, and "
             "I+2.");

      pc += 2;
      break;
    case 0x0055:
      printf("Store registers V0 through Vx in memory starting at location I.");
      for (int i = 0; i <= (opcode & 0x0F00); ++i) {
        memory[I + i] = V[i];
      }
      pc += 2;
      break;
    case 0x0065:
      printf("Read registers V0 through Vx from memory starting at location I");
      for (int i = 0; i <= (opcode & 0x0F00); ++i) {
        V[i] = memory[I + i];
      }
      pc += 2;
      break;
    default:
      std::printf("Unknown opcode: 0x%X\n", opcode);
      break;
    }
    break;

  default:
    std::printf("Unknown opcode: 0x%X\n", opcode);
    return 0;
    break;
  }

  if (delay_timer > 0)
    --delay_timer;

  if (sound_timer > 0) {
    if (sound_timer == 1)
      std::printf("BEEP!\n");
    --sound_timer;
  }
  return 1;
}
