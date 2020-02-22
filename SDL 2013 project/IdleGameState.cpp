#include "IdleGameState.h"


IdleGameState::IdleGameState()
{
}


IdleGameState::~IdleGameState()
{
}

void IdleGameState::update(float dt){
	idleTimer -= dt;
	if (idleTimer <= 0){
		done = true;
	}
}
void IdleGameState::render(){
	if (gameObject != NULL)
	{
		//fill out
	}
}
bool IdleGameState::onEnter(){
	idleTimer = rand() % 2 + 1;//around 2 seconds of waiting  
	done = false;

	return true;
}
bool IdleGameState::onExit(){
	return true;
}
std::string IdleGameState::getStateID(){
	return "idleGameState";
}