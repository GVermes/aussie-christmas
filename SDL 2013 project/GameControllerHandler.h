#pragma once
#include "InputHandler.h"
class GameControllerHandler :
	public InputHandler
{
public:
	SDL_GameController* controller;

	GameControllerHandler();
	~GameControllerHandler();

	void close();


	void setupController();
	//override
	virtual void update(SDL_Event* event);
	virtual void updateHeld();
};

