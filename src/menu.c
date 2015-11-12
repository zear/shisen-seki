#include "menu.h"

#include "font.h"
#include "input.h"

menuItems menuMainItems[] =
{
	MENU_CONTINUE,
	MENU_NEW_GAME,
	MENU_HISCORE,
	MENU_HELP,
	MENU_CREDITS,
	MENU_QUIT
};

menuItems menuNewGameItems[] =
{
	MENU_START_GAME,
	MENU_PRACTICE,
	MENU_BACK,
	MENU_SEPARATOR,
	MENU_GAME_TYPE,
	MENU_ALGORITHM
};

char menuText[][20] =
{
	"",
	"Back",
	"Continue game",
	"New game",
	"Score table",
	"Help",
	"Credits",
	"Exit",
	"Start game",
	"Practice",
	"Game type",
	"Algorithm",
};

menuContainer menuMain;
menuContainer menuNewGame;

void menuLoad()
{
	menuMain.items = menuMainItems;
	menuMain.length = sizeof(menuMainItems)/sizeof(menuMainItems[0]);
	menuNewGame.items = menuNewGameItems;
	menuNewGame.length = sizeof(menuNewGameItems)/sizeof(menuNewGameItems[0]);
}

void menuDraw(menuContainer *curMenu, font *fontRegular, font *fontSelected, int selection, int offset, int y)
{
	int i;

	if (!curMenu)
	{
		return;
	}

	for (i = offset; i < curMenu->length; ++i)
	{
		font *curFont = i == selection ? fontSelected : fontRegular;

		dTextCentered(curFont, menuText[curMenu->items[i]-1], y + (curFont->h + curFont->leading) * i, SHADOW_DROP);
	}
}
