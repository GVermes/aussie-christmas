#include "Globals.h"


SDL_Window* Globals::window = NULL;
SDL_Renderer* Globals::renderer = NULL;


int Globals::screenWidth = 1280;
int Globals::screenHeight = 720;
SDL_Rect Globals::camera = { 0, 0, Globals::screenWidth, Globals::screenHeight };