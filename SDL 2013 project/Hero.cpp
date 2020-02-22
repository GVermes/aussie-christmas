#include "Hero.h"

Hero::Hero()
{
	animation = NULL;
	faceRight = true;

	type = "hero";
}


Hero::~Hero()
{
}

int Hero::getScore()
{
	return score;
}

void Hero::update(float dt)
{
	
	//use basic game object vector movement
	updateMovement(dt);

	//update facing direction based on velocity
	if (velocity.x > 0)
		faceRight = true;
	if (velocity.x < 0)
		faceRight = false;

	//update our animation
	if(velocity.x != 0 || velocity.y != 0)
		animation->update(dt);
}

void Hero::draw()
{
	if (animation != NULL) {
		float drawX = pos.x - Globals::camera.x;
		float drawY = pos.y - Globals::camera.y;

		if (faceRight)
			animation->draw(pos.x, pos.y);
		else
			animation->draw(pos.x, pos.y, true);
	}
}

void Hero::updateMovement(float dt)
{
	if (!willCollideWithWall(dt)) {

		//velocity changes position
		pos.x = pos.x + velocity.x * dt;
		pos.y = pos.y + velocity.y * dt;

		//re-aligns the collisionbox
		updateCollisionBox();
	}
}

void Hero::updateCollisionBox() {
	collisionBox.w = 32;
	collisionBox.h = 50;
	collisionBox.x = pos.x;
	collisionBox.y = pos.y;
}

void Hero::updateCollisions(float dt) {
	for (list<GameObject*>::iterator gObj = GameObject::gameObjects->begin();
		gObj != GameObject::gameObjects->end(); gObj++)
	{
		if (((*gObj)->type == "present" || (*gObj)->type == "beer") && GameObject::AABBCheck(collisionBox, (*gObj)->collisionBox))
		{
			SoundManager::soundManager.playSound("coin");
			(*gObj)->active = false;
			score++;
		}
	}
}

