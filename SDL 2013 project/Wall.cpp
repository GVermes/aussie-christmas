#include "Wall.h"

Wall::Wall()
{
	type = "wall";
}


Wall::~Wall()
{
}

void Wall::draw()
{

	float drawX = pos.x - Globals::camera.x;
	float drawY = pos.y - Globals::camera.y;

	SDL_Rect rect = { drawX,drawY,w,h };
	collisionBox.x = drawX;
	collisionBox.y = drawY;
	collisionBox.w = w;
	collisionBox.h = h;
	//set render draw colour
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	//draw the rectangle
	SDL_RenderFillRect(renderer, &rect);
}



