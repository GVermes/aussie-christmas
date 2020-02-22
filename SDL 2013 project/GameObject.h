#pragma once
#include <SDL.h>
#include "Vector.h"
#include <list>
#include "Globals.h"
#include <string>

using namespace std;

class GameObject
{

public:
	string type = "gameObject";
	bool active = true;//inactive gameObjects are deleted
	SDL_Rect collisionBox;//area of gameObject that bumps into stuff

	SDL_Renderer* renderer;
	
	Vector pos; //xy position
	Vector velocity; //how fast and where to move from this pos (pixels per second)
	Vector acceleration; //how much to increase/decrease velocity (pixels per second per second)

	GameObject();
	~GameObject();

	void setPosition(Vector pos);
	void setVelocity(Vector pos);

	virtual void update(float dt);
	virtual void updateMovement(float dt);
	bool willCollideWithWall(float dt);
	virtual void updateCollisionBox();
	virtual void updateCollisions(float dt);
	virtual void draw();

	//static - exists for entire program life time, is only 1 of this variable, but shared amoungst all GameObject objects
	static list<GameObject*> *gameObjects;
	//collision detection(axis aligned bounding box)
	static bool AABBCheck(SDL_Rect b1, SDL_Rect b2);

	static void deleteInactiveGameObjects();

};

