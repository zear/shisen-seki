#include "input.h"

#include <SDL.h>
#include "main.h"

SDL_Event event;
int keys[2048];
int joyCanMoveX = 1;
int joyCanMoveY = 1;

void input()
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				quit = 1;
			break;
			case SDL_KEYDOWN:			// Button press
				keys[event.key.keysym.sym] = 1;
			break;
			case SDL_KEYUP:				// Button release
				keys[event.key.keysym.sym] = 0;
			break;
			case SDL_JOYAXISMOTION:			// Analog joystick movement
				switch (event.jaxis.axis)
				{
					case 0:		// axis 0 (left-right)
						if (event.jaxis.value < -JOY_DEADZONE)
						{
							if (joyCanMoveX)
							{
								joyCanMoveX = 0;
								// left movement
								keys[SDLK_LEFT] = 1;
								keys[SDLK_RIGHT] = 0;
							}
						}
						else if (event.jaxis.value > JOY_DEADZONE)
						{
							if (joyCanMoveX)
							{
								joyCanMoveX = 0;
								// right movement
								keys[SDLK_LEFT] = 0;
								keys[SDLK_RIGHT] = 1;
							}
						}
						else
						{
							joyCanMoveX = 1;
							keys[SDLK_LEFT] = 0;
							keys[SDLK_RIGHT] = 0;
						}
					break;
					case 1:		// axis 1 (up-down)
						if (event.jaxis.value < -JOY_DEADZONE)
						{
							if (joyCanMoveY)
							{
								joyCanMoveY = 0;
								// up movement
								keys[SDLK_UP] = 1;
								keys[SDLK_DOWN] = 0;
							}
						}
						else if (event.jaxis.value > JOY_DEADZONE)
						{
							if (joyCanMoveY)
							{
								joyCanMoveY = 0;
								// down movement
								keys[SDLK_UP] = 0;
								keys[SDLK_DOWN] = 1;
							}
						}
						else
						{
							joyCanMoveY = 1;
							keys[SDLK_UP] = 0;
							keys[SDLK_DOWN] = 0;
						}
					break;

					default:
					break;
				}
			break;

			default:
			break;
		}
	}
}
