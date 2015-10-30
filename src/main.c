#include "main.h"

#include <stdlib.h>
#include <time.h>
#include "audio.h"
#include "fileio.h"
#include "font.h"
#include "input.h"
#include "board.h"
#include "states.h"
#include "video.h"

int quit;

int init()
{
	getConfigDir();
	getConfig();

	if (initSDL())
	{
		return -1;
	}

	if (initAudio())
	{
		deinitSDL();
		return -1;
	}

	if (fontLoad(&gameFontRegular, "data/gfx/fontRegular.bmp", 6, 11, 1, 4))
	{
		return -1;
	}

	if (fontLoad(&gameFontSelected, "data/gfx/fontSelected.bmp", 6, 11, 1, 4))
	{
		return -1;
	}

	return 0;
}

void deinit()
{
	if (configDir)
	{
		free(configDir);
	}

	fontUnload(&gameFontRegular);
	fontUnload(&gameFontSelected);

	deinitAudio();
	deinitSDL();
}

int main(int argc, char *argv[])
{
	int i;
	quit = 0;

	for (i = 1; i < argc; i++)
	{
		if ((!strcmp(argv[i], "--help")) || (!strcmp(argv[i], "-help")) || (!strcmp(argv[i], "-h")))
		{
			printf("Shisen-Seki v%d.%d.%d, Copyright (c) 2015 Artur Rojek.\nThis program is MIT licensed.\n", PROGRAM_MAJOR_VERSION, PROGRAM_MINOR_VERSION, PROGRAM_PATCH_VERSION);
			return 0;
		}
	}

	if (init())
	{
		quit = 1;
	}

	srand(time(NULL));

	while (!quit)
	{
		if (!frameLimiter())
		{
			input();
			logic();
			draw();
		}
	}

	storeConfig();
	deinit();

	return 0;
}
