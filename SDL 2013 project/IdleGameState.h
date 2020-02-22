#pragma once
#include "GameState.h"
#include "GameObject.h"
#include <stdlib.h> //rand

class IdleGameState :
	public GameState
{
public:
	float idleTimer; //1=1 second, 0.2 = 200 milliseconds
	GameObject* gameObject = NULL;

	IdleGameState();
	~IdleGameState();

	//override all of the abstract methods from GameState
	virtual void update(float dt);
	virtual void render();
	virtual bool onEnter();
	virtual bool onExit();
	virtual std::string getStateID();
};

