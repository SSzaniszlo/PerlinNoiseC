#include <math.h>
#include <stdio.h>
#include <SDL.h>



typedef struct {
  float x, y;
} vector2;




#include "Main.h"

#define WIDTH 800
#define HEIGHT 800

int running;

SDL_Window* window;
SDL_Renderer* renderer;
const Uint8* keyState;

float ticksCount;

vector2 gradients[WIDTH][HEIGHT];

int main() {
  

  if (initSDL()) {
    running = 1;
    
    populateRandomGradients();
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
    
    window = SDL_CreateWindow("Test", 0, 0, WIDTH, HEIGHT, 0);

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

  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      float noise = perlin(x*0.01, y*0.01) * 255.0f;
      SDL_SetRenderDrawColor(renderer, noise, noise, noise, 255);
      
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
  vector2 gradient = gradients[ix][iy];

  float dx = x - (float) ix;
  float dy = y - (float) iy;

  return (dx*gradient.x + dy*gradient.y);

}

void populateRandomGradients() {

  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      vector2 v;
      v.x = -1+ 2*((float)rand())/ RAND_MAX;
      v.y = -1+ 2*((float)rand())/ RAND_MAX;
      gradients[x][y] = v; 
    }
  } 

}



float interpolate(float a0, float a1, float w) {
  return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
}


