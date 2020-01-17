#include "SNAudioManager.h"
#include <stdio.h>

SNAudioManager::SNAudioManager()
{
}

void SNAudioManager::InitSounds()
{
	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) != 0)
	{
		fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError()); exit(1);
	}

	if (!(music = Mix_LoadMUS(musicPath)))
	{
		fprintf(stderr, "Unable to Find music source: %s\n", Mix_GetError()); exit(1);
	}

	if (!(punch = Mix_LoadWAV(punchPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
}

void SNAudioManager::PlayChunkOnce(Mix_Chunk* chunk)
{
	if (Mix_PlayChannel(-1, chunk, 0) == -1)
		printf("Mix_PlayChannel: %s\n", Mix_GetError());
}

void SNAudioManager::PlayMusicLoop(Mix_Music* song)
{
	if (Mix_PlayMusic(song, -1) == -1)
		printf("Mix_PlayChannel: %s\n", Mix_GetError());
}
