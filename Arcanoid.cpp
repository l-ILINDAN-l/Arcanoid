#include "Arcanoid.h"

SDL_Texture* loadTexture(const char* filename, SDL_Renderer** renderer, SDL_Rect rect) {
	SDL_Surface* surface = IMG_Load(filename); // Загрузка изображения
	if (!surface) {
		// Обработка ошибки, если изображение не загружено
		SDL_Log("Не удалось загрузить изображение: %s", IMG_GetError());
		return nullptr;
	}

	// Создание текстуры из поверхности
	SDL_Texture* texture = SDL_CreateTextureFromSurface(*renderer, surface);
	SDL_FreeSurface(surface); // Освобождение поверхности, так как она больше не нужна
	if (!texture) {
		// Обработка ошибки, если текстура не создана
		SDL_Log("Не удалось создать текстуру: %s", SDL_GetError());
		return nullptr;
	}

	// Обрезаем текстуру по указанному прямоугольнику
	SDL_Texture* croppedTexture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
	SDL_SetRenderTarget(*renderer, croppedTexture);
	SDL_Rect cropRect = { 0, 0, rect.w, rect.h };
	SDL_RenderCopy(*renderer, texture, &rect, &cropRect);
	SDL_SetRenderTarget(*renderer, NULL);

	// Освобождаем исходную текстуру
	SDL_DestroyTexture(texture);

	return croppedTexture;
}

#define BALL_X_IN_TEXTURES 1536
#define BALL_Y_IN_TEXTURES 288
#define PADDLE_X_IN_TEXTURES 0
#define PADDLE_Y_IN_TEXTURES 384


void loadTextures(Game* game, SDL_Renderer** renderer,
	const char* fileField, const char* fileTextures,
	const char* fileBackGround, const char* fileLogoArcanoid,
	const char* fileHeart, const char* fileGameOver,
	const char* fileWin, const char* filePressEnter)
{
	game->textures.logo = IMG_LoadTexture(*renderer, fileLogoArcanoid);
	game->textures.backGround = IMG_LoadTexture(*renderer, fileBackGround);
	game->textures.field = loadTexture(fileField, renderer, { 0, 0, game->field.rectField.w,  game->field.rectField.h });
	for (int ind = 0; ind < NUM_BLOCKS_TEXTURE; ind++) {
		game->textures.blocks[ind] = loadTexture(fileTextures, renderer, { BLOCK_WIDTH * ind, 0, BLOCK_WIDTH, BLOCK_HEIGHT });
	}
	game->textures.ball = loadTexture(fileTextures, renderer, { BALL_X_IN_TEXTURES, BALL_Y_IN_TEXTURES, game->ball.rectBall.w, game->ball.rectBall.h });
	game->textures.life = IMG_LoadTexture(*renderer, fileHeart);
	game->textures.paddle = loadTexture(fileTextures, renderer, { PADDLE_X_IN_TEXTURES, PADDLE_Y_IN_TEXTURES, game->paddle.rectPaddle.w, game->paddle.rectPaddle.h });
	game->textures.gameOver = IMG_LoadTexture(*renderer, fileGameOver);
	game->textures.pressEnter = IMG_LoadTexture(*renderer, filePressEnter);
	game->textures.win = IMG_LoadTexture(*renderer, fileWin);
}

void generateBlocks(Game* game) {
	int upperLeftCornerX = game->field.rectField.x + (game->field.rectField.w - BLOCK_WIDTH * NUM_BLOCKS_WIDTH)/2;
	int upperLeftCornerY = game->field.rectField.y + 32;
	for (int indH = 0; indH < NUM_BLOCKS_HEIGHT; indH++) {
		for (int indW = 0; indW < NUM_BLOCKS_WIDTH; indW++) {
			game->blocks[indH][indW].isAlive = 1;
			game->blocks[indH][indW].rectBlock.x = upperLeftCornerX + BLOCK_WIDTH * indW;
			game->blocks[indH][indW].rectBlock.y = upperLeftCornerY + BLOCK_HEIGHT * indH;
			game->blocks[indH][indW].texture = game->textures.blocks[(rand() % NUM_BLOCKS_TEXTURE)];
		}
	}
}

void InitArcanoid(Game* game, SDL_Renderer** renderer,
	const char* fileField, const char* fileTextures,
	const char* fileBackGround, const char* fileLogoArcanoid,
	const char* fileHeart, const char* fileGameOver,
	const char* fileWin, const char* filePressEnter)
{
	loadTextures(game, renderer,
		fileField, fileTextures,
		fileBackGround, fileLogoArcanoid,
		fileHeart, fileGameOver,
		fileWin, filePressEnter
	);
}

void updateArcanoid(Game* game, SDL_Renderer** renderer, ManagerInput mInput, ArcanoidState* aState) {
	SDL_RenderClear(*renderer);
	SDL_RenderCopy(*renderer, game->textures.backGround, NULL, &game->backGround.rectBG);
	SDL_RenderCopy(*renderer, game->textures.field, NULL, &game->field.rectField);
	SDL_RenderCopy(*renderer, game->textures.logo, NULL, &game->logo.rectLogo);
	if (game->gState == Start) {
		game->ball.rectBall.x = FIELD_X_IN_GAME + FIELD_WIDTH / 2 - BALL_WIDTH / 2;
		game->ball.rectBall.y = FIELD_Y_IN_GAME + FIELD_HEIGHT - PADDLE_HEIGHT - 32 - BALL_HEIGHT;
		game->ball.dx = BALL_VEL_X;
		game->ball.dy = BALL_VEL_Y;
		game->paddle.rectPaddle.x = FIELD_X_IN_GAME + FIELD_WIDTH / 2 - PADDLE_WIDTH / 2;
		game->paddle.rectPaddle.y = FIELD_Y_IN_GAME + FIELD_HEIGHT - PADDLE_HEIGHT - 32;
		for (int indH = 0; indH < NUM_BLOCKS_HEIGHT; indH++) {
			for (int indW = 0; indW < NUM_BLOCKS_WIDTH; indW++) {
				SDL_RenderCopy(*renderer, game->blocks[indH][indW].texture, NULL, &game->blocks[indH][indW].rectBlock);
			}
		}
		for (int count = 0; count < game->count_lives; count++) {
			SDL_Rect rectCurrentLive{ game->lives.x + game->lives.width * count, game->lives.y,  game->lives.width, game->lives.height };
			SDL_RenderCopy(*renderer, game->textures.life, NULL, &rectCurrentLive);
		}
		SDL_RenderCopy(*renderer, game->textures.ball, NULL, &game->ball.rectBall);
		SDL_RenderCopy(*renderer, game->textures.paddle, NULL, &game->paddle.rectPaddle);
		SDL_RenderCopy(*renderer, game->textures.pressEnter, NULL, &game->pressEnter);
		SDL_RenderPresent(*renderer);
		if (mInput.isEnter || mInput.isDown || mInput.isUp) {
			game->gState = GameProcess;
		}
		else if (mInput.isRight) {
			game->paddle.rectPaddle.x += game->paddle.velocity;
			game->ball.dx += (game->paddle.velocity - 2);
			game->gState = GameProcess;
		}
		else if (mInput.isLeft) {
			game->paddle.rectPaddle.x -= game->paddle.velocity;
			game->ball.dx -= (game->paddle.velocity - 2);
			game->gState = GameProcess;
		}
	}
	else if (game->gState == GameOver) {
		SDL_RenderCopy(*renderer, game->textures.gameOver, NULL, &game->gameOver);
		SDL_RenderPresent(*renderer);
		if (mInput.isEnter || mInput.isDown || mInput.isUp || mInput.isRight || mInput.isLeft){
			*aState = AMenu;
		}
		return;
	}
	else if (game->gState == Launch) {
		game->ball.dx = BALL_VEL_X;
		game->ball.dy = BALL_VEL_Y;
		game->ball.rectBall.x = FIELD_X_IN_GAME + FIELD_WIDTH / 2 - BALL_WIDTH / 2;
		game->ball.rectBall.y = FIELD_Y_IN_GAME + FIELD_HEIGHT - PADDLE_HEIGHT - 32 - BALL_HEIGHT;
		game->paddle.rectPaddle.x = FIELD_X_IN_GAME + FIELD_WIDTH / 2 - PADDLE_WIDTH / 2;
		game->paddle.rectPaddle.y = FIELD_Y_IN_GAME + FIELD_HEIGHT - PADDLE_HEIGHT - 32;

		for (int indH = 0; indH < NUM_BLOCKS_HEIGHT; indH++) {
			for (int indW = 0; indW < NUM_BLOCKS_WIDTH; indW++) {
				if (game->blocks[indH][indW].isAlive) {
					SDL_RenderCopy(*renderer, game->blocks[indH][indW].texture, NULL, &game->blocks[indH][indW].rectBlock);
				}
			}
			game->ball.dx = 0;
			game->ball.dy = -abs(game->ball.dy);
		}
		SDL_RenderCopy(*renderer, game->textures.ball, NULL, &game->ball.rectBall);
		SDL_RenderCopy(*renderer, game->textures.paddle, NULL, &game->paddle.rectPaddle);
		for (int count = 0; count < game->count_lives; count++) {
			SDL_Rect rectCurrentLive{ game->lives.x + game->lives.width * count, game->lives.y,  game->lives.width, game->lives.height };
			SDL_RenderCopy(*renderer, game->textures.life, NULL, &rectCurrentLive);
		}
		SDL_RenderPresent(*renderer);

		if (mInput.isEnter || mInput.isDown || mInput.isUp) {
			game->gState = GameProcess;
		}
		else if (mInput.isRight) {
			game->paddle.rectPaddle.x += game->paddle.velocity;
			game->ball.dx += game->paddle.velocity % 3;
			if (game->ball.dx > 4) {
				game->ball.dx = 4;
			}
			game->gState = GameProcess;
		}
		else if (mInput.isLeft) {
			game->paddle.rectPaddle.x -= game->paddle.velocity;
			game->ball.dx -= game->paddle.velocity % 4;
			if (game->ball.dx < -4) {
				game->ball.dx = -4;
			}
			game->gState = GameProcess;
		}
	}
	else if (game->gState == Win) {
		SDL_RenderCopy(*renderer, game->textures.win, NULL, &game->win);
		SDL_RenderPresent(*renderer);
		if (mInput.isEnter || mInput.isDown || mInput.isUp || mInput.isRight || mInput.isLeft) {
			game->gState = Start;
			*aState = AMenu;											  
		}
	}
	else if (game->gState == GameProcess) {
		if (game->ball.dx < -10) {
			game->ball.dx = -10;
		}
		else if (game->ball.dx > 10) {
			game->ball.dx = 10;
		}
		for (int count = 0; count < game->count_lives; count++) {
			SDL_Rect rectCurrentLive{ game->lives.x + game->lives.width * count, game->lives.y,  game->lives.width, game->lives.height };
			SDL_RenderCopy(*renderer, game->textures.life, NULL, &rectCurrentLive);
		}
		int countAliveBlock = 0;
		SDL_Rect opreationRect = game->ball.rectBall;
		game->ball.rectBall.x += game->ball.dx;
		game->ball.rectBall.y += game->ball.dy;
		SDL_RenderCopy(*renderer, game->textures.ball, NULL, &game->ball.rectBall);
		if (mInput.isRight && (game->paddle.rectPaddle.x + game->paddle.rectPaddle.w) < (game->field.rectField.x + game->field.rectField.w - 24)) {
			game->paddle.rectPaddle.x += game->paddle.velocity;
		}
		else if (mInput.isLeft && game->paddle.rectPaddle.x > (game->field.rectField.y + 24)) {
			game->paddle.rectPaddle.x -= game->paddle.velocity;
		}
		SDL_RenderCopy(*renderer, game->textures.paddle, NULL, &game->paddle.rectPaddle);
		if (((game->field.rectField.x + 24) >= game->ball.rectBall.x) || ((game->field.rectField.x + game->field.rectField.w - 24) <= (game->ball.rectBall.x + game->ball.rectBall.w))) {
			game->ball.dx = -game->ball.dx;
		}
		if ((game->field.rectField.y + 24) >= game->ball.rectBall.y) {
			game->ball.dy = -game->ball.dy;
		}
		if ((game->field.rectField.y + game->field.rectField.h - 24) <= (game->ball.rectBall.y + game->ball.rectBall.h)) {
			game->count_lives -= 1;
			if (game->count_lives > 0) game->gState = Launch;
			else game->gState = GameOver;
			return;
		}
		for (int indH = 0; indH < NUM_BLOCKS_HEIGHT; indH++) {
			for (int indW = 0; indW < NUM_BLOCKS_WIDTH; indW++) {
				if (game->blocks[indH][indW].isAlive) {
					SDL_Rect opreationRectdx = opreationRect;
					opreationRectdx.x += game->ball.dx;
					SDL_Rect opreationRectdy = opreationRect;
					opreationRectdy.y += game->ball.dy;
					if (SDL_HasIntersection(&game->blocks[indH][indW].rectBlock, &opreationRectdx)
						&& SDL_HasIntersection(&game->blocks[indH][indW].rectBlock, &opreationRectdy)) {
						game->blocks[indH][indW].isAlive = 0;
						game->ball.dy = -game->ball.dy;
						game->ball.dx = -game->ball.dx;
					}
					else if (SDL_HasIntersection(&game->blocks[indH][indW].rectBlock, &opreationRectdx)) {
						game->blocks[indH][indW].isAlive = 0;
						game->ball.dx = -game->ball.dx;
					}
					else if (SDL_HasIntersection(&game->blocks[indH][indW].rectBlock, &opreationRectdy)) {
						game->blocks[indH][indW].isAlive = 0;
						game->ball.dy = -game->ball.dy;
					}
					else {
						SDL_RenderCopy(*renderer, game->blocks[indH][indW].texture, NULL, &game->blocks[indH][indW].rectBlock);
						countAliveBlock += 1;
					}
				}
			}
		}
		if (!countAliveBlock) {
			game->gState = Win;
			return;
		}
	

		if (SDL_HasIntersection(&game->paddle.rectPaddle, &game->ball.rectBall) ) {
			if (mInput.isLeft) {
				game->ball.dx -= 2;
				if (game->ball.dx < -10) {
					game->ball.dx = -10;
				}
			}
			else if (mInput.isRight) {
				game->ball.dx += 2;
				if (game->ball.dx > 10) {
					game->ball.dx = 10;
				}
			}
			game->ball.dy = -game->ball.dy;
		}

		SDL_RenderPresent(*renderer);
	}
}

//void InitArcanoid(Arcanoid* arcanoid, SDL_Window* window, SDL_Renderer** renderer, 
//	char* fileField, char* fileTextures, char* fileBackGround, char* fileLogoArcanoid) {
//	arcanoid->sGArcanoid = beginningGame;
//	
//	arcanoid->BackGround = IMG_LoadTexture(*renderer, fileBackGround);
//	
//	arcanoid->logoArcanoid.logo = IMG_LoadTexture(*renderer, fileLogoArcanoid);
//	arcanoid->logoArcanoid.rectDisplay = { 813, 90, 641, 200 };
//	
//	for (int ind = 0; ind < 7; ind++) {
//		arcanoid->BlocksTexture[ind] = loadTexture("Textures_scaled_4x_pngcrushed.png", *renderer, { 64*ind, 0, 64, 32 });
// 	}
//
//	for (int ind = 0; ind < 7; ind++) {
//		if (ind < 5) {
//			arcanoid->FieldsTexture[ind] = loadTexture("Fields_scaled_3x_pngcrushed.png", *renderer, { (696*ind), 0, 672, 720});
//		}
//		else {
//			arcanoid->FieldsTexture[ind] = loadTexture("Fields_scaled_3x_pngcrushed.png", *renderer, { (696 * (ind - 5)), 768, 672, 720 });
//		}
//	}
//	
//
//	arcanoid->field.texture = arcanoid->FieldsTexture[0];
//	arcanoid->field.rectField = { 60, 60, 672, 720 };
//	arcanoid->field.locLeftWall = arcanoid->field.rectField.x + 23;
//	arcanoid->field.locUpperWall = arcanoid->field.rectField.y + 23;
//	arcanoid->field.locRightWall = arcanoid->field.rectField.x + 648;
//	arcanoid->field.locLowerWall = arcanoid->field.rectField.y + 720;
//
//	arcanoid->racket.texture = loadTexture("Textures_scaled_4x_pngcrushed.png", *renderer, { 0, 288, 64, 32 });
//	arcanoid->racket.velocity = 30;
//	arcanoid->racket.rectRacket = { arcanoid->field.rectField.x + 292, arcanoid->field.rectField.y + 668, 64, 32 };
//
//	arcanoid->ball.texture = loadTexture("Textures_scaled_4x_pngcrushed.png", *renderer, { 1536, 288, 20, 20 });
//	arcanoid->ball.velocity = 20;
//	arcanoid->ball.angel = M_PI * 3 / 2 + M_PI * (((float)(rand() % 10) - 5) / 100);
//	arcanoid->ball.rectBall = { arcanoid->racket.rectRacket.x + 22, arcanoid->racket.rectRacket.y - 20, 20, 20 };
//
//
//}

//void drawArcanoid(Arcanoid* arcanoid, SDL_Window* window, SDL_Renderer** renderer, int GW_width, int GW_height) {
//	SDL_RenderClear(*renderer);
//	SDL_Rect BGRect = { 0, 0, GW_width, GW_height };
//	SDL_RenderCopy(*renderer, arcanoid->BackGround, NULL, &BGRect);
//	SDL_RenderCopy(*renderer, arcanoid->logoArcanoid.logo, NULL, &(arcanoid->logoArcanoid.rectDisplay));
//	SDL_RenderCopy(*renderer, arcanoid->field.texture, NULL, &(arcanoid->field.rectField));
//
//	for (size_t ind = 0; ind < arcanoid->lenBlocks; ind++) {
//		SDL_RenderCopy(*renderer, arcanoid->blocks[ind].texture, NULL, &(arcanoid->blocks[ind].rectBlock));
//	}
//	SDL_RenderCopy(*renderer, arcanoid->ball.texture, NULL, &(arcanoid->ball.rectBall));
//	SDL_RenderCopy(*renderer, arcanoid->racket.texture, NULL, &(arcanoid->racket.rectRacket));
//	SDL_RenderCopy(*renderer, arcanoid->lives.textureLeftSideStrip, NULL, &(arcanoid->lives.rectLeftSideStrip));
//	SDL_RenderCopy(*renderer, arcanoid->lives.textureRightSideStrip, NULL, &(arcanoid->lives.rectRightSideStrip));
//	if (arcanoid->lives.count != 0) {
//		for (int count = 0; count < (arcanoid->lives.count <= 3? arcanoid->lives.count: 3) - 1; count++) {
//			SDL_Rect rectMiddlePart{ arcanoid->lives.rectLeftSideStrip.x + arcanoid->lives.rectLeftSideStrip.w +arcanoid->lives.sizeMiddlePart.w*count,
//			arcanoid->lives.rectLeftSideStrip.y, arcanoid->lives.sizeMiddlePart.w, arcanoid->lives.sizeMiddlePart.h};
//			SDL_RenderCopy(*renderer, arcanoid->lives.textureMiddlePart, NULL, &rectMiddlePart);
//		}
//		SDL_Rect rectEndStrip{ arcanoid->lives.rectLeftSideStrip.x +
//			arcanoid->lives.rectLeftSideStrip.w +
//			arcanoid->lives.sizeMiddlePart.w * ((arcanoid->lives.count <= 3 ? arcanoid->lives.count : 3) - 1),
//				arcanoid->lives.rectLeftSideStrip.y, arcanoid->lives.sizeEndStrip.w, arcanoid->lives.sizeEndStrip.h };
//		SDL_RenderCopy(*renderer, arcanoid->lives.textureEndStrip, NULL, &rectEndStrip);
//	}
//	for (int countExtra = 0; countExtra < arcanoid->lives.count - 3; countExtra++) {
//		SDL_RenderCopy(*renderer, arcanoid->lives.extraLives[countExtra].textureExtraLife, NULL, &(arcanoid->lives.extraLives[countExtra].rectExtraLife));
//	}
//	switch (arcanoid->sGArcanoid)
//	{
//	case beginningGame:
//		break;
//	case defeat:
//		break;
//	case win:
//		break;
//	default:
//		break;
//	}
//	SDL_RenderPresent(*renderer);
//}
//
//void updateGame(Arcanoid* arcanoid, SDL_Window* window, int GW_width, int GW_height, 
//	SDL_Renderer** renderer, Uint32 last_time) {
//	
//
//}