#include "GameStateMachine.h"


GameStateMachine::GameStateMachine()
{
}


GameStateMachine::~GameStateMachine()
{
}

void GameStateMachine::push(GameState* state){
	gameStates.push_back(state);//add state to top of stack
	//run onEnter function on newly active state
	gameStates.back()->onEnter();

}
void GameStateMachine::pop(){
	if (!gameStates.empty()){
		//run onExit before destroying this state
		if (gameStates.back()->onExit()){
			//remove the dynamically allocated gameState from ram
			delete gameStates.back();
			//remove the pointer from the stack
			gameStates.pop_back();
		}
	}
	//if we're returning to an existing state, re-run its onEnter function
	if (!gameStates.empty())
		gameStates.back()->onEnter();
}
void GameStateMachine::replace(GameState* state){
	pop();
	push(state);
}
void GameStateMachine::clearAll(){
	//delete all dynamic memory first
	for (GameState* gs : gameStates)
		delete gs;

	//then empty the stack of pointers
	gameStates.clear();
}

void GameStateMachine::update(float dt){
	if (!gameStates.empty())
		gameStates.back()->update(dt);
}
void GameStateMachine::render(){
	if (!gameStates.empty())
		gameStates.back()->render();
}
