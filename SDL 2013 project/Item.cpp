#include "Item.h"



Item::Item()
{
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = 300;
	srcRect.h = 300;
	collisionBox.w = 300 / 7;
	collisionBox.h = 300 / 7;
}


Item::~Item()
{
}

void Item::draw()
{
	SDL_RenderCopy(renderer, texture, &srcRect, &collisionBox);
}


