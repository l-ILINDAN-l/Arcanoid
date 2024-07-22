#include "Menu.h"
void InitMenu(Menu* menu, SDL_Window* window,
	const char* fileStartD, const char* fileStartND,
	const char* fileQuitD, const char* fileQuitND,
	const char* fileBackGround, const char* fileLogoArcanoid, SDL_Renderer** renderer)
{	
	menu->BackGround = IMG_LoadTexture(*renderer, fileBackGround);
	menu->logoArcanoid.logo = IMG_LoadTexture(*renderer, fileLogoArcanoid);
	menu->ButtonStart.textureDirect = IMG_LoadTexture(*renderer, fileStartD);
	menu->ButtonStart.textureNotDirect = IMG_LoadTexture(*renderer, fileStartND);
	menu->ButtonQuit.textureDirect = IMG_LoadTexture(*renderer, fileQuitD);
	menu->ButtonQuit.textureNotDirect = IMG_LoadTexture(*renderer, fileQuitND);
	menu->logoArcanoid.rectDisplay = { 479,104,641,200 };
	menu->ButtonStart.rectDisplay = { 585,445,430,200 };
	menu->ButtonQuit.rectDisplay = { 600,650,400,168 };
	menu->aButton = BStart;
}


void drawMenu(Menu* menu, SDL_Window* window, int GW_width, int GW_height, SDL_Renderer** renderer) {
	SDL_RenderClear(*renderer);
	SDL_Rect BGRect = { 0, 0, GW_width, GW_height };
	SDL_RenderCopy(*renderer, menu->BackGround, NULL, &BGRect);
	SDL_RenderCopy(*renderer, menu->logoArcanoid.logo, NULL, &(menu->logoArcanoid.rectDisplay));
	if (menu->aButton == BStart) {
		SDL_RenderCopy(*renderer, menu->ButtonStart.textureDirect, NULL, &(menu->ButtonStart.rectDisplay));
		SDL_RenderCopy(*renderer, menu->ButtonQuit.textureNotDirect, NULL, &(menu->ButtonQuit.rectDisplay));
	}
	else {
		SDL_RenderCopy(*renderer, menu->ButtonStart.textureNotDirect, NULL, &(menu->ButtonStart.rectDisplay));
		SDL_RenderCopy(*renderer, menu->ButtonQuit.textureDirect, NULL, &(menu->ButtonQuit.rectDisplay));
	}
	SDL_RenderPresent(*renderer);
}

void updateMenu(Menu* menu, ManagerInput* mInput, ArcanoidState* aState, SDL_Window* window, int GW_width, int GW_height, SDL_Renderer** renderer) {
	if (!(mInput->isDown && mInput->isUp)) {
		if (mInput->isDown && menu->aButton == BStart) {
			menu->aButton = Quit;
			drawMenu(menu, window, GW_width, GW_height, renderer);
		}
		else if (mInput->isUp && menu->aButton == Quit) {
			menu->aButton = BStart;
			drawMenu(menu, window, GW_width, GW_height, renderer);
		}
	}
	if (mInput->isEnter) {
		if (menu->aButton == BStart) {
			*aState = AGame;
		}
		else {
			mInput->isQuit = true;
		}
	}
}
