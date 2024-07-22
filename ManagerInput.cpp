#include "ManagerInput.h"

void clearManagerInput(ManagerInput* managerInput) {
	managerInput->isEscape = false;
	managerInput->isQuit = false;
	managerInput->isUp = false;
	managerInput->isDown = false;
	managerInput->isLeft = false;
	managerInput->isRight = false;
	managerInput->isEnter = false;
}
void updateManagerInput(ManagerInput* managerInput, SDL_Event* event) {
	while (SDL_PollEvent(event))
	{
		switch (event->type)
		{
		case SDL_QUIT:
		{
			managerInput->isQuit = true;
			break;
		}

		case SDL_KEYDOWN:
		{
			switch (event->key.keysym.sym)
			{
			case SDLK_UP: {
				managerInput->isUp = true;
				break;
			}
			case SDLK_DOWN: {
				managerInput->isDown = true;
				break;
			}
			case SDLK_RIGHT: {
				managerInput->isRight = true;
				break;
			}
			case SDLK_LEFT: {
				managerInput->isLeft = true;
				break;
			}
			case SDLK_ESCAPE: {
				managerInput->isEscape = true;
				break;
			}
			case SDLK_ENTER: {
				managerInput->isEnter = true;
				break;
			}
			}
			break;
		}
		case SDL_KEYUP:
		{
			switch (event->key.keysym.sym)
			{
			case SDLK_UP: {
				managerInput->isUp = false;
				break;
			}
			case SDLK_DOWN: {
				managerInput->isDown = false;
				break;
			}
			case SDLK_RIGHT: {
				managerInput->isRight = false;
				break;
			}
			case SDLK_LEFT: {
				managerInput->isLeft = false;
				break;
			}
			case SDLK_ESCAPE: {
				managerInput->isEscape = false;
				break;
			}
			case SDLK_ENTER: {
				managerInput->isEnter = false;
				break;
			}
			}
			break;
		}
		}
	}
}