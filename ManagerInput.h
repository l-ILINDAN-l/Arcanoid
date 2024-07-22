#pragma once
#include <SDL.h>
#define SDLK_ENTER 13
enum ArcanoidState
{
	AMenu, AGame
};
typedef struct ManagerInput
{
	bool isEscape = false;
	bool isQuit = false;
	bool isUp = false;
	bool isDown = false;
	bool isLeft = false;
	bool isRight = false;
	bool isEnter = false;
};

void clearManagerInput(ManagerInput* managerInput);
void updateManagerInput(ManagerInput* managerInput, SDL_Event* event);