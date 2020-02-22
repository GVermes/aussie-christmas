#include "ChaseGameState.h"


ChaseGameState::ChaseGameState()
{
}


ChaseGameState::~ChaseGameState()
{
}

void ChaseGameState::update(float dt){
	if (gameObject != NULL && target != NULL)
	{
		//distance between gameobject and target in pixels
		float distance = abs(sqrt(pow(gameObject->pos.x - target->pos.x, 2) +
			pow(gameObject->pos.y - target->pos.y, 2)));

		if (distance > 10)
		{
			//chase target
			//get angle between gameObject and target
			float angleBetween = atan2(target->pos.y - gameObject->pos.y,
				target->pos.x - gameObject->pos.x); 

			float moveSpeed = 50;//pixels per second

			gameObject->velocity.x = cos(angleBetween) * moveSpeed;
			gameObject->velocity.y = sin(angleBetween) * moveSpeed;
		}
		else
		{
			//stop chasing
			gameObject->velocity.x = 0;
			gameObject->velocity.y = 0;
			done = true;
		}
	}
}
void ChaseGameState::render(){
	
}
bool ChaseGameState::onEnter(){
	return true;
}
bool ChaseGameState::onExit(){
	return true;
}
std::string ChaseGameState::getStateID(){
	return "chaseGameState";
}
