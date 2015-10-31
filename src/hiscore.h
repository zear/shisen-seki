#ifndef _HISCORE_H_
#define _HISCORE_H_

#include <SDL.h>
#include "game.h"
#include "board.h"

#define MAX_MODES	((GAME_MODE_COUNT - 1) + (ALGO_COUNT - 1))
#define MAX_SCORES	5
#define SCORE_NAME_LEN	4

typedef struct scoreEntry
{
	char name[SCORE_NAME_LEN];
	unsigned long time;
} scoreEntry;

SDL_Surface *hiscoreBackgroundIMG;
extern scoreEntry scoreTable[MAX_MODES][MAX_SCORES];

void hiscoreUnload();
void hiscoreLoad();
void hiscoreAddRecord(scoreEntry *entry, gameMode *mode, algorithm *algo);
void hiscoreLogic();
void hiscoreDraw();

#endif /* _HISCORE_H_ */
