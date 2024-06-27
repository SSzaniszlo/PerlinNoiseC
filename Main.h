void render();
int initSDL();
void processInput();
float perlin(float x, float y);
float interpolate(float a0, float a1, float w);
vector2 randomGradient(int ix, int iy);
float dotGridGradient(int ix, int iy, float x, float y);

