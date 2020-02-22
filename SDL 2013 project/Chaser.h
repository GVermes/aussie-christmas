#pragma once
#include "GameObject.h"
#include "GameStateMachine.h"
#include "IdleGameState.h"
#include "ChaseGameState.h"
#include "Animation.h"
#include "SoundManager.h"

class Chaser :
	public GameObject
{
public:
	GameStateMachine gsm;
	GameObject* target = NULL;
	Animation* animation;
	bool faceRight;

	Chaser();
	~Chaser();

	//override
	virtual void update(float dt);
	virtual void draw();

	void updateMovement(float dt);

	void updateCollisionBox();
	void updateCollisions(float dt);
};

