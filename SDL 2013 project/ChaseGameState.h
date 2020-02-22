#pragma once
#include "GameState.h"
#include "GameObject.h"
#include <math.h>

class ChaseGameState :
	public GameState
{
public:
	GameObject* gameObject = NULL;
	GameObject* target = NULL; //who to chase

	ChaseGameState();
	~ChaseGameState();
	//override all of the abstract methods from GameState
	virtual void update(float dt);
	virtual void render();
	virtual bool onEnter();
	virtual bool onExit();
	virtual std::string getStateID();
};

