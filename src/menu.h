#ifndef _MENU_H_
#define _MENU_H_

#include "font.h"

typedef enum menuItems
{
	MENU_UNSET = 0,
	MENU_SEPARATOR,
	MENU_BACK,
	MENU_CONTINUE,
	MENU_NEW_GAME,
	MENU_HISCORE,
	MENU_HELP,
	MENU_CREDITS,
	MENU_QUIT,
	MENU_START_GAME,
	MENU_PRACTICE,
	MENU_GAME_TYPE,
	MENU_ALGORITHM,
	MENU_COUNT
	
} menuItems;

typedef struct menuContainer
{
	menuItems *items;
	int length;
} menuContainer;

extern menuContainer menuMain;
extern menuContainer menuNewGame;
extern char menuText[][20];

void menuLoad();
void menuDraw(menuContainer *curMenu, font *fontRegular, font *fontSelected, int selection, int offset, int y);

#endif /* _MENU_H_ */
