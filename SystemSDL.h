#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Menu.h"
#include "ManagerInput.h"
#include "Arcanoid.h"
#include <windows.h>


typedef struct GameWindow {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event event;
	int GW_width = 1600;
	int GW_height = 900;
} GameWindow;

void InitSDL(GameWindow* gWindow);
void mainLoop(GameWindow* gWindow);