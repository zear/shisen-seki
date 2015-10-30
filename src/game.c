#include "game.h"

#include <stdlib.h>
#include "board.h"
#include "fileio.h"
#include "font.h"
#include "input.h"
#include "main.h"
#include "states.h"
#include "video.h"

int continueGame;
gameMode newGameMode = GAME_MODE_CLASSIC;
gameMode currentGameMode;
int canMoveX;
int canMoveY;
unsigned long gameTime;
int gameOver;

void gameUnload()
{
	storeBoard();
	boardUnload();
	gameTime = 0;
}

void gameLoad()
{
	boardLoad();
}

void gameLogic()
{
	if (keys[KEY_BACK])
	{
		keys[KEY_BACK] = 0;
		programStateNew = STATE_TITLE;
	}
	if (keys[KEY_LEFT])
	{
		if (--canMoveX <= 0)
		{
			canMoveX = KEY_DELAY;
			--cursorX;
		}
	}
	else if (keys[KEY_RIGHT])
	{
		if (--canMoveX <= 0)
		{
			canMoveX = KEY_DELAY;
			++cursorX;
		}
	}
	else
	{
		canMoveX = 0;
	}

	if (keys[KEY_UP])
	{
		if (--canMoveY <= 0)
		{
			canMoveY = KEY_DELAY;
			--cursorY;
		}
	}
	else if (keys[KEY_DOWN])
	{
		if (--canMoveY <= 0)
		{
			canMoveY = KEY_DELAY;
			++cursorY;
		}
	}
	else
	{
		canMoveY = 0;
	}

	if (keys[KEY_OK])
	{
		boardSelectStone(cursorX, cursorY);
		keys[KEY_OK] = 0;
	}

	if (keys[KEY_CANCEL])
	{
		stoneA.type = STONE_EMPTY;
		keys[KEY_CANCEL] = 0;
	}

	if (cursorX < 1)
	{
		cursorX = BOARD_W - 2;
	}
	if (cursorX >= BOARD_W - 1)
	{
		cursorX = 1;
	}
	if (cursorY < 1)
	{
		cursorY = BOARD_H - 2;
	}
	if (cursorY >= BOARD_H - 1)
	{
		cursorY = 1;
	}

	if (!gameOver)
	{
		++gameTime;
	}
}

void gameGuiDraw()
{
	char txtTime[50];
	char txtStones[50];
	char txtGameOver[50];
	char txtBottomBar[100];
	int txtPositionY;

	if (gameOver)
	{
		if (stonesLeft)
		{
			sprintf(txtGameOver, "No moves left!");
		}
		else
		{
			sprintf(txtGameOver, "Congratulations!");
		}
	}
	sprintf(txtTime, "Time: %02lu:%02lu", gameTime/FPS/60, gameTime/FPS%60);
	sprintf(txtStones, "Stones Left: %d", stonesLeft);
	sprintf(txtBottomBar, "Time: %02lu:%02lu  Stones Left: %d", gameTime/FPS/60, gameTime/FPS%60, stonesLeft);

	txtPositionY = SCREEN_H - (gameFont.h + gameFont.leading);

	if (gameOver && !stonesLeft)
	{
		dTextCentered(&gameFont, txtGameOver, SCREEN_H/2 - (gameFont.h + gameFont.leading)/2);
	}
	dTextCentered(&gameFont, txtBottomBar, txtPositionY);
}

void gameDraw()
{
	boardDraw();
	gameGuiDraw();
}
