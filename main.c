#include <math.h>
#include <stdio.h>
#include <SDL.h>



typedef struct {
  float x, y;
} vector2;




#include "Main.h"

const int width = 800;
const int height = 800;
unsigned int running;

SDL_Window* window;
SDL_Renderer* renderer;
const Uint8* keyState;

float ticksCount;

int main() {
  
  printf("Hello world");

  if (initSDL()) {
    running = 1;
    
    render();

    while (running) {
      
      while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 32));
        
      float deltaTime = (SDL_GetTicks() - ticksCount) / 1000.0f;
      ticksCount = SDL_GetTicks();

    
      processInput();
    
    }
  }

}

int initSDL() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to init SDL: %s", SDL_GetError());
        return 0;
    }
    
    window = SDL_CreateWindow("Test", 0, 0, width, height, 0);

    if (!window) {
        SDL_Log("Enable to create window: %s", SDL_GetError());
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return 0;
    }

    keyState = SDL_GetKeyboardState(NULL);

    return 1;
}




void render() {

  

  SDL_SetRenderDrawColor(renderer,255,255,255,255);
  SDL_RenderClear(renderer);

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      float noise = perlin(x*0.25f, y*0.25f) * 255.0f;
      SDL_RenderPresent(renderer);
      SDL_SetRenderDrawColor(renderer, noise, noise, noise, 255);
      
      printf("%f\n", noise);
      

      SDL_RenderDrawPoint(renderer,x,y);
      
    }
  }

  SDL_RenderPresent(renderer);

  fflush(stdout);
}


void processInput() {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = 0;
                break;     
            default:
                break;
        }
    }

    if (keyState[SDL_SCANCODE_ESCAPE]) {
      running = 0;
    }

}


float perlin(float x, float y) {
 
  int x0 = (int) floor(x);
  int x1 = x0 + 1;
  int y0 = (int) floor(y);
  int y1 = y0 + 1;

  float sx = x - (float) x0;
  float sy = y - (float) y0;

  float n0,n1,ix0,ix1,value;
  
  n0 = dotGridGradient(x0, y0, x, y);
  n1 = dotGridGradient(x1, y0, x, y);
  ix0 = interpolate(n0, n1, sx);

  n0 = dotGridGradient(x0, y1, x, y);
  n1 = dotGridGradient(x1, y1, x, y);
  ix1 = interpolate(n0, n1, sx);

  value = interpolate(ix0, ix1, sy);
  
  value = (value * 0.5) + 0.5; // Make positive

  return value;
}

float dotGridGradient(int ix, int iy, float x, float y) {
  vector2 gradient = randomGradient(ix, iy);

  float dx = x - (float) ix;
  float dy = y - (float) iy;

  return (dx*gradient.x + dy*gradient.y);

}

vector2 randomGradient(int ix, int iy) {
  
  const unsigned w = 8 * sizeof(unsigned);
  const unsigned s = w / 2;

  unsigned a = ix, b = iy;

  a *= 3284157443; b^= a << s | a >> w-s;
  b *= 1911520717; a ^= b << s | b >> w-s;
  a *= 2048419325;
  float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
  vector2 v;
  v.x = cos(random); v.y = sin(random);
  return v;
}

float interpolate(float a0, float a1, float w) {
  return (a1 - a0) * w + a0;
}


