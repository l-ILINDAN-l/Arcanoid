#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "ManagerInput.h"
#include <stdlib.h>


#define NUM_BLOCKS_TEXTURE 7
#define NUM_LIVES 3
#define NUM_BLOCKS_WIDTH 8
#define NUM_BLOCKS_HEIGHT 9

//typedef struct Texture {
//    SDL_Rect srcRect;
//    SDL_Rect destRect;
//} Texture;

enum GameArcanoidState {
    Start,
    Win,
    GameOver,
    Launch,
    GameProcess
};

typedef struct Textures {
    SDL_Texture* paddle;
    SDL_Texture* ball;
    SDL_Texture* blocks[NUM_BLOCKS_TEXTURE];
    SDL_Texture* life;
    SDL_Texture* backGround;
    SDL_Texture* field;
    SDL_Texture* logo;
    SDL_Texture* win;
    SDL_Texture* gameOver;
    SDL_Texture* pressEnter;
} Textures;

#define BLOCK_WIDTH 64
#define BLOCK_HEIGHT 32

typedef struct Block {
    int width = BLOCK_WIDTH;
    int height = BLOCK_HEIGHT;
    SDL_Rect rectBlock{ 60, 60, BLOCK_WIDTH, BLOCK_HEIGHT };
    int isAlive = 1;
    SDL_Texture* texture;
} Block;

#define BACKGROUND_WIDTH 1600
#define BACKGROUND_HEIGHT 900

typedef struct BackGround {
    SDL_Rect rectBG{ 0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT };
} BackGround;

#define FIELD_WIDTH 672
#define FIELD_HEIGHT 720
#define FIELD_X_IN_GAME 60
#define FIELD_Y_IN_GAME 60

typedef struct Field {
    SDL_Rect rectField{ FIELD_X_IN_GAME , FIELD_Y_IN_GAME , FIELD_WIDTH , FIELD_HEIGHT };
} Field;

#define PADDLE_WIDTH 160
#define PADDLE_HEIGHT 32


typedef struct Paddle {
    SDL_Rect rectPaddle{ FIELD_X_IN_GAME + FIELD_WIDTH / 2 - PADDLE_WIDTH / 2, 
        FIELD_Y_IN_GAME + FIELD_HEIGHT - PADDLE_HEIGHT - 32 ,
        PADDLE_WIDTH , PADDLE_HEIGHT };
    int velocity = 6;
} Paddle;

#define BALL_WIDTH 20
#define BALL_HEIGHT 20
#define BALL_VEL_X 0
#define BALL_VEL_Y -3

typedef struct Ball {
    SDL_Rect rectBall{ FIELD_X_IN_GAME + FIELD_WIDTH / 2 - BALL_WIDTH / 2,
        FIELD_Y_IN_GAME + FIELD_HEIGHT - PADDLE_HEIGHT - 32 - BALL_HEIGHT,
        BALL_WIDTH, BALL_HEIGHT };
    int dx = BALL_VEL_X;
    int dy = BALL_VEL_Y;
} Ball;

#define LOGO_X_IN_GAME (FIELD_X_IN_GAME+FIELD_WIDTH+60)
#define LOGO_Y_IN_GAME (FIELD_Y_IN_GAME)
#define LOGO_WIDTH 641
#define LOGO_HEIGHT 200

typedef struct GLogo {
    SDL_Rect rectLogo{ LOGO_X_IN_GAME , LOGO_Y_IN_GAME, LOGO_WIDTH, LOGO_HEIGHT };
} GLogo;

#define LIVES_WIDTH 50
#define LIVES_HEIGHT 50

typedef struct Lives
{
    //x and y first heart
    int x = LOGO_X_IN_GAME;
    int y = LOGO_Y_IN_GAME + LOGO_HEIGHT + 50;
    int width = LIVES_WIDTH;
    int height = LIVES_HEIGHT;
} Lives;

typedef struct Game {
    GameArcanoidState gState = Start;
    Textures textures;
    Ball ball;
    Paddle paddle;
    GLogo logo;
    Lives lives;
    Field field;
    BackGround backGround;
    Block blocks[NUM_BLOCKS_HEIGHT][NUM_BLOCKS_WIDTH];
    int count_lives = 3;
    SDL_Rect pressEnter{ 390, 320, 820, 260 };
    SDL_Rect win{ 370, 209, 860, 481};
    SDL_Rect gameOver{ 300, 132, 1000, 563 };
} Game;

void InitArcanoid(Game* game, SDL_Renderer** renderer,
    const char* fileField, const char* fileTextures,
    const char* fileBackGround, const char* fileLogoArcanoid,
    const char* fileHeart, const char* fileGameOver,
    const char* fileWin, const char* filePressEnter);

void generateBlocks(Game* game);
void updateArcanoid(Game* game, SDL_Renderer** renderer, ManagerInput mInput, ArcanoidState* aState);