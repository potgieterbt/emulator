#include "cpu.hpp"
#include "ppu.hpp"
#include "rom.hpp"
#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << "Usage: ./NES <path/to/rom>";
    return 1;
  }

  const std::string path = argv[1];

  ROM cart;
  bool res = cart.loadROM(path);
  if (!res) {
    printf("Could not load file, Aborting");
    return 1;
  }

  const std::vector<uint8_t> &chr_rom = cart.getCHR();
  ppu ppu(chr_rom);
  ppu.setMapper(cart.getMapperNumber());
  for (int i = 0; i < 10; ++i) {
    printf("%x\n", ppu.getVdisplayCopy()[i]);
  }

  const std::vector<uint8_t> &prg_rom = cart.getPRG();
  cpu CPU(prg_rom, ppu);

  std::vector<uint8_t> test = cart.getPRG();

  CPU.reset();

  // while (true) {
  //   CPU.runCycle();
  //   // std::cin.get();
  // }

  // The window we'll be rendering to
  SDL_Window *window = NULL;

  // The surface contained by the window
  SDL_Surface *screenSurface = NULL;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  } else {
    // Create window
    window =
        SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, 256, 240, SDL_WINDOW_SHOWN);
    if (window == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else {
      // Get window surface
      SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
      if (renderer == nullptr) {
        printf("Error in initialising rendering %s", SDL_GetError());
        SDL_Quit();
        exit(1);
      }

      SDL_RenderSetLogicalSize(renderer, 512, 480);

      SDL_Texture *texture =
          SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                            SDL_TEXTUREACCESS_STREAMING, 64, 32);
      if (texture == nullptr) {
        printf("Error in initialising rendering %s", SDL_GetError());
        SDL_Quit();
        exit(1);
      }
      std::array<uint32_t, 61440> display = ppu.getVdisplayCopy();

      SDL_UpdateTexture(texture, NULL, display.data(), 256 * sizeof(uint32_t));

      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, texture, NULL, NULL);
      SDL_RenderPresent(renderer);

      // Wait ten seconds
      SDL_Delay(10000);
    }
  }
  // Destroy window
  SDL_DestroyWindow(window);

  // Quit SDL subsystems
  SDL_Quit();

  return 0;
}
