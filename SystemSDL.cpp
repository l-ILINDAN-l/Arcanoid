#include "SystemSDL.h"
#include "ManagerInput.h"

#define FILESTARTD "./ButtonStartD.png"
#define FILESTARTND "./ButtonStartND.png"
#define FILEQUITD "./QuitButttonD.png"
#define FILEQUITND "./QuitButttonND.png"
#define FILEBACKGROUND "./background.png"
#define FILEALOGOARKANOID "./Arkanoid-logo.png"
#define FILEFIELD "./Fields_scaled_3x_pngcrushed.png"
#define FILETEXTURES "./Textures_scaled_4x_pngcrushed.png"
#define FILEHEART "./heart.png"
#define FILEGAMEOVER "./gameOver.png"
#define FILEWIN "./win.png"
#define FILEENTER "./PressEnter.png"

void InitSDL(GameWindow* gWindow) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)	printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
	IMG_Init(IMG_INIT_PNG);
	Uint32 flagsW = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL; //SDL_WINDOW_RESIZABLE
	gWindow->window = SDL_CreateWindow("Arkanoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gWindow->GW_width, gWindow->GW_height, flagsW);
	if (!gWindow->window) printf("SDL window not create! SDL Error: %s\n", SDL_GetError());
	gWindow->renderer = SDL_CreateRenderer(gWindow->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!gWindow->renderer) printf("SDL render not create! SDL Error: %s\n", SDL_GetError());
}

void mainLoop(GameWindow* gWindow) {
	ManagerInput mInput;
	SDL_Event event;
	ArcanoidState aState = AMenu;
	Uint32 lastTime = SDL_GetTicks();
	Menu menu;
	Game game;
	InitMenu(&menu, gWindow->window,
		FILESTARTD, FILESTARTND,
		FILEQUITD, FILEQUITND, 
		FILEBACKGROUND, FILEALOGOARKANOID, 
		&(gWindow->renderer)
	);
	
	InitArcanoid(&game, &(gWindow->renderer),
		FILEFIELD, FILETEXTURES,
		FILEBACKGROUND, FILEALOGOARKANOID,
		FILEHEART, FILEGAMEOVER,
		FILEWIN, FILEENTER);

	drawMenu(&menu, gWindow->window, 
		gWindow->GW_width, gWindow->GW_height, 
		&(gWindow->renderer)
	);
	while (true)
	{	
		updateManagerInput(&mInput, &event);
		if (mInput.isQuit) {
			SDL_Quit();
			IMG_Quit();
			break;
		}
		else if (aState == AMenu) {
			updateManagerInput(&mInput, &event);
			updateMenu(&menu, &mInput, &aState, gWindow->window, gWindow->GW_width, gWindow->GW_height, &(gWindow->renderer));
			if (aState == AGame) {
				generateBlocks(&game);
				game.gState = Start;
				game.count_lives = 3;
				game.ball.rectBall.x = FIELD_X_IN_GAME + FIELD_WIDTH / 2 - BALL_WIDTH / 2;
				game.ball.rectBall.y = FIELD_Y_IN_GAME + FIELD_HEIGHT - PADDLE_HEIGHT - 32 - BALL_HEIGHT;
				Sleep(200);
			}
		}
		else if (aState == AGame) {
			updateManagerInput(&mInput, &event);
			updateArcanoid(&game, &(gWindow->renderer), mInput, &aState);
			if (aState == AMenu) {
				menu.aButton = BStart;
				drawMenu(&menu, gWindow->window,
					gWindow->GW_width, gWindow->GW_height,
					&(gWindow->renderer)
				);
				
				updateManagerInput(&mInput, &event);
				Sleep(500);
			}
		}
	}

}