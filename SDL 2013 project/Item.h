#pragma once
#include "GameObject.h"
class Item :
	public GameObject
{
public:
	Item();
	~Item();

	SDL_Rect srcRect;
	SDL_Texture* texture;
	SDL_Surface* sourface;

	void draw();

};

