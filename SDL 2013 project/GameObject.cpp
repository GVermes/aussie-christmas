#include "GameObject.h"
#include <iostream>

using namespace std;


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::setPosition(Vector pos)
{
	this->pos = pos;
}

void GameObject::setVelocity(Vector pos)
{
	this->velocity = velocity;
}

void GameObject::update(float dt)
{
	//let sub classes fill these in if appropriate
}

void GameObject::updateMovement(float dt)
{
	// let sub classes fill there in if neccessarry
}

bool GameObject::willCollideWithWall(float dt)
{
	SDL_Rect nextMoveCollisionBox;
	nextMoveCollisionBox.w = collisionBox.w;
	nextMoveCollisionBox.h = collisionBox.h;

	if (velocity.x > 0) {
		nextMoveCollisionBox.x = pos.x + 32 + velocity.x *dt;
	}
	else {
		nextMoveCollisionBox.x = pos.x + velocity.x *dt;
	}

	if (velocity.y > 0) {
		nextMoveCollisionBox.y = pos.y + 50 + velocity.y *dt;
	}
	else {
		nextMoveCollisionBox.y = pos.y + velocity.y *dt;
	}

	for (list<GameObject*>::iterator gObj = GameObject::gameObjects->begin();
		gObj != GameObject::gameObjects->end(); gObj++)
	{
		if ((*gObj)->type == "wall" && GameObject::AABBCheck(nextMoveCollisionBox, (*gObj)->collisionBox))
		{
			return true;
		}
	}
	return false;
}

void GameObject::updateCollisionBox(){
	//reposition collision box to where pos.x and pos.y are
	collisionBox.x = pos.x;
	collisionBox.y = pos.y;
	//Note: override this function if you need something different to happen
}

void GameObject::updateCollisions(float dt){
	//override if need to deal with collisions
}

void GameObject::draw()
{
}

//initialising static variable on start of program
list<GameObject*> *GameObject::gameObjects = NULL;

bool GameObject::AABBCheck(SDL_Rect b1, SDL_Rect b2){
		return !(b1.x + b1.w < b2.x || b1.x > b2.x + b2.w || b1.y + b1.h < b2.y || b1.y > b2.y + b2.h);
	}

void GameObject::deleteInactiveGameObjects(){
	for (list<GameObject*>::iterator gObj = gameObjects->begin();
		gObj != gameObjects->end(); /*gObj++*/)
	{
		//if gameobject not active
		if (!(*gObj)->active)
		{
			//deleting from RAM
			delete (*gObj);
			//remove reference the list
			gObj = gameObjects->erase(gObj);
		}
		else
		{
			//if active, then move to next thing in the list
			gObj++;
		}
	}
}