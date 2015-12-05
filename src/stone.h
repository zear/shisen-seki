#ifndef _STONE_H_
#define _STONE_H_

typedef enum stoneType
{
	STONE_EMPTY = 0,
	DOT_ONE,
	DOT_TWO,
	DOT_THREE,
	DOT_FOUR,
	DOT_FIVE,
	DOT_SIX,
	DOT_SEVEN,
	DOT_EIGHT,
	DOT_NINE,
	BAMBOO_ONE,
	BAMBOO_TWO,
	BAMBOO_THREE,
	BAMBOO_FOUR,
	BAMBOO_FIVE,
	BAMBOO_SIX,
	BAMBOO_SEVEN,
	BAMBOO_EIGHT,
	BAMBOO_NINE,
	CHAR_ONE,
	CHAR_TWO,
	CHAR_THREE,
	CHAR_FOUR,
	CHAR_FIVE,
	CHAR_SIX,
	CHAR_SEVEN,
	CHAR_EIGHT,
	CHAR_NINE,
	WIND_EAST,
	WIND_SOUTH,
	WIND_WEST,
	WIND_NORTH,
	DRAGON_RED,
	DRAGON_GREEN,
	DRAGON_WHITE,
	SEASON_SPRING,
	SEASON_SUMMER,
	SEASON_AUTUMN,
	SEASON_WINTER,
	FLOWER_ONE,
	FLOWER_TWO,
	FLOWER_THREE,
	FLOWER_FOUR,
	STONE_COUNT
} stoneType;

typedef struct stone
{
	stoneType type;
	int alpha;
	int x;
	int y;
} stone;

int stoneAmountOfStonesOfType(stoneType type);
int stoneCheckMatchingTypes(stoneType typeA, stoneType typeB);

#endif /* _STONE_H_ */
