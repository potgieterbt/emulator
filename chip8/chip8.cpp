#include "chip8.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>

unsigned char chip8_fontset[80] = {
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

chip8::chip8() {}
chip8::~chip8() {}

void chip8::init() {
  pc = 0x200;
  opcode = 0;
  I = 0;
  sp = 0;

  for (int i = 0; i < 2048; ++i) {
    gfx[i] = 0;
  }

  for (int i = 0; i < 16; ++i) {
    stack[i] = 0;
    key[i] = 0;
    V[i] = 0;
  }

  for (int i = 0; i < 4096; ++i) {
    memory[i] = 0;
  }

  for (int i = 0; i < 80; ++i) {
    memory[i] = chip8_fontset[i];
  }

  delay_timer = 0;
  sound_timer = 0;

  srand(time(NULL));
}

bool chip8::loadGame(std::string rom_path) {
  init();
  rom_path = rom_path;
  std::ifstream gamefile(rom_path, std::ios::binary | std::ios::in);
  if (!gamefile) {
    return false;
  }

  char c;

  for (int i = 0x200; gamefile.get(c) && i < 4096; ++i) {
    memory[i] = (uint8_t)c;
  }
  return true;
};

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
      draw_flag = true;
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
    pc = opcode & 0x0FFF;
    break;

  case 0x2000:
    stack[sp] = pc;
    ++sp;
    pc = opcode & 0x0FFF;
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
    if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
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
    V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
    pc += 2;
    break;

  case 0x8000:
    switch (opcode & 0x000F) {

    case 0x0000:
      V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;

    case 0x0001:
      V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;

    case 0x0002:
      V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;

    case 0x0003:
      V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;

    case 0x0004:
      V[0xF] = 0;
      if (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4] > 255) {
        V[0xF] = 1;
      }
      V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;

    case 0x0005:
      V[0xF] = 0;
      if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]) {
        V[0xF] = 1;
      }
      V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;

    case 0x0006:
      V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
      V[(opcode & 0x0F00) >> 8] >>= 1;
      pc += 2;
      break;

    case 0x0007:
      V[0xF] = 0;
      if (V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0) >> 4]) {
        V[0xF] = 1;
      }
      V[opcode & 0x0F00 >> 8] =
          V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
      pc += 2;
      break;

    case 0x000E:
      V[0xF] = 0;
      if (V[(opcode & 0x0F00) >> 8] >> 7) {
        V[0xF] = 1;
      }
      V[opcode & 0x0F00 >> 8] <<= 1;
      pc += 2;
      break;

    default:
      std::printf("Unknown opcode: 0x%X\n", opcode);
      break;
    }
    break;

  case 0x9000:
    if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
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
    V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF) & (rand() % (0xFF + 1));
    pc += 2;
    break;

  case 0xD000: {
    unsigned short x = V[(opcode & 0x0F00) >> 8];
    unsigned short y = V[(opcode & 0x00F0) >> 4];
    int ht = opcode & 0x000F;
    int wt = 8;
    unsigned short pixel;

    V[0x0F] = 0;
    for (int i = 0; i < ht; ++i) {
      pixel = memory[I + i];
      for (int j = 0; j < wt; ++j) {
        if ((pixel & (0x80 >> j)) != 0) {
          int index = ((x + j) + ((y + i) * 64));
          if (gfx[index] == 1) {
            V[0x0F] = 1;
          }
          gfx[index] ^= 1;
        }
      }
    }

    draw_flag = true;
    pc += 2;
  } break;

  case 0xE000:

    switch (opcode & 0x00FF) {
    case 0x009E:
      if (key[V[(opcode & 0x0F00) >> 8]] != 0) {
        pc += 4;
      } else {
        pc += 2;
      }
      break;

    case 0x00A1:
      if (key[V[(opcode & 0x0F00) >> 8]] == 0) {
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
      for (int i = 0; i < 16; i++) {
        if (key[i] != 0) {
          key_pressed = true;
          V[(opcode & 0x0F00) >> 8] = i;
        }
      }
      if (!key_pressed) {
        return true;
      }
      pc += 2;
    } break;

    case 0x0015:
      delay_timer = V[(opcode & 0x0F00) >> 8];
      pc += 2;
      break;

    case 0x0018:
      sound_timer = V[(opcode & 0x0F00) >> 8];
      pc += 2;
      break;

    case 0x001E:
      V[0xF] = 0;
      if (I + V[(opcode & 0x0F00) >> 8] > 0xFFF) {
        V[0xF] = 1;
      }
      I += V[(opcode & 0x0F00) >> 8];
      pc += 2;
      break;

    case 0x0029:
      I = (V[(opcode & 0x0F00) >> 8]) * 5;
      pc += 2;
      break;

    case 0x0033:
      n = (opcode & 0x0F00) >> 8;
      memory[I] = (V[n] / 100);
      memory[I + 1] = (V[n] / 10) % 10;
      memory[I + 2] = V[n] % 10;
      pc += 2;
      break;

    case 0x0055:
      for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
        memory[I + i] = V[i];
      }
      I += ((opcode & 0x0F00) >> 8) + 1;
      pc += 2;
      break;

    case 0x0065:
      for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
        V[i] = memory[I + i];
      }
      I += ((opcode & 0x0F00) >> 8) + 1;
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
