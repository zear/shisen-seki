#ifndef _FONT_H_
#define _FONT_H_

#include "tileset.h"

typedef struct font
{
	tileset tiles;
	int w;
	int h;
	int tracking;
	int leading;
} font;

extern font gameFontRegular;
extern font gameFontSelected;

int fontLoad(font *fontObj, char *filename, int glyphWidth, int glyphHeight, int tracking, int leading);
void fontUnload(font *fontObj);
void dTextCentered(font *fontObj, char *string, int y);
void dText(font *fontObj, char *string, int x, int y);

#endif /* _FONT_H_ */
