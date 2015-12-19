#include "title.h"

#include <SDL.h>
#include "audio.h"
#include "board.h"
#include "fileio.h"
#include "font.h"
#include "game.h"
#include "input.h"
#include "main.h"
#include "menu.h"
#include "states.h"
#include "video.h"

#define HELP_PAGES	5
#define CREDITS_PAGES	3

menuContainer *curMenu;
int menuSel;
int displayHelp;
int displayCredits;
int helpPage;
int creditsPage;
int savePresent;
tileset stonesTileset;

void titleUnload()
{
	SDL_FreeSurface(titleBackgroundIMG);
	titleBackgroundIMG = NULL;
	tilesetUnload(&stonesTileset);
}

void titleLoad()
{
	if (!titleBackgroundIMG)
	{
		titleBackgroundIMG = loadImage("data/gfx/background.bmp");
	}

	tilesetLoad(&stonesTileset, "data/gfx/stones.bmp", STONE_W, STONE_H, 9, STONE_COUNT);

	savePresent = getBoard(1);
	menuSel = savePresent ? 0 : 1;
	bgdMusic = loadMusic(bgdMusic, "data/music/bgd01.ogg");

	if(bgdMusic)
	{
		playMusic(bgdMusic);
	}

	menuLoad();
	curMenu = &menuMain;
}

void titleLogic()
{
	int i;

	if(keys[KEY_BACK])
	{
		keys[KEY_BACK] = 0;

		if (displayHelp)
		{
			displayHelp = 0;
		}
		else if (displayCredits)
		{
			displayCredits = 0;
		}
		else
		{
			if (curMenu == &menuNewGame)
			{
				curMenu = &menuMain;
				menuSel = savePresent ? 0 : 1;
			}
			else
			{
				menuSel = curMenu->length - 1;
			}
		}
	}
	if(keys[KEY_CANCEL])
	{
		keys[KEY_CANCEL] = 0;

		if (displayHelp)
		{
			displayHelp = 0;
		}
		else if (displayCredits)
		{
			displayCredits = 0;
		}
		else
		{
			if (curMenu == &menuNewGame)
			{
				curMenu = &menuMain;
				menuSel = savePresent ? 0 : 1;
			}
			else
			{
				menuSel = curMenu->length - 1;
			}
		}
	}
	if(keys[KEY_OK])
	{
		keys[KEY_OK] = 0;

		if (displayHelp)
		{
			if(++helpPage >= HELP_PAGES)
			{
				helpPage = 0;
				displayHelp = 0;
			}
		}
		else if (displayCredits)
		{
			if(++creditsPage >= CREDITS_PAGES)
			{
				creditsPage = 0;
				displayCredits = 0;
			}
		}
		else
		{
			switch (curMenu->items[menuSel])
			{
				case MENU_BACK:
					if (curMenu == &menuNewGame)
					{
						curMenu = &menuMain;
						menuSel = savePresent ? 0 : 1;
					}
				break;
				case MENU_CONTINUE:
					if (savePresent)
					{
						programStateNew = STATE_GAME;
						continueGame = 1;
					}
				break;

				case MENU_START_GAME:
					programStateNew = STATE_GAME;
					continueGame = 0;
					practice = 0;
				break;

				case MENU_PRACTICE:
					programStateNew = STATE_GAME;
					continueGame = 0;
					practice = 1;
				break;

				case MENU_NEW_GAME:
					curMenu = &menuNewGame;
					menuSel = 0;
				break;

				case MENU_GAME_TYPE:
					if (++newGameMode >= GAME_MODE_COUNT)
					{
						newGameMode = GAME_MODE_UNSET+1;
					}
				break;

				case MENU_ALGORITHM:
					if (++currentAlgorithm >= ALGO_COUNT)
					{
						currentAlgorithm = ALGO_UNSET+1;
					}
				break;
				case MENU_ANIMATIONS:
					showAnimations = !showAnimations;
				break;

				case MENU_HISCORE:
					programStateNew = STATE_HISCORE;
				break;

				case MENU_HELP:
					displayHelp = 1;
				break;

				case MENU_CREDITS:
					displayCredits = 1;
				break;

				case MENU_QUIT:
					unloadMusic(&bgdMusic);
					quit = 1;
				break;

				default:
				break;
			}
		}
	}

	if(keys[KEY_LEFT])
	{
		keys[KEY_LEFT] = 0;

		if (displayHelp)
		{
			if (--helpPage < 0)
			{
				helpPage = 0;
			}
		}
		else if (displayCredits)
		{
			if (--creditsPage < 0)
			{
				creditsPage = 0;
			}
		}
		else if (curMenu->items[menuSel] == MENU_GAME_TYPE && --newGameMode == GAME_MODE_UNSET)
		{
			newGameMode = GAME_MODE_COUNT - 1;
		}
		else if (curMenu->items[menuSel] == MENU_ALGORITHM && --currentAlgorithm == ALGO_UNSET)
		{
			currentAlgorithm = ALGO_COUNT - 1;
		}
		else if (curMenu->items[menuSel] == MENU_ANIMATIONS)
		{
			showAnimations = !showAnimations;
		}
	}

	if(keys[KEY_RIGHT])
	{
		keys[KEY_RIGHT] = 0;

		if (displayHelp)
		{
			if (++helpPage >= HELP_PAGES)
			{
				helpPage = HELP_PAGES - 1;
			}
		}
		if (displayCredits)
		{
			if (++creditsPage >= CREDITS_PAGES)
			{
				creditsPage = CREDITS_PAGES - 1;
			}
		}
		else if (curMenu->items[menuSel] == MENU_GAME_TYPE && ++newGameMode >= GAME_MODE_COUNT)
		{
			newGameMode = GAME_MODE_UNSET + 1;
		}
		else if (curMenu->items[menuSel] == MENU_ALGORITHM && ++currentAlgorithm >= ALGO_COUNT)
		{
			currentAlgorithm = ALGO_UNSET + 1;
		}
		else if (curMenu->items[menuSel] == MENU_ANIMATIONS)
		{
			showAnimations = !showAnimations;
		}
	}

	for (i = 0; mouseMoved && (i < curMenu->length); ++i)
	{
		int posY = 80 + (gameFontRegular.h + gameFontRegular.leading) * i;

		if (mouse[1] >= posY && mouse[1] <= posY + gameFontRegular.h)
		{
			menuSel = i;
			break;
		}
	}

	if (!displayHelp)
	{
		if(keys[KEY_UP])
		{
			keys[KEY_UP] = 0;

			--menuSel;

			if (curMenu->items[menuSel] == MENU_SEPARATOR)
			{
				--menuSel;
			}

			if(menuSel < (curMenu == &menuMain ? (savePresent ? 0 : 1) : 0))
			{
				menuSel = curMenu->length - 1;
			}
		}

		if(keys[KEY_DOWN])
		{
			keys[KEY_DOWN] = 0;

			++menuSel;

			if (curMenu->items[menuSel] == MENU_SEPARATOR)
			{
				++menuSel;
			}

			if(menuSel > curMenu->length - 1)
			{
				menuSel = curMenu == &menuMain ? (savePresent ? 0 : 1) : 0;
			}
		}
	}
}

void titleDraw()
{
	int i;

	drawImage(titleBackgroundIMG, NULL, screen, 0, 0);

	if (displayHelp)
	{
		char page[10] = "";
		char helpHeader[HELP_PAGES][30] = { "Rules", "Stones", "Game types", "Board generation algorithms", "Controls" };
		char helpText[HELP_PAGES][500] = {
			"The goal of Shisen-Seki is to remove\nall stones from the board. Do so by\nmatching pairs of stones with\nthe same face.\n\nYou can only match stones if they\ncan be joined by 3 or less lines.\nEach line must be either horizontal\nor vertical and can't cross other\nstones on the board.\n\nNot all games are solvable.",
			"There are 144 stones on the board.\nThe majority of them will be in\ngroups of four stones of the same\nface. There are two exceptions:\nthe Seasons and Flowers stones.\n\n     Seasons:         Flowers:\n\n\n\nEven if those stones have distinct\nfaces, they can be paired together.",
			"Classic:                          \nThe original Shisen-Sho rules.\nSee the \"Rules\" page for more\ndetails.\n\nGravity:\nIn this mode, after a stone is\nremoved, the remaining stones\nin the column are moved downwards.",
			"Reverse:                               \nThe board is populated using a reverse\nbuilding algorithm.\nThis method generates a higher amount\nof solvable games.\n\nRandom:\nThe board is populated at a fully\nrandom order.\nThis method might generate a higher\nnumber of difficult or unsolvable games.",
			"D-PAD  - move selection     \nA      - select a stone\nB      - remove selection\nY      - display stone ranks\nSELECT - exit game" };

		sprintf(page, "[Page %d]", helpPage + 1);

		dText(&gameFontBlack, page, 2, 2, SHADOW_NONE);
		dTextCentered(&gameFontRegular, helpHeader[helpPage], 50 - (gameFontRegular.h + gameFontRegular.leading) * 2, SHADOW_OUTLINE);
		dTextCentered(&gameFontBlack, helpText[helpPage], 50, SHADOW_NONE);

		if (helpPage == 1) // Stone info.
		{
			int i;

			for (i = 0; i < 4; ++i)
			{
				drawImage(stonesTileset.image, &stonesTileset.clip[SEASON_SPRING - 1 + i], screen, 68 + (STONE_W + 1) * i, 155);
			}

			for (i = 0; i < 4; ++i)
			{
				drawImage(stonesTileset.image, &stonesTileset.clip[FLOWER_PLUM - 1 + i], screen, 188 + (STONE_W + 1) * i, 155);
			}
		}
	}
	else if (displayCredits)
	{
		char page[10] = "";
		char creditsText[CREDITS_PAGES][10][500] =
		{
			{ "Programming:",
			  "Artur Rojek (zear)",
			  "",
			  "Graphics:",
			  "Joe Vargas (jxv)",
			  "",
			  "",
			  "",
			  "",
			  ""
			},
			{ "Music:",
			  "Kevin MacLeod (incompetech.com)",
			  "\"White Lotus\"",
			  "",
			  "Licensed under",
			  "Creative Commons: By Attribution 3.0",
			  "http://creativecommons.org/licenses/by/3.0/",
			  "",
			  "Changes: Trimmed and converted to OGG",
			  ""
			},
			{ "Special thanks:",
			  "(alphabetical order)",
			  "",
			  "JohnnyOnFlame",
			  "Nebuleon",
			  "remaxim",
			  "Surkow",
			  "",
			  "",
			  ""
			}
		};

		sprintf(page, "[Page %d]", creditsPage + 1);

		dText(&gameFontBlack, page, 2, 2, SHADOW_NONE);
		dTextCentered(&gameFontRegular, "Credits", 50 - (gameFontRegular.h + gameFontRegular.leading) * 2, SHADOW_OUTLINE);

		for (i = 0; i < 10; ++i)
		{
			dTextCentered(&gameFontBlack, creditsText[creditsPage][i], 50 + (gameFontRegular.h + gameFontRegular.leading) * i, SHADOW_NONE);
		}
	}
	else
	{
		char version[15];
		sprintf(version, "v%d.%d.%d", PROGRAM_MAJOR_VERSION, PROGRAM_MINOR_VERSION, PROGRAM_PATCH_VERSION);
		dText(&gameFontBlack, version, SCREEN_W - strlen(version) * (gameFontSelected.w + gameFontSelected.tracking), 1, SHADOW_NONE);
		dTextCentered(&gameFontRegular, "S h i s e n - S e k i", 40, SHADOW_OUTLINE);

		if (programStateNew == STATE_GAME)
		{
			dTextCentered(&gameFontRegular, "* Generating new board *", SCREEN_H/2, SHADOW_OUTLINE);
			dTextCentered(&gameFontRegular, "Please wait...", SCREEN_H/2 + (gameFontRegular.h + gameFontRegular.leading) * 2, SHADOW_OUTLINE);
		}
		else
		{
			if (curMenu == &menuNewGame)
			{
				switch (newGameMode)
				{
					case GAME_MODE_CLASSIC:
						strncpy(menuText[MENU_GAME_TYPE-1], "Game type: classic", 20);
					break;
					case GAME_MODE_GRAVITY:
						strncpy(menuText[MENU_GAME_TYPE-1], "Game type: gravity", 20);
					break;

					default:
					break;
				}

				switch (currentAlgorithm)
				{
					case ALGO_RANDOM:
						strncpy(menuText[MENU_ALGORITHM-1], "Algorithm: random", 20);
					break;
					case ALGO_REVERSE:
						strncpy(menuText[MENU_ALGORITHM-1], "Algorithm: reverse", 20);
					break;

					default:
					break;
				}

				snprintf(menuText[MENU_ANIMATIONS-1], 20, "Animations: %s", showAnimations ? "yes" : "no");
			}

			menuDraw(curMenu, &gameFontRegular, &gameFontSelected, menuSel, curMenu == &menuMain ? (savePresent ? 0 : 1) : 0, 80);
		}

		dTextCentered(&gameFontSelected, "(c) 2015 Artur Rojek", SCREEN_H - (gameFontSelected.h + gameFontSelected.leading), SHADOW_DROP);

	}
}
