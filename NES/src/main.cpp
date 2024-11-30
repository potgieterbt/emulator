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
#include <memory>
#include <string>
#include <utility>
#include <vector>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << "Usage: ./NES <path/to/rom>";
    return 1;
  }

  const std::string path = argv[1];

  ROM cart;
  std::shared_ptr<ROM> rom_ptr = std::shared_ptr<ROM>(&cart);
  bool res = cart.loadROM(path);
  if (!res) {
    printf("Could not load file, Aborting");
    return 1;
  }

  const std::vector<uint8_t> &chr_rom = cart.getCHR();
  ppu PPU(rom_ptr);
  std::shared_ptr<ppu> ppu_ptr = std::shared_ptr<ppu>(&PPU);
  PPU.setMapper(cart.getMapperNumber());

  const std::vector<uint8_t> &prg_rom = cart.getPRG();
  cpu CPU(rom_ptr, ppu_ptr);

  std::vector<uint8_t> test = cart.getPRG();

  CPU.reset();

  // The window we'll be rendering to
  SDL_Window *window = NULL;

  // The surface contained by the window
  SDL_Surface *screenSurface = NULL;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  }

  // Create window
  window =
      SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 512, 480, SDL_WINDOW_SHOWN);

  std::cin.get();
  if (window == NULL) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
  }
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
                        SDL_TEXTUREACCESS_STREAMING, 256, 240);
  if (texture == nullptr) {
    printf("Error in initialising rendering %s", SDL_GetError());
    SDL_Quit();
    exit(1);
  }

  while (true) {
    CPU.runCycle();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        exit(0);
      }

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          exit(0);
        }
      }
    }
    if (PPU.getFrameComplete() == true) {
      PPU.setFrameComplete(false);

      SDL_UpdateTexture(texture, NULL, PPU.getVdisplayCopy().data(), 256);
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, texture, NULL, NULL);
      SDL_RenderPresent(renderer);
    }
  }

  // Destroy window
  SDL_DestroyWindow(window);

  // Quit SDL subsystems
  SDL_Quit();

  return 0;
}
