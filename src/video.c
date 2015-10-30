#include "video.h"

#include <SDL.h>

SDL_Surface *screen;
Uint32 curTicks;
Uint32 lastTicks = 0;
int blinkTimer = 0;
int blinkTimerSlow = 0;

int initSDL()
{
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK))
	{
		return -1;
	}

	SDL_WM_SetCaption("Shisen-Seki", NULL);
	SDL_ShowCursor(SDL_DISABLE);

	screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);

	if(screen == NULL)
	{
		return -1;
	}

	if(SDL_NumJoysticks() > 0)
	{
		SDL_JoystickOpen(0);
	}

	return 0;
}

void deinitSDL()
{
	if(SDL_NumJoysticks() > 0)
	{
		SDL_JoystickClose(0);
	}

	SDL_Quit();
}

SDL_Surface *loadImage(char *fileName)
{
	SDL_Surface *loadedImage;
	SDL_Surface *optimizedImage;
	Uint32 colorKey;

	if (!fileName)
	{
		fprintf(stderr, "ERROR: Filename is empty.");
		return NULL;
	}

	loadedImage = SDL_LoadBMP(fileName);

	if (!loadedImage)
	{
		fprintf(stderr, "ERROR: Failed to load image: %s\n", fileName);
		return NULL;
	}

	optimizedImage = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_DOUBLEBUF, loadedImage->w, loadedImage->h, SCREEN_BPP, 0, 0, 0, 0);
	SDL_BlitSurface(loadedImage, NULL, optimizedImage, NULL);
	SDL_FreeSurface(loadedImage);

	if (!optimizedImage)
	{
		fprintf(stderr, "ERROR: Failed to optimize image: %s\n", fileName);
		return NULL;
	}

	colorKey = SDL_MapRGB(optimizedImage->format, 255, 0, 255); // Set transparency to magenta.
	SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorKey);

	return optimizedImage;
}

void clipImage(SDL_Rect *source, int tileWidth, int tileHeight, int rowLength, int numOfTiles)
{
	int i;
	int j;
	int k;
	int l;

	for(i = 0, k = 0; k < numOfTiles; i+= tileHeight)
	{
		for(j = 0, l = 0; l < rowLength; j+= tileWidth)
		{
			if (k >= numOfTiles)
			{
				return;
			}
			source[k].x = j;
			source[k].y = i;
			source[k].w = tileWidth;
			source[k].h = tileHeight;
			++k;
			++l;
		}
		l = 0;
	}
}

void drawImage(SDL_Surface *source, SDL_Rect *clip, SDL_Surface *destination, int x, int y)
{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}

void drawRectangle(SDL_Surface *source, int x, int y, int w, int h)
{
	SDL_Rect r;
	int rcolor = SDL_MapRGB(screen->format, 0, 0, 255);

	// Top wall.
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = 1;
	SDL_FillRect(source, &r, rcolor);

	// Bottom wall.
	r.x = x;
	r.y = y + h - 1;
	r.w = w;
	r.h = 1;
	SDL_FillRect(source, &r, rcolor);

	// Left wall.
	r.x = x;
	r.y = y;
	r.w = 1;
	r.h = h;
	SDL_FillRect(source, &r, rcolor);

	// Right wall.
	r.x = x + w - 1;
	r.y = y;
	r.w = 1;
	r.h = h;
	SDL_FillRect(source, &r, rcolor);
}

int frameLimiter()
{
	int t;

#if defined(NO_FRAMELIMIT)
	return 0;
#endif

	curTicks = SDL_GetTicks();
	t = curTicks - lastTicks;

	if(t >= 1000/FPS)
	{
		lastTicks = curTicks;
		return 0;
	}

	SDL_Delay(1);
	return 1;
}

void blinkTimersTick()
{
	if(++blinkTimer > 20)
	{
		blinkTimer = 0;
	}
	if(++blinkTimerSlow > 40)
	{
		blinkTimerSlow = 0;
	}
}

void clearScreen()
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
}
