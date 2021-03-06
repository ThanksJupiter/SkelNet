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

	//music

	if (!(music = Mix_LoadMUS(musicPath)))
	{
		fprintf(stderr, "Unable to Find music source: %s\n", Mix_GetError()); exit(1);
	}

	if (!(midnaLament = Mix_LoadMUS(midnaLamentPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}

	//Chunks

	//Attacks

	if (!(punch = Mix_LoadWAV(punchPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	punch->volume = 75;

	if (!(hit = Mix_LoadWAV(hitPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	hit->volume = 5;

	if (!(whip_start = Mix_LoadWAV(whip_startPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	whip_start->volume = 25;

	if (!(whip_hit = Mix_LoadWAV(whip_hitPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	whip_hit->volume = 5;

	if (!(whip_miss = Mix_LoadWAV(whip_missPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	whip_miss->volume = 35;

	//Movement

	if (!(jump = Mix_LoadWAV(jumpPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	jump->volume = 128;

	if (!(land = Mix_LoadWAV(landPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	land->volume = 128;

	//Platform

	if (!(platformLand = Mix_LoadWAV(platformLandPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	platformLand->volume = 105;

	if (!(chainSound = Mix_LoadWAV(chainPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}

	if (!(chainSound = Mix_LoadWAV(chainPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	chainSound->volume = 128;

	//Doot
	if (!(dootSound1 = Mix_LoadWAV(dootPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	dootSound1->volume = 105;

	if (!(dootSound2 = Mix_LoadWAV(dootPath2)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	dootSound2->volume = 105;

	if (!(dootSound3 = Mix_LoadWAV(dootPath3)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	dootSound3->volume = 105;

	if (!(dootSound4 = Mix_LoadWAV(dootPath4)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	dootSound4->volume = 105;

	//Death
	if (!(deathSound = Mix_LoadWAV(deathPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	deathSound->volume = 105;
	
	if (!(deathExplosionSound = Mix_LoadWAV(deathExplosionPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	deathExplosionSound->volume = 105;

	if (!(smokeThrowSound = Mix_LoadWAV(smokeThrowPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	smokeThrowSound->volume = 105;

	if (!(gameVoiceSound = Mix_LoadWAV(gameVoicePath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	gameVoiceSound->volume = 115;

	if (!(cigLand = Mix_LoadWAV(cigLandPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	cigLand->volume = 115;

	if (!(cigLight = Mix_LoadWAV(cigLightPath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	cigLight->volume = 115;

	if (!(inhale = Mix_LoadWAV(inhalePath)))
	{
		fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
	}
	inhale->volume = 115;
}

void SNAudioManager::PlayChunkOnce(Mix_Chunk* chunk, int channel)
{
	if (Mix_PlayChannel(channel, chunk, 0) == -1)
		printf("Mix_PlayChannel: %s\n", Mix_GetError());
}

void SNAudioManager::LoopChunk(Mix_Chunk* chunk, int channel)
{
	if (Mix_PlayChannel(1, chunk, 0) == -1)
		printf("Mix_LoopChunk: %s\n", Mix_GetError());
}

void SNAudioManager::StopChannel(int channel)
{
	if (Mix_HaltChannel(1) == -1)
		printf("Mix_StopLoopigChunk: %s\n", Mix_GetError());
}


void SNAudioManager::PlayMusicLoop(Mix_Music* song)
{
	if (Mix_PlayMusic(song, -1) == -1)
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
}
