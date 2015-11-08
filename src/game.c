#include "game.h"

#include <stdlib.h>
#include "board.h"
#include "fileio.h"
#include "font.h"
#include "hiscore.h"
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
scoreEntry hiscoreEntry;
int enteringHiscore;
int scoreCursorPos;

void gameUnload()
{
	storeBoard();
	boardUnload();
	gameTime = 0;
}

void gameLoad()
{
	boardLoad();
	enteringHiscore = 0;
}

void gameLogic()
{
	if (enteringHiscore)
	{
		if (keys[KEY_BACK])
		{
			keys[KEY_BACK] = 0;
		}

		if (keys[KEY_LEFT])
		{
			if (--canMoveX <= 0)
			{
				canMoveX = KEY_DELAY;
				--scoreCursorPos;
			}
		}
		else if (keys[KEY_RIGHT])
		{
			if (--canMoveX <= 0)
			{
				canMoveX = KEY_DELAY;
				++scoreCursorPos;
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
				++hiscoreEntry.name[scoreCursorPos];
			}

			if ((hiscoreEntry.name[scoreCursorPos] > ' ') && (hiscoreEntry.name[scoreCursorPos] < 'A'))
			{
				hiscoreEntry.name[scoreCursorPos] = 'A';
			}
			else if ((hiscoreEntry.name[scoreCursorPos] > 'Z') && (hiscoreEntry.name[scoreCursorPos] < 'a'))
			{
				hiscoreEntry.name[scoreCursorPos] = 'a';
			}
			else if (hiscoreEntry.name[scoreCursorPos] > 'z')
			{
				hiscoreEntry.name[scoreCursorPos] = ' ';
			}
		}
		else if (keys[KEY_DOWN])
		{
			if (--canMoveY <= 0)
			{
				canMoveY = KEY_DELAY;
				--hiscoreEntry.name[scoreCursorPos];
			}

			if (hiscoreEntry.name[scoreCursorPos] < ' ')
			{
				hiscoreEntry.name[scoreCursorPos] = 'z';
			}
			else if ((hiscoreEntry.name[scoreCursorPos] > ' ') && (hiscoreEntry.name[scoreCursorPos] < 'A'))
			{
				hiscoreEntry.name[scoreCursorPos] = ' ';
			}
			else if ((hiscoreEntry.name[scoreCursorPos] > 'Z') && (hiscoreEntry.name[scoreCursorPos] < 'a'))
			{
				hiscoreEntry.name[scoreCursorPos] = 'Z';
			}
		}
		else
		{
			canMoveY = 0;
		}

		if (keys[KEY_OK])
		{
			hiscoreEntry.time = gameTime;
			hiscoreAddRecord(&hiscoreEntry, &currentGameMode, &currentAlgorithm);
			storeHiscore();
			programStateNew = STATE_HISCORE;
		}

		if (keys[KEY_CANCEL])
		{
			keys[KEY_CANCEL] = 0;
		}

		if (scoreCursorPos < 0)
		{
			scoreCursorPos = 0;
		}
		else if (scoreCursorPos >= SCORE_NAME_LEN - 1)
		{
			scoreCursorPos = SCORE_NAME_LEN - 2;
		}
	}
	else
	{
		if (keys[KEY_BACK])
		{
			keys[KEY_BACK] = 0;

			if (gameOver)
			{
				if (stonesLeft)
				{
					programStateNew = STATE_TITLE;
				}
				else
				{
					int i;
					enteringHiscore = 1;
					scoreCursorPos = 0;

					for (i = 0; i < SCORE_NAME_LEN - 1; ++i)
					{
						hiscoreEntry.name[i] = ' ';
					}

					hiscoreEntry.name[scoreCursorPos] = 'A';
					hiscoreEntry.name[SCORE_NAME_LEN-1] = '\n';
				}
			}
			else
			{
				programStateNew = STATE_TITLE;
			}
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
			keys[KEY_OK] = 0;

			if (gameOver)
			{
				if (stonesLeft)
				{
					programStateNew = STATE_TITLE;
				}
				else
				{
					int i;
					enteringHiscore = 1;
					scoreCursorPos = 0;

					for (i = 0; i < SCORE_NAME_LEN - 1; ++i)
					{
						hiscoreEntry.name[i] = ' ';
					}

					hiscoreEntry.name[scoreCursorPos] = 'A';
					hiscoreEntry.name[SCORE_NAME_LEN-1] = '\n';
				}
			}
			else
			{
				boardSelectStone(cursorX, cursorY);
			}
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
		if (enteringHiscore)
		{
			int i;
			char txtHiscore[SCORE_NAME_LEN];

			strcpy(txtHiscore, hiscoreEntry.name);

			dTextCentered(&gameFontRegular, "Enter your name:", 40, SHADOW_OUTLINE);

			dTextCentered(&gameFontRegular, txtHiscore, SCREEN_H/2 - (gameFontRegular.h + gameFontRegular.leading)/2, SHADOW_OUTLINE);

			for (i = 0; i < SCORE_NAME_LEN - 1; ++i)
			{
				txtHiscore[i] = (i == scoreCursorPos) ? (hiscoreEntry.name[scoreCursorPos] == ' ' ? '_' : hiscoreEntry.name[scoreCursorPos]) : ' ';
			}

			dTextCentered(&gameFontSelected, txtHiscore, SCREEN_H/2 - (gameFontSelected.h + gameFontSelected.leading)/2, SHADOW_OUTLINE);
		}
		else
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
	}

	snprintf(txtTime, 15, "Time: %lu:%02lu:%02lu", gameTime/FPS/3600 > 99 ? 99 : gameTime/FPS/3600, gameTime/FPS/60%60, gameTime/FPS%60);
	sprintf(txtStones, "Stones Left: %d", stonesLeft);
	sprintf(txtBottomBar, "%s  Stones Left: %d", txtTime, stonesLeft);

	txtPositionY = SCREEN_H - (gameFontShadow.h + gameFontShadow.leading);

	if (gameOver && !stonesLeft && !enteringHiscore)
	{
		dTextCentered(&gameFontRegular, txtGameOver, SCREEN_H/2 - (gameFontRegular.h + gameFontRegular.leading)/2, SHADOW_OUTLINE);
	}
	dTextCentered(&gameFontShadow, (gameOver && stonesLeft) ? txtGameOver : txtBottomBar, txtPositionY, SHADOW_NONE);
}

void gameDraw()
{
	boardDraw();
	gameGuiDraw();
}
