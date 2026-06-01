#include <SDL2/SDL.h>

#include <iostream>

#include "chip8.h"

const int SCALE = 10;
const int WIDTH = 64 * SCALE;
const int HEIGHT = 32 * SCALE;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    SDL_Log("Usage: %s <rom path>\n", argv[0]);
    return 1;
  }

  // Init SDL
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window =
      SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WIDTH, HEIGHT, 0);
  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  chip8 cpu;

  // Load ROM
  FILE* rom = fopen(argv[1], "rb");
  fread(cpu.ram + 0x200, 1, sizeof(cpu.ram) - 0x200, rom);
  fclose(rom);

  bool running = true;
  SDL_Event event;

  while (running) {
    // Input
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) running = false;
    }

    // Update: ~12 instructions per frame at 60fps ≈ 700hz
    for (int i = 0; i < 12; i++) {
      uint16_t opcode = cpu.fetch();
      cpu.decode(opcode);
    }

    // Render
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < 32; y++) {
      for (int x = 0; x < 64; x++) {
        if (cpu.display[y][x]) {
          SDL_Rect rect = {x * SCALE, y * SCALE, SCALE, SCALE};
          SDL_RenderFillRect(renderer, &rect);
        }
      }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1000 / 60);  // cap at 60fps
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}