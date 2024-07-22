#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "ManagerInput.h"
#include "Logo.h"

typedef struct Button {
	SDL_Texture* textureNotDirect;
	SDL_Texture* textureDirect;
	SDL_Rect rectDisplay;
} Button;

enum ButtonActive {
	BStart, Quit
};
typedef struct Menu
{
	Button ButtonStart;
	Button ButtonQuit;
	SDL_Texture* BackGround;
	Logo logoArcanoid;
	ButtonActive aButton = BStart;
} Menu;

void InitMenu(Menu* menu, SDL_Window* window,
	const char* fileStartD, const char* fileStartND,
	const char* fileQuitD, const char* fileQuitND,
	const char* fileBackGround, const char* fileLogoArcanoid, 
	SDL_Renderer** renderer);
void drawMenu(Menu* menu, SDL_Window* window, 
	int GW_width, int GW_height, 
	SDL_Renderer** renderer);
void updateMenu(Menu* menu, ManagerInput* mInput,
	ArcanoidState* aState, SDL_Window* window, 
	int GW_width, int GW_height, 
	SDL_Renderer** renderer
);