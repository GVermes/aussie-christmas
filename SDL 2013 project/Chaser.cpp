#include "Chaser.h"

Chaser::Chaser()
{
	//build idle game state for our default state for chaser
	IdleGameState* idle = new IdleGameState();
	idle->gameObject = this;

	gsm.push(idle);

	faceRight = true;

	type = "chaser";
}


Chaser::~Chaser()
{
}

void Chaser::update(float dt){
	gsm.update(dt);

	//use basic game object vector movement
	updateMovement(dt);

	//update facing direction based on velocity
	if (velocity.x > 0)
		faceRight = true;
	if (velocity.x < 0)
		faceRight = false;

	//update our animation
	if (velocity.x != 0 || velocity.y != 0)
		animation->update(dt);

	GameState* currentState = gsm.gameStates.back();
	if (currentState->done)
	{
		//if idle finished, add chase state ontop of stack
		if (currentState->getStateID() == "idleGameState")
		{
			ChaseGameState* newState = new ChaseGameState();
			newState->gameObject = this;
			newState->target = target;

			gsm.push(newState);
		}
		else
		{
			gsm.pop();
		}
	}
	updateMovement(dt);

	animation->update(dt);
}
void Chaser::draw(){
	gsm.render();
	if (animation != NULL){
		float drawX = pos.x - Globals::camera.x;
		float drawY = pos.y - Globals::camera.y;

		if (faceRight)
			animation->draw(drawX, drawY);
		else
			animation->draw(drawX, drawY, true);
	}
}

void Chaser::updateMovement(float dt)
{

	// kangaroo can jump trough wall just as second chaser the koala

		//velocity changes position
		pos.x = pos.x + velocity.x * dt;
		pos.y = pos.y + velocity.y * dt;

		//re-aligns the collisionbox
		updateCollisionBox();
	}

void Chaser::updateCollisionBox(){
	collisionBox.w = 32;
	collisionBox.h = 32;
	collisionBox.x = pos.x;
	collisionBox.y = pos.y;
}
void Chaser::updateCollisions(float dt){
	for (list<GameObject*>::iterator gObj = GameObject::gameObjects->begin();
		gObj != GameObject::gameObjects->end(); gObj++)
	{
		//if collide with santa
		if ((*gObj)->type == "hero" && GameObject::AABBCheck(collisionBox, (*gObj)->collisionBox))
		{
			SoundManager::soundManager.playSound("gameover");
			(*gObj)->active = false;
		}
	}
}