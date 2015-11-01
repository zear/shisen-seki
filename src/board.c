#include "board.h"

#include <stdlib.h>
#include "fileio.h"
#include "font.h"
#include "game.h"
#include "stone.h"
#include "tileset.h"
#include "title.h"
#include "video.h"

algorithm currentAlgorithm = ALGO_REVERSE;
tileset stonesTileset;
stoneType **stones;
stone stoneA;
stone stoneB;
line lineC;
int stonesLeft;
int crossing;
int cursorX = 1;
int cursorY = 1;
int generating;
int checkingMoves;

int boardStoneSurrounded(stone *st)
{
	int n;
	int e;
	int s;
	int w;

	if (!st)
	{
		return -1;
	}

	// Check board boundaries.
	n = (st->y <= 2) ? 1 : stones[st->x][st->y-1];
	e = (st->x >= BOARD_W - 2) ? 1 : stones[st->x+1][st->y];
	s = (st->y >= BOARD_H - 2) ? 1 : stones[st->x][st->y+1];
	w = (st->x <= 2) ? 1 : stones[st->x-1][st->y];

	return (n && e && s && w) ? 1 : 0;
}

int boardCheckAvailableMoves()
{
	int i;
	int j;
	int k;
	int l;

	if (!stones)
	{
		return 0;
	}

	checkingMoves = 1;

	for (i = 1; i < BOARD_W - 1; ++i)
	{
		for (j = 1; j < BOARD_H - 1; ++j)
		{
			int stoneNum;
			stone stoneA;

			stoneNum = stones[i][j];

			if (stoneNum == STONE_EMPTY)
			{
				continue;
			}

			stoneA.x = i;
			stoneA.y = j;
			stoneA.type = stoneNum;

			for (k = 1; k < BOARD_W - 1; ++k)
			{
				for (l = 1; l < BOARD_H - 1; ++l)
				{
					stone stoneB;

					if (stones[k][l] != stoneNum)
					{
						continue;
					}

					stoneB.x = k;
					stoneB.y = l;
					stoneB.type = stoneNum;

					if (boardCheckConnection(&stoneA, &stoneB))
					{
						stones[i][j] = stoneNum;
						stones[k][l] = stoneNum;

						checkingMoves = 0;
						return 1;
					}
				}
			}
		}
	}

	checkingMoves = 0;
	return 0;
}

void boardApplyGravity()
{
	int i;
	int j;

	if (!stones)
	{
		return;
	}

	for (i = 1; i < BOARD_W - 1; ++i)
	{
		for (j = BOARD_H - 3; j > 0; --j)
		{
			if (stones[i][j] != STONE_EMPTY)
			{
				int k;
				stone lastStone;
				lastStone.x = i;
				lastStone.y = j;
				lastStone.type = stones[i][j];

				for (k = j+1; k < BOARD_H - 1; ++k)
				{
					if (stones[i][k] != STONE_EMPTY)
					{
						break;
					}

					stones[i][k] = lastStone.type;
					stones[lastStone.x][lastStone.y] = STONE_EMPTY;
					lastStone.y = k;
				}
			}
		}
	}
}

void boardGenerate()
{
	int pool[STONE_COUNT-1];
	int i;
	int j;

	if (!stones)
	{
		return;
	}

	generating = 1;

	memset(pool, 0, sizeof(pool));

	switch (currentAlgorithm)
	{
		case ALGO_RANDOM:
			for (i = 0; i < BOARD_W; ++i)
			{
				for (j = 0; j < BOARD_H; ++j)
				{
					if (i == 0 || j == 0 || i == BOARD_W - 1 || j == BOARD_H - 1)
					{
						stones[i][j] = STONE_EMPTY;
					}
					else
					{
						int draw = 1;
						int stoneNum;

						while (draw)
						{
							stoneNum = 1 + rand() % (STONE_COUNT - 1);

							if (pool[stoneNum - 1] < 4)
							{
								++pool[stoneNum - 1];
								draw = 0;

								stones[i][j] = stoneNum;
							}
						}
					}
				}
			}
		break;

		case ALGO_REVERSE:
			for (stonesLeft = 144; stonesLeft > 0; stonesLeft-=2)
			{
				int stoneNum;
				int match = 0;
				int x;
				int y;

				while (!match)
				{
					stoneNum = 1 + rand() % (STONE_COUNT - 1);

					if (pool[stoneNum - 1] < 4)
					{
						pool[stoneNum - 1] += 2;
						match = 1;
					}
				}

				match = 0;

				while (!match)
				{
					x = 1 + rand() % (BOARD_W - 2);
					y = 1 + rand() % (BOARD_H - 2);

					if (stones[x][y] == STONE_EMPTY)
					{
						stoneA.type = stoneNum;
						stones[x][y] = stoneA.type;
						stoneA.x = x;
						stoneA.y = y;

						match = 1;
					}
				}

				match = 0;

				while (!match)
				{
					x = 1 + rand() % (BOARD_W - 2);
					y = 1 + rand() % (BOARD_H - 2);

					if (stones[x][y] == STONE_EMPTY)
					{
						stoneA.type = stoneNum;
						stoneB.type = stoneNum;
						stones[x][y] = stoneB.type;
						stoneB.x = x;
						stoneB.y = y;

						match = boardCheckConnection(&stoneA, &stoneB);

						if (match)
						{
							stones[stoneA.x][stoneA.y] = stoneNum;
							stones[stoneB.x][stoneB.y] = stoneNum;
						}
						else
						{
							if (boardStoneSurrounded(&stoneA))
							{
								match = 1;
							}
							else
							{
								stones[stoneB.x][stoneB.y] = STONE_EMPTY;
							}
						}
					}
				}
			}
		break;

		default:
		break;
	}

	stoneA.type = STONE_EMPTY;
	stoneB.type = STONE_EMPTY;

	stonesLeft = 144;
	gameTime = 0;

	currentGameMode = newGameMode;

	generating = 0;
}

void boardSelectStone(int x, int y)
{
	crossing = 0;

	if (stones[x][y] == STONE_EMPTY)
	{
		return;
	}

	if (stoneA.type == STONE_EMPTY)
	{
		stoneA.x = x;
		stoneA.y = y;
		stoneA.type = stones[x][y];
	}
	else if (x != stoneA.x || y != stoneA.y)
	{
		stoneB.x = x;
		stoneB.y = y;
		stoneB.type = stones[x][y];

		if (!boardCheckConnection(&stoneA, &stoneB))
		{
			stoneA = stoneB;
			stoneB.type = STONE_EMPTY;
		}
		else
		{
			line tmpLineC = lineC;
			stonesLeft -= 2;

			if (!boardCheckAvailableMoves())
			{
				gameOver = 1;
				crossing = 0;
			}

			lineC = tmpLineC;
		}
	}
}

int boardCheckHorizontalIntersection(line *A, line *B)
{
	int i;
	int j;

	int x1 = A->x1 < B->x1 ? A->x1 : B->x1;
	int x2 = A->x2 > B->x2 ? A->x2 : B->x2;
	int y1 = A->y1 > B->y1 ? A->y1 : B->y1;
	int y2 = A->y2 < B->y2 ? A->y2 : B->y2;

	if (x1 == x2)
	{
		return 0;
	}

	if (!((A->y1 >= B->y1 && A->y1 <= B->y2) || (A->y2 >= B->y1 && A->y2 <= B->y2) || (B->y1 >= A->y1 && B->y1 <= A->y2) || (B->y2 >= A->y1 && B->y2 <= A->y2)))
	{
		return 0;
	}

	for (j = y1; j <= y2; ++j)
	{
		for (i = x1; i <= x2; ++i)
		{
			if (stones[i][j] > STONE_EMPTY)
			{
				break;
			}
		}

		if (i > x2)
		{
			lineC.x1 = x1;
			lineC.x2 = x2;
			lineC.y1 = j;
			lineC.y2 = j;

			return 1;
		}
	}

	return 0;
}

int boardCheckVerticalIntersection(line *A, line *B)
{
	int i;
	int j;

	int x1 = A->x1 > B->x1 ? A->x1 : B->x1;
	int x2 = A->x2 < B->x2 ? A->x2 : B->x2;
	int y1 = A->y1 < B->y1 ? A->y1 : B->y1;
	int y2 = A->y2 > B->y2 ? A->y2 : B->y2;

	if (y1 == y2)
	{
		return 0;
	}

	if (!((A->x1 >= B->x1 && A->x1 <= B->x2) || (A->x2 >= B->x1 && A->x2 <= B->x2) || (B->x1 >= A->x1 && B->x1 <= A->x2) || (B->x2 >= A->x1 && B->x2 <= A->x2)))
	{
		return 0;
	}

	for (i = x1; i <= x2; ++i)
	{
		for (j = y1; j <= y2; ++j)
		{
			if (stones[i][j] > STONE_EMPTY)
			{
				break;
			}

#if defined(DEBUG)
			drawRectangle(screen, i * STONE_W, j * STONE_H, STONE_W, STONE_H);
			SDL_Flip(screen);
			SDL_Delay(100);
#endif
		}

		if (j > y2)
		{
			lineC.x1 = i;
			lineC.x2 = i;
			lineC.y1 = y1;
			lineC.y2 = y2;
#if defined(DEBUG)
			SDL_Delay(100);
#endif
			return 1;
		}
	}

	return 0;
}

int boardCheckConnection(stone *A, stone *B)
{
	line lineA;
	line lineB;

	int i;
	int j;

	if (A == NULL || B == NULL || A->type == STONE_EMPTY || B->type == STONE_EMPTY)
	{
		return 0;
	}

	if (A->type != B->type)
	{
		return 0;
	}

	stones[A->x][A->y] = STONE_EMPTY;
	stones[B->x][B->y] = STONE_EMPTY;

	// Horizontal field of view.
	lineA.x1 = A->x;
	lineA.y1 = A->y;
	lineA.x2 = A->x;
	lineA.y2 = A->y;

	for (i = A->x, j = A->y; i >= 0; --i)
	{
		if (stones[i][j] != STONE_EMPTY)
		{
			break;
		}

		lineA.x1 = i;
	}
	for (i = A->x, j = A->y; i < BOARD_W; ++i)
	{
		if (stones[i][j] != STONE_EMPTY)
		{
			break;
		}

		lineA.x2 = i;
	}

	lineB.x1 = B->x;
	lineB.y1 = B->y;
	lineB.x2 = B->x;
	lineB.y2 = B->y;

	for (i = B->x, j = B->y; i >= 0; --i)
	{
		if (stones[i][j] != STONE_EMPTY)
		{
			break;
		}

		lineB.x1 = i;
	}
	for (i = B->x, j = B->y; i < BOARD_W; ++i)
	{
		if (stones[i][j] != STONE_EMPTY)
		{
			break;
		}

		lineB.x2 = i;
	}

	crossing = boardCheckVerticalIntersection(&lineA, &lineB);

	if (!crossing)
	{
		// Vertical field of view.
		lineA.x1 = A->x;
		lineA.y1 = A->y;
		lineA.x2 = A->x;
		lineA.y2 = A->y;

		for (i = A->x, j = A->y; j >= 0; --j)
		{
			if (stones[i][j] != STONE_EMPTY)
			{
				break;
			}

			lineA.y1 = j;
		}
		for (i = A->x, j = A->y; j < BOARD_H; ++j)
		{
			if (stones[i][j] != STONE_EMPTY)
			{
				break;
			}

			lineA.y2 = j;
		}

		lineB.x1 = B->x;
		lineB.y1 = B->y;
		lineB.x2 = B->x;
		lineB.y2 = B->y;

		for (i = B->x, j = B->y; j >= 0; --j)
		{
			if (stones[i][j] != STONE_EMPTY)
			{
				break;
			}

			lineB.y1 = j;
		}
		for (i = B->x, j = B->y; j < BOARD_H; ++j)
		{
			if (stones[i][j] != STONE_EMPTY)
			{
				break;
			}

			lineB.y2 = j;
		}

		crossing = boardCheckHorizontalIntersection(&lineA, &lineB);
	}

	if (crossing)
	{
		A->type = STONE_EMPTY;
		B->type = STONE_EMPTY;

		if (!generating && !checkingMoves && currentGameMode == GAME_MODE_GRAVITY)
		{
			boardApplyGravity();
		}

		return 1;
	}
	else
	{
		stones[A->x][A->y] = A->type;
		stones[B->x][B->y] = B->type;
	}

	return 0;
}

void boardDrawConnection()
{
	line lineA;
	line lineB;

	int x1;
	int y1;
	int x2;
	int y2;

	if (lineC.y1 == lineC.y2) // lineC is a horizontal line.
	{
		lineA.x1 = stoneA.x;
		lineA.x2 = stoneA.x;
		lineA.y1 = (stoneA.y < lineC.y1 ? stoneA.y : lineC.y1);
		lineA.y2 = (stoneA.y > lineC.y2 ? stoneA.y : lineC.y2);

		lineB.x1 = stoneB.x;
		lineB.x2 = stoneB.x;
		lineB.y1 = (stoneB.y < lineC.y1 ? stoneB.y : lineC.y1);
		lineB.y2 = (stoneB.y > lineC.y2 ? stoneB.y : lineC.y2);
	}
	else if (lineC.x1 == lineC.x2) // lineC is a vertical line.
	{
		lineA.x1 = (stoneA.x < lineC.x1 ? stoneA.x : lineC.x1);
		lineA.x2 = (stoneA.x > lineC.x2 ? stoneA.x : lineC.x2);
		lineA.y1 = stoneA.y;
		lineA.y2 = stoneA.y;

		lineB.x1 = (stoneB.x < lineC.x1 ? stoneB.x : lineC.x1);
		lineB.x2 = (stoneB.x > lineC.x2 ? stoneB.x : lineC.x2);
		lineB.y1 = stoneB.y;
		lineB.y2 = stoneB.y;
	}
	else
	{
		return;
	}

	// Line A.
	x1 = lineA.x1 * STONE_W - 1 + STONE_W/2 - lineA.x1;
	y1 = lineA.y1 * STONE_H + STONE_H/2 - lineA.y1;
	x2 = (lineA.x2 - lineA.x1) * STONE_W + 1 - (lineA.x2 - lineA.x1);
	y2 = (lineA.y2 - lineA.y1) * STONE_H + 1 - (lineA.y2 - lineA.y1);

	// Drawing offset at the top of the screen.
	if (y1 < STONE_H - STONE_H/3)
	{
		y2 -= (STONE_H - STONE_H/3) - y1;
		y1 = STONE_H - STONE_H/3;
	}
	// Drawing offset at the bottom of the screen.
	else if ((y1 + y2) > (BOARD_H - 1) * STONE_H - STONE_H/3)
	{
		y2 -= STONE_H/3;
	}

	x1 += BOARD_OFFSET_X;
	y1 += BOARD_OFFSET_Y;
	drawRectangle(screen, x1, y1, x2, y2);

	// Line B.
	x1 = lineB.x1 * STONE_W - 1 + STONE_W/2 - lineB.x1;
	y1 = lineB.y1 * STONE_H + STONE_H/2 - lineB.y1;
	x2 = (lineB.x2 - lineB.x1) * STONE_W + 1 - (lineB.x2 - lineB.x1);
	y2 = (lineB.y2 - lineB.y1) * STONE_H + 1 - (lineB.y2 - lineB.y1);

	// Drawing offset at the top of the screen.
	if (y1 < STONE_H - STONE_H/3)
	{
		y2 -= (STONE_H - STONE_H/3) - y1;
		y1 = STONE_H - STONE_H/3;
	}
	// Drawing offset at the bottom of the screen.
	else if ((y1 + y2) > (BOARD_H - 1) * STONE_H - STONE_H/3)
	{
		y2 -= STONE_H/3;
	}

	x1 += BOARD_OFFSET_X;
	y1 += BOARD_OFFSET_Y;
	drawRectangle(screen, x1, y1, x2, y2);

	// Line C.
	x1 = lineC.x1 * STONE_W - 1 + STONE_W/2 - lineC.x1;
	y1 = lineC.y1 * STONE_H + STONE_H/2 - lineC.y1;
	x2 = (lineC.x2 - lineC.x1) * STONE_W + 1 - (lineC.x2 - lineC.x1);
	y2 = (lineC.y2 - lineC.y1) * STONE_H + 1 - (lineC.y2 - lineC.y1);

	// Drawing offset at the top of the screen.
	if (y1 < STONE_H - STONE_H/3)
	{
		y1 = STONE_H - STONE_H/3;
	}
	// Drawing offset at the bottom of the screen.
	else if ((y1 + y2) > (BOARD_H - 1) * STONE_H - STONE_H/3)
	{
		y1 -= STONE_H/3;
	}

	x1 += BOARD_OFFSET_X;
	y1 += BOARD_OFFSET_Y;
	drawRectangle(screen, x1, y1, x2, y2);
}

void boardLoad()
{
	int i;

	if (!boardBackgroundIMG)
	{
		boardBackgroundIMG = loadImage("data/gfx/background.bmp");
	}

	tilesetLoad(&stonesTileset, "data/gfx/stones.bmp", STONE_W, STONE_H, 9, STONE_COUNT);

	stones = malloc(sizeof(int*) * BOARD_W);
	if (!stones)
	{
		return;
	}
	for (i = 0; i < BOARD_W; ++i)
	{
		stones[i] = malloc(sizeof(int) * BOARD_H);
		if (!stones[i])
		{
			return;
		}

		memset(stones[i], 0, sizeof(int) * BOARD_H);
	}

	gameOver = 0;

	if (continueGame)
	{
		stonesLeft = 0;
		getBoard(0);
		if (!boardCheckAvailableMoves())
		{
			gameOver = 1;

			if (!stonesLeft)
			{
				gameOver = 0;
				boardGenerate();
				cursorX = 1;
				cursorY = 1;
			}
		}
	}
	else
	{
		boardGenerate();
		cursorX = 1;
		cursorY = 1;
	}

	crossing = 0;
}

void boardUnload()
{
	SDL_FreeSurface(boardBackgroundIMG);
	boardBackgroundIMG = NULL;

	tilesetUnload(&stonesTileset);

	if (stones)
	{
		int i;

		for (i = 0; i < BOARD_W; ++i)
		{
			if (stones[i])
			{
				free(stones[i]);
			}
		}

		free(stones);
		stones = NULL;
	}
}

void boardDraw()
{
	int i;
	int j;
	int x;
	int y;

	drawImage(boardBackgroundIMG, NULL, screen, 0, 0);

	for (i = 0, x = 0; x < BOARD_W; i+=STONE_W, ++x)
	{
		for (j = 0, y = 0; y < BOARD_H; j+=STONE_H, ++y)
		{
			if (stones[x][y] > 0)
			{
				drawImage(stonesTileset.image, &stonesTileset.clip[stones[x][y] - 1], screen, BOARD_OFFSET_X + i - x, BOARD_OFFSET_Y + j - y);
			}
		}
	}

	if (crossing)
	{
		boardDrawConnection();
	}

	if (!gameOver)
	{
		drawRectangle(screen, BOARD_OFFSET_X + cursorX * STONE_W - 1 - cursorX, BOARD_OFFSET_Y + cursorY * STONE_H - 1 - cursorY, STONE_W + 2, STONE_H + 2);
		drawRectangle(screen, BOARD_OFFSET_X + cursorX * STONE_W - cursorX, BOARD_OFFSET_Y + cursorY * STONE_H - cursorY, STONE_W, STONE_H);
		if (stoneA.type != STONE_EMPTY)
		{
			drawRectangle(screen, BOARD_OFFSET_X + stoneA.x * STONE_W - 1 - stoneA.x, BOARD_OFFSET_Y + stoneA.y * STONE_H - 1 - stoneA.y, STONE_W + 2, STONE_H + 2);
			drawRectangle(screen, BOARD_OFFSET_X + stoneA.x * STONE_W - stoneA.x, BOARD_OFFSET_Y + stoneA.y * STONE_H - stoneA.y, STONE_W, STONE_H);
		}
		if (stoneB.type != STONE_EMPTY)
		{
			drawRectangle(screen, BOARD_OFFSET_X + stoneB.x * STONE_W - 1 - stoneB.x, BOARD_OFFSET_Y + stoneB.y * STONE_H - 1 - stoneB.y, STONE_W + 2, STONE_H + 2);
			drawRectangle(screen, BOARD_OFFSET_X + stoneB.x * STONE_W - stoneB.x, BOARD_OFFSET_Y + stoneB.y * STONE_H - stoneB.y, STONE_W, STONE_H);
		}
	}
}
