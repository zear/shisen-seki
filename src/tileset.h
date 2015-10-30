#ifndef _TILESET_H_
#define _TILESET_H_

#include <SDL.h>

//#define TILESET_ROW_LENGTH	8

typedef struct tileset
{
	SDL_Surface *image;
	SDL_Rect *clip;
	int length;
} tileset;

int tilesetLoad(tileset *tSet, char *fileName, int width, int height, int rowLen, int length);
void tilesetUnload(tileset *tSet);

#endif /* _TILESET_H_ */
