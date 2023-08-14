#include "chip8.cpp"
#include <SDL2/SDL.h>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

uint8_t keymap[16] = {
    SDLK_x, SDLK_1, SDLK_2, SDLK_3, SDLK_q, SDLK_w, SDLK_e, SDLK_a,
    SDLK_s, SDLK_d, SDLK_z, SDLK_c, SDLK_4, SDLK_r, SDLK_f, SDLK_v,
};

int main(int argc, char *argv[]) {
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Texture *texture = nullptr;
  const int ht = 320, wt = 640;
  chip8 myChip8;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("Error in initialising SDL %s", SDL_GetError());
    SDL_Quit();
    exit(1);
  }

  window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, wt, ht, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("Error creating window %s", SDL_GetError());
    SDL_Quit();
    exit(1);
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == nullptr) {
    printf("Error in initialising rendering %s", SDL_GetError());
    SDL_Quit();
    exit(1);
  }

  SDL_RenderSetLogicalSize(renderer, wt, ht);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, 64, 32);
  if (texture == nullptr) {
    printf("Error in initialising rendering %s", SDL_GetError());
    SDL_Quit();
    exit(1);
  }

  myChip8.loadGame("pong");

  for (;;) {
    bool res = myChip8.emulateCycle();
    if (!res) {
      break;
    }
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        exit(0);
      }
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          exit(0);
        }
        for (int i : keymap) {
          if (event.key.keysym.sym == i) {
            myChip8.set_keypad_value(i, 1);
          }
        }
      }
      if (event.type == SDL_KEYUP) {
        for (int i = 0; i < 16; ++i) {
          if (event.key.keysym.sym == keymap[i]) {
            myChip8.set_keypad_value(i, 0);
          }
        }
      }
    }
    if (myChip8.get_draw_flag()) {
      myChip8.set_draw_flag(false);
      uint32_t pixels[32 * 64];
      for (int i = 0; i < 32 * 64; i++) {
        if (myChip8.get_display_value(i) == 0) {
          pixels[i] = 0xFF000000;
        } else {
          pixels[i] = 0xFFFFFFFF;
        }
      }

      SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint32_t));
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, texture, NULL, NULL);
      SDL_RenderPresent(renderer);
    }
    usleep(1500);
  }
  return 0;
}
