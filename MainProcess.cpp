#include <windows.h>
#include "MainProcess.h"
#include <SDL.h>
int main(int argc, char* argv[]) {
	GameWindow gWindow;
	InitSDL(&gWindow);
	mainLoop(&gWindow);
	return 0;
}