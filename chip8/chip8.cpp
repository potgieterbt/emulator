#include "chip8.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

chip8::chip8() {
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
  memset(keypad, 0, sizeof(keypad));
}

void chip8::loadGame(std::string rom_path) {
  rom_path = "./PONG";
  std::vector<char> buffer;
  std::ifstream gamefile(rom_path, std::ios::binary | std::ios::in);

  char c;

  for (int i = 0x200; gamefile.get(c) && i < 4096; ++i) {
    memory[i] = (uint8_t)c;
  }
};

bool chip8::get_draw_flag() { return draw_flag; }

void chip8::set_draw_flag(bool flag) { draw_flag = flag; }

int chip8::get_display_value(int i) { return gfx[i]; }

void chip8::set_keypad_value(int index, int val) { keypad[index] = val; }

bool chip8::emulateCycle() {
  opcode = (memory[pc]) << 8 | memory[pc + 1];

  printf("\n");
  printf("%.4X, %.4X, %.2X", pc, opcode, sp);
  for (int i = 0; i < 15; ++i) {
    printf("%.2X ", V[i]);
  }
  printf("\n");

  switch (opcode & 0xF000) {

  case 0x0000:
    switch (opcode & 0x00FF) {
    case 0x00E0:
      printf("clear screen");
      memset(gfx, 0, sizeof(gfx));
      pc += 2;
      break;
    case 0x00EE:
      sp -= 1;
      pc = stack[sp];
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
    pc = I;
    break;
  case 0x2000:
    I = opcode & 0x0FFF;
    stack[sp] = pc;
    ++sp;
    pc = I;
    break;
  case 0x3000:
    if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
      pc += 4;
    } else {
      pc += 2;
    }
    break;
  case 0x4000:
    if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
      pc += 4;
    } else {
      pc += 2;
    }
    break;
  case 0x5000:
    if (V[(opcode & 0x0F00 >> 8)] == V[(opcode & 0x00F0 >> 4)]) {
      pc += 4;
    } else {
      pc += 2;
    }
    break;
  case 0x6000:
    V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
    pc += 2;
    break;
  case 0x7000:
    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] + (opcode & 0x00FF);
    pc += 2;
    break;
  case 0x8000:
    switch (opcode & 0x000F) {
    case 0x0000:
      V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    case 0x0001:
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    case 0x0002:
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    case 0x0003:
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    case 0x0004:
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];
      V[0xF] = 0;
      if (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4] > 255) {
        V[0xF] = 1;
      }
      pc += 2;
      break;
    case 0x0005:
      if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]) {
        V[0xF] = 1;
      }
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    case 0x0006:
      if (V[opcode & 0x0001]) {
        V[0xF] = 1;
      }
      V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] >> 1;
      pc += 2;
      break;
    case 0x0007:
      if (V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0) >> 4]) {
        V[0xF] = 1;
      }
      V[opcode & 0x0F00 >> 8] =
          V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
      pc += 2;
      break;
    case 0x000E:
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
    pc = (opcode & 0x0FFF) + V[0x0];
    break;
  case 0xC000:
    output = (rand() % (255 + 1));
    V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF) & output;
    pc += 2;
    break;
  case 0xD000: {
    printf("Draw n-byte sprite at mem loc I at (Vx, Vy), set VF = collision");
    loc[0] = ((opcode & 0x0F00) >> 8) & n;
    loc[1] = (opcode & 0x00F0) >> 4;
    n = opcode & 0x000F;
    int ht = opcode & 0x000F;
    int wt = 8;
    for (int i = 0; i < n; ++i) {
      int pixel = memory[I + i];
      for (int j = 0; j < wt; ++j) {
        if ((pixel & (0x80 >> j)) != 0) {
          int index = ((loc[0] + j) + ((loc[1] + i) * 64)) % 2048;
          if (gfx[index] == 1) {
            V[0x0F] = 1;
          }
          gfx[index] ^= 1;
        }
      }
    }
    draw_flag = true;
    pc += 2;
    break;
  }
  case 0xE000:
    switch (opcode & 0x00FF) {
    case 0x009E:
      if (keypad[V[(opcode & 0x0F00) >> 8]] != 0) {
        pc += 4;
      } else {
        pc += 2;
      }
      break;
    case 0x00A1:
      printf("Skip next instruction if key with value of Vx is not pressed");
      if (keypad[V[(opcode & 0x0F00) >> 8]] == 0) {
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

chip8::~chip8() {}
