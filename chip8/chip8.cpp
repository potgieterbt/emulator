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
      V[(opcode & 0x0F00) >> 8] = delay_timer;
      pc += 2;
      break;
    case 0x000A: {
      bool key_pressed = false;
      for (int i = 0; i < 16; ++i) {
        if (keypad[i] != 0) {
          key_pressed = true;
          V[(opcode & 0x0F00) >> 8] = i;
        }
      }
      if (key_pressed) {
        pc += 2;
      }
      break;
    }
    case 0x0015:
      delay_timer = V[opcode & 0x0F00];
      pc += 2;
      break;
    case 0x0018:
      sound_timer = V[opcode & 0x0F00];
      pc += 2;
      break;
    case 0x001E:
      V[0xF] = 0;
      if (I + V[(opcode & 0x0F00) >> 8] > 0xFFF) {
        V[0xF] = 1;
      }
      I = I + V[opcode & 0x0F00];
      pc += 2;
      break;
    case 0x0029:
      I = (V[(opcode & 0x0F00) >> 8]) * 5;
      pc += 2;
      break;
    case 0x0033:
      n = (opcode & 0x0F00) >> 8;
      memory[I] = (uint8_t)((uint8_t)V[n] / 100);
      memory[I + 1] = (uint8_t)((uint8_t)(V[n] / 10) % 10);
      memory[I + 2] = (uint8_t)((uint8_t)(V[n] % 100) % 10);
      pc += 2;
      break;
    case 0x0055:
      for (int i = 0; i <= (opcode & 0x0F00) >> 8; ++i) {
        memory[I + i] = V[i];
      }
      I = I + ((opcode & 0x0F00) >> 8) + 1;
      pc += 2;
      break;
    case 0x0065:
      for (int i = 0; i <= (opcode & 0x0F00) >> 8; ++i) {
        V[i] = memory[I + i];
      }
      I = I + ((opcode & 0x0F00) >> 8) + 1;
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
    --sound_timer;
  }
  return 1;
}

chip8::~chip8() {}
