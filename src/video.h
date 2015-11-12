#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <SDL.h>

#define SCREEN_W		320
#define SCREEN_H		240
#define SCREEN_BPP		16
#define FPS			60

extern int quit;
extern SDL_Surface *screen;
extern int blinkTimer;
extern int blinkTimerSlow;

int initSDL();
void deinitSDL();
SDL_Surface *loadImage(char *fileName);
void clipImage(SDL_Rect *source, int tileWidth, int tileHeight, int rowLength, int numOfTiles);
void drawImage(SDL_Surface *source, SDL_Rect *clip, SDL_Surface *destination, int x, int y);
void drawRectangle(SDL_Surface *source, int x, int y, int w, int h, int color);
int frameLimiter();
void blinkTimersTick();
void clearScreen();

#endif /* _VIDEO_H_ */
