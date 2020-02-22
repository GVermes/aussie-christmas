#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "SoundManager.h"

class Hero : public GameObject
{
public:
	Animation* animation;
	bool faceRight;
	int score = 0;

	Hero();
	~Hero();

	int getScore();

	void update(float dt);
	void updateCollisionBox();
	void updateCollisions(float dt);
	void draw();
	void updateMovement(float dt);
};

