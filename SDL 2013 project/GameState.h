#pragma once
#include <string>
#include "Globals.h"

//abstract class to help build other gamestates
class GameState
{
public:
	//if its finished or not
	bool done = false;

	GameState();
	~GameState();
	virtual void update(float dt) = 0;//abstract function
	virtual void render() = 0;
	virtual bool onEnter() = 0;
	virtual bool onExit() = 0;
	virtual std::string getStateID() = 0;
};

