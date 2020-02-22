#include "KeyboardHandler.h"



KeyboardHandler::KeyboardHandler()
{
}


KeyboardHandler::~KeyboardHandler()
{
}

void KeyboardHandler::update(SDL_Event * event)
{
}

void KeyboardHandler::updateHeld()
{
	//check the state of keyboard keys
	//first we get all states in an array where each key is 1=pressed or 0=not pressed
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	//check individual keys from this point
	//move char up
	if (keystates[SDL_SCANCODE_UP])
		hero->velocity.y = -150;
	//down
	if (keystates[SDL_SCANCODE_DOWN])
		hero->velocity.y = 150;
	//move chara up
	if (keystates[SDL_SCANCODE_LEFT])
		hero->velocity.x = -150;
	//down
	if (keystates[SDL_SCANCODE_RIGHT])
		hero->velocity.x = 150;
	//check if not holding up or down
	if (!keystates[SDL_SCANCODE_UP] && !keystates[SDL_SCANCODE_DOWN])
		hero->velocity.y = 0;
	//if not holding left or right
	if (!keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT])
		hero->velocity.x = 0;
}
