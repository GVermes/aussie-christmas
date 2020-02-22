#include "GameControllerHandler.h"



GameControllerHandler::GameControllerHandler()
{
	controller = NULL;

	setupController();
}


GameControllerHandler::~GameControllerHandler()
{
	
}

void GameControllerHandler::close()
{
	//cleanup any memory reserved to describe this controller
	if (controller != NULL) {
		SDL_GameControllerClose(controller);
		controller = NULL;
	}
}

void GameControllerHandler::setupController()
{
	
	//lets see how many are currently attached to the computer
	int numJoySticks = SDL_NumJoysticks();

	//lets loop through them, joystick ids start at id = 0
	for (int joyStickID = 0; joyStickID < numJoySticks; joyStickID++)
	{
		//check if this joystick is a game controller (xbox, ps or logitech controller)
		if (SDL_IsGameController(joyStickID)) {
			//if it is, setup controller with this joystick
			controller = SDL_GameControllerOpen(joyStickID);
			//we are done, exit loop
			break;
		}
	}
}

void GameControllerHandler::update(SDL_Event * event)
{
	if (controller != NULL )
	{
		//grab the joystick id of the connected controller
		SDL_JoystickID joyStickID = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));


		//if controller removed
		if (controller != NULL && event->type == SDL_JOYDEVICEREMOVED )
		{
			if (event->jdevice.which == joyStickID) {
				close();
				return;
			}
		}

		//check if events triggered are from this game controller
		if (event->cdevice.which == joyStickID)
		{


			//IF button is pressed
			if (event->type == SDL_CONTROLLERBUTTONDOWN)
			{
				//Note: think of all controllers as Xbox controllers.
				//E.G ps4 controllers X button lines up with xbox's A button
				if (event->cbutton.button == SDL_CONTROLLER_BUTTON_A)
				{
					//reset hero position
					hero->pos.x = 200;
					hero->pos.y = 200;
				}
			}
		}
	}
	


	//if controller added
	if (controller == NULL && event->type == SDL_JOYDEVICEADDED)
	{
		//check if this joystick is a game controller (xbox, ps or logitech controller)
		if (SDL_IsGameController(event->jdevice.which)) {
			//if it is, setup controller with this joystick
			controller = SDL_GameControllerOpen(event->jdevice.which);
		}
	}
}

void GameControllerHandler::updateHeld()
{
	if (controller == NULL)
		return; //exit function early


	//read left sticks X and Y axis values
	Sint16 axisX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
	Sint16 axisY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);

	//stick generally doesn't rest at zero like we would like, so we cater for a range of numbers to ignore
	int deadzone = 10000;

	//check if moving stick LEFT OR DPAD left button pressed
	if (axisX < -deadzone || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
		hero->velocity.x = -150;
	//check right
	if (axisX > deadzone || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
		hero->velocity.x = 150;
	//check up
	if (axisY < -deadzone || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP))
		hero->velocity.y = -150;
	//check right
	if (axisY > deadzone || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		hero->velocity.y = 150;

	//check if stopped on x axis
	if (axisX >= -deadzone && axisX <= deadzone 
		&& !SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) 
		&& !SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
		hero->velocity.x = 0;
	//check if stopped on y axis
	if (axisY >= -deadzone && axisY <= deadzone 
		&& !SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP) 
		&& !SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		hero->velocity.y = 0;

}
