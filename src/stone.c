#include "stone.h"

int stoneAmountOfStonesOfType(stoneType type)
{
	switch (type)
	{
		case SEASON_SPRING:
		case SEASON_SUMMER:
		case SEASON_AUTUMN:
		case SEASON_WINTER:
		case FLOWER_ONE:
		case FLOWER_TWO:
		case FLOWER_THREE:
		case FLOWER_FOUR:
		return 1;

		default:
		return 4;
	}
}

int stoneCheckMatchingTypes(stoneType typeA, stoneType typeB)
{
	if (typeA == typeB)
	{
		return 1;
	}
	else if (typeA >= SEASON_SPRING && typeA <= SEASON_WINTER && typeB >= SEASON_SPRING && typeB <= SEASON_WINTER)
	{
		return 1;
	}
	else if (typeA >= FLOWER_ONE && typeA <= FLOWER_FOUR && typeB >= FLOWER_ONE && typeB <= FLOWER_FOUR)
	{
		return 1;
	}

	return 0;
}
