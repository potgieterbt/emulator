#include <SDL2/SDL.h>
#include <cstddef>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
enum KeyPressSurfaces {
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL,
};

bool init();
bool loadMedia();
void close();
SDL_Surface *loadSurface(std::string path);

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface *gCurrentSurface = NULL;

int main(int argc, char *argv[]) {
  if (!init()) {
    printf("Failed to initialize!\n");
  } else {
    if (!loadMedia()) {
      printf("Failed to load media!\n");
    } else {

      bool quit = false;
      SDL_Event e;
      gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
      while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
          if (e.type == SDL_QUIT) {
            quit = true;
          } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {

            case SDLK_UP:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
              break;

            case SDLK_DOWN:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
              break;

            case SDLK_LEFT:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
              break;

            case SDLK_RIGHT:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
              break;

            default:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
              break;
            }
          }
        }
        SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
        SDL_UpdateWindowSurface(gWindow);
      }
      return 0;
    }
  }
}

bool init() {
  bool success = true;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    success = false;
  } else {
    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      success = false;
    } else {
      gScreenSurface = SDL_GetWindowSurface(gWindow);
    }
  }

  return success;
}

bool loadMedia(std::string path) {
  bool success = true;
  SDL_Surface *optimizedSurface = NULL;
  SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());

  if (loadedSurface == NULL) {
    printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
  } else {
    optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
  }

  return success;
}

void close() {
  for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i) {
    SDL_FreeSurface(gKeyPressSurfaces[i]);
    gKeyPressSurfaces[i] = NULL;
  }
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  SDL_Quit();
}

SDL_Surface *loadSurface(std::string path) {
  SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());
  if (loadedSurface == NULL) {
    printf("Unable to load image %s! SDL Error: %s\n", path.c_str(),
           SDL_GetError());
  }
  return loadedSurface;
}
