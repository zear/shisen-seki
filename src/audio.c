#include "audio.h"

#include <SDL_mixer.h>

Mix_Music *bgdMusic = NULL;

int initAudio()
{
	int flags = MIX_INIT_OGG;

	if ((Mix_Init(flags)&flags) != flags)
	{
		return -1;
	}

	if (Mix_OpenAudio(AUDIO_SAMPLE_RATE, MIX_DEFAULT_FORMAT, 2, AUDIO_CHUNK_SIZE))
	{
		return -1;
	}

	return 0;
}

void deinitAudio()
{
	unloadMusic(&bgdMusic);

	Mix_CloseAudio();
	Mix_Quit();
}

Mix_Music *loadMusic(Mix_Music *track, char *fileName)
{
	track = Mix_LoadMUS(fileName);

	return track;
}

void unloadMusic(Mix_Music **track)
{
	Mix_FreeMusic(*track);
	*track = NULL;
}

void playMusic(Mix_Music *track)
{
	if (!Mix_PlayingMusic())
	{
		Mix_PlayMusic(track, -1);
	}
}

void pauseMusic()
{
	if (Mix_PlayingMusic())
	{
		Mix_PauseMusic();
	}
}
