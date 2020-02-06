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
		punch->volume = 75;
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}

	if (!(hit = Mix_LoadWAV(hitPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}

	if (!(whip_start = Mix_LoadWAV(whip_startPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}

	if (!(whip_hit = Mix_LoadWAV(whip_hitPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}

	if (!(whip_miss = Mix_LoadWAV(whip_missPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}

	if (!(jump = Mix_LoadWAV(jumpPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}

	if (!(land = Mix_LoadWAV(landPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}

	if (!(midnaLament = Mix_LoadMUS(midnaLamentPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}

	if (!(platformLand = Mix_LoadWAV(platformLandPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}

	if (!(chainSound = Mix_LoadWAV(chainPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
}

void SNAudioManager::PlayChunkOnce(Mix_Chunk* chunk)
{
	if (Mix_PlayChannel(-1, chunk, 0) == -1)
		printf("Mix_PlayChannel: %s\n", Mix_GetError());
}

void SNAudioManager::LoopChunk(Mix_Chunk* chunk)
{
	if (Mix_PlayChannel(-2, chunk, 0) == -1)
		printf("Mix_LoopChunk: %s\n", Mix_GetError());
}

void SNAudioManager::StopLoopigChunk(Mix_Chunk* chunk)
{
	if (Mix_HaltChannel(-2) == -1)
		printf("Mix_StopLoopigChunk: %s\n", Mix_GetError());
}


void SNAudioManager::PlayMusicLoop(Mix_Music* song)
{
	if (Mix_PlayMusic(song, -1) == -1)
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
}
