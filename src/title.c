#include "title.h"

#include <SDL.h>
#include "audio.h"
#include "board.h"
#include "fileio.h"
#include "font.h"
#include "game.h"
#include "input.h"
#include "main.h"
#include "states.h"
#include "video.h"

#define MENU_ITEM_COUNT	7

int menuSel;
int displayHelp;
int displayCredits;
int helpPage;
int creditsPage;
int savePresent;

void titleUnload()
{
	SDL_FreeSurface(titleBackgroundIMG);
	titleBackgroundIMG = NULL;
}

void titleLoad()
{
	if (!titleBackgroundIMG)
	{
		titleBackgroundIMG = loadImage("data/gfx/background.bmp");
	}

	savePresent = getBoard(1);

	if (savePresent)
	{
		menuSel = 0;
	}
	else
	{
		menuSel = 1;
	}

	bgdMusic = loadMusic(bgdMusic, "data/music/bgd01.mp3");

	if(bgdMusic)
	{
		playMusic(bgdMusic);
	}
}

void titleLogic()
{
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
			quit = 1;
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
			menuSel = MENU_ITEM_COUNT-1;
		}
	}
	if(keys[KEY_OK])
	{
		keys[KEY_OK] = 0;

		if (displayHelp)
		{
			if(++helpPage > 1)
			{
				helpPage = 0;
				displayHelp = 0;
			}
		}
		else if (displayCredits)
		{
			if(++creditsPage > 1)
			{
				creditsPage = 0;
				displayCredits = 0;
			}
		}
		else
		{
			switch (menuSel)
			{
				case 0:
					if (savePresent)
					{
						programStateNew = STATE_GAME;
						continueGame = 1;
					}
				break;

				case 1:
					programStateNew = STATE_GAME;
					continueGame = 0;
				break;

				case 2:
					if (++newGameMode >= GAME_MODE_COUNT)
					{
						newGameMode = GAME_MODE_UNSET+1;
					}
				break;

				case 3:
					if (++currentAlgorithm >= ALGO_COUNT)
					{
						currentAlgorithm = ALGO_UNSET+1;
					}
				break;

				case 4:
					displayHelp = 1;
				break;

				case 5:
					displayCredits = 1;
				break;

				case 6:
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
		else if (menuSel == 2 && --newGameMode == GAME_MODE_UNSET)
		{
			newGameMode = GAME_MODE_COUNT - 1;
		}
		else if (menuSel == 3 && --currentAlgorithm == ALGO_UNSET)
		{
			currentAlgorithm = ALGO_COUNT - 1;
		}
	}

	if(keys[KEY_RIGHT])
	{
		keys[KEY_RIGHT] = 0;

		if (displayHelp)
		{
			if (++helpPage > 1)
			{
				helpPage = 1;
			}
		}
		if (displayCredits)
		{
			if (++creditsPage > 1)
			{
				creditsPage = 1;
			}
		}
		else if (menuSel == 2 && ++newGameMode >= GAME_MODE_COUNT)
		{
			newGameMode = GAME_MODE_UNSET + 1;
		}
		else if (menuSel == 3 && ++currentAlgorithm >= ALGO_COUNT)
		{
			currentAlgorithm = ALGO_UNSET + 1;
		}
	}

	if (!displayHelp)
	{
		if(keys[KEY_UP])
		{
			keys[KEY_UP] = 0;

			if(--menuSel < (savePresent ? 0 : 1))
			{
				menuSel = MENU_ITEM_COUNT-1;
			}
		}

		if(keys[KEY_DOWN])
		{
			keys[KEY_DOWN] = 0;

			if(++menuSel > MENU_ITEM_COUNT-1)
			{
				menuSel = savePresent ? 0 : 1;
			}
		}
	}
}

void titleDraw()
{
	char menuItems[MENU_ITEM_COUNT][20] = {"Continue game", "New game", "Game type", "Algorithm", "Help", "Credits", "Exit"};
	int i;

	drawImage(titleBackgroundIMG, NULL, screen, 0, 0);

	if (displayHelp)
	{
		char page[10] = "";
		char helpHeader[2][10] = { "Rules", "Controls" };
		char helpText[2][500] = { "The goal of Shisen-Sho is to remove\nall stones from the board. Do so\nby matching pairs of stones with\nthe same face.\n\nYou can only match stones if they\ncan be joined by 3 or less lines.\nEach line must be either horizontal\nor vertical and can't cross other\nstones on the board.\n\nNot all games are solvable.", "D-PAD  - move selection\nA      - select a stone\nB      - remove selection\nSELECT - exit game" };

		strcpy(page, helpPage ? "[Page 2]" : "[Page 1]");

		dText(&gameFontGray, page, 2, 2);
		dTextCentered(&gameFont, helpHeader[helpPage], 50 - (gameFont.h + gameFont.leading) * 2);
		dTextCentered(&gameFontGray, helpText[helpPage], 50);
	}
	else if (displayCredits)
	{
		int i;
		char page[10] = "";
		char creditsText[2][10][500] =
		{
			{ "Programming:",
			  "Artur Rojek",
			  "",
			  "Graphics:",
			  "Joe Vargas",
			  "",
			  "",
			  "",
			  "",
			  ""
			},
			{ "Music:",
			  "",
			  "",
			  "",
			  "",
			  "",
			  "",
			  "",
			  "",
			  ""
			}
		};

		strcpy(page, creditsPage ? "[Page 2]" : "[Page 1]");

		dText(&gameFontGray, page, 2, 2);
		dTextCentered(&gameFont, "Credits", 50 - (gameFont.h + gameFont.leading) * 2);

		for (i = 0; i < 10; ++i)
		{
			dTextCentered(&gameFontGray, creditsText[creditsPage][i], 50 + (gameFont.h + gameFont.leading) * i);
		}
	}
	else
	{
		char version[15];
		sprintf(version, "v%d.%d.%d", PROGRAM_MAJOR_VERSION, PROGRAM_MINOR_VERSION, PROGRAM_PATCH_VERSION);
		dText(&gameFontGray, version, SCREEN_W - strlen(version) * (gameFontGray.w + gameFontGray.tracking), 1);
		dTextCentered(&gameFont, "S h i s e n - S e k i", 80);

		if (programStateNew == STATE_GAME)
		{
			dTextCentered(&gameFont, "* Generating new board *", SCREEN_H/2);
			dTextCentered(&gameFont, "Please wait...", SCREEN_H/2 + (gameFont.h + gameFont.leading) * 2);
		}
		else
		{
			switch (newGameMode)
			{
				case GAME_MODE_CLASSIC:
					strncpy(menuItems[2], "Game type: classic", 20);
				break;
				case GAME_MODE_GRAVITY:
					strncpy(menuItems[2], "Game type: gravity", 20);
				break;

				default:
				break;
			}

			switch (currentAlgorithm)
			{
				case ALGO_RANDOM:
					strncpy(menuItems[3], "Algorithm: random", 20);
				break;
				case ALGO_REVERSE:
					strncpy(menuItems[3], "Algorithm: reverse", 20);
				break;

				default:
				break;
			}

			for (i = savePresent ? 0 : 1; i < sizeof(menuItems)/sizeof(menuItems[0]); ++i)
			{
				font *curFont = (i == menuSel ? &gameFont : &gameFontGray);
				dTextCentered(curFont, menuItems[i], SCREEN_H/2 + (curFont->h + curFont->leading) * i);
			}
		}

		dTextCentered(&gameFont, "(c) 2015 Artur Rojek", SCREEN_H - (gameFont.h + gameFont.leading));
	}
}
