#ifndef _BOARD_H_
#define _BOARD_H_

#include <SDL.h>
#include "stone.h"

#define	BOARD_W	20
#define	BOARD_H	10

#define	STONE_W	16
#define	STONE_H	26

#define BOARD_OFFSET_X	SCREEN_W/2 - (BOARD_W*STONE_W - BOARD_W)/2
#define BOARD_OFFSET_Y	SCREEN_H/2 - (BOARD_H*STONE_H - BOARD_H)/2

#define	ALLOWED_MOVES	3

typedef enum algorithm
{
	ALGO_UNSET = 0,
	ALGO_RANDOM,
	ALGO_REVERSE,
	ALGO_COUNT
} algorithm;

typedef struct line
{
	int x1;
	int y1;
	int x2;
	int y2;
} line;

extern algorithm currentAlgorithm;
extern stoneType **stones;
extern stone stoneA;
extern stone stoneB;
extern int stonesLeft;
extern int cursorX;
extern int cursorY;
SDL_Surface *boardBackgroundIMG;

int boardStoneSurrounded(stone *st);
int boardCheckAvailableMoves();
void boardApplyGravity();
void boardGenerate();
void boardSelectStone(int x, int y);
int boardCheckHorizontalIntersection(line *A, line *B);
int boardCheckVerticalIntersection(line *A, line *B);
int boardCheckConnection(stone *A, stone *B);
void boardDrawConnection();
void boardLoad();
void boardUnload();
void boardDraw();

#endif /* _BOARD_H_ */
