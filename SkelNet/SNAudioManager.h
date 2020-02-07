#pragma once
#include "SDL_mixer.h"

class SNAudioManager
{
public:
	//When adding sounds and music 
	//1. Create a Mix variable and a path variable
	//2. Init the sound in InitSounds by loading the path

	SNAudioManager();

	Mix_Music* music;
	const char* musicPath = "Song.wav";
	
	Mix_Chunk* punch;
	const char* punchPath = "Puffer1.wav";

	Mix_Chunk* hit;
	const char* hitPath = "Puffer2.wav";

	Mix_Chunk* whip_start;
	Mix_Chunk* whip_hit;
	Mix_Chunk* whip_miss;
	Mix_Chunk* jump;
	Mix_Chunk* land;
	Mix_Chunk* chainSound;
	Mix_Chunk* platformLand;
	Mix_Chunk* dootSound1;
	Mix_Chunk* dootSound2;
	Mix_Chunk* dootSound3;
	Mix_Chunk* dootSound4;
	Mix_Music* midnaLament;
	Mix_Chunk* deathSound;
	Mix_Chunk* smokeThrowSound; 

	const char* whip_startPath = "Whip_StartUp.wav";
	const char* whip_hitPath = "Whip_Hit.wav";
	const char* whip_missPath = "Whip_Miss.wav";
	const char* jumpPath = "Jump.wav";
	const char* landPath = "Land.wav";
	const char* midnaLamentPath = "Midnas_Lament_-_Twilight_Princess_8-Bit.mp3";
	const char* chainPath = "ChainsRatteling.wav";
	const char* platformLandPath = "PlatformLanded.wav";
	const char* dootPath = "Doot.wav";
	const char* dootPath2 = "Doot2.wav";
	const char* dootPath3 = "Doot3.wav";
	const char* dootPath4 = "Doot4.wav";
	const char* deathPath = "Death.wav";
	const char* smokeThrowPath = "SmokeThrow.wav";

	void InitSounds();
	void PlayChunkOnce(Mix_Chunk* chunk, int channel = -1);
	void LoopChunk(Mix_Chunk* chunk, int channel);
	void StopChannel(int channel);
	void PlayMusicLoop(Mix_Music* song);
};
