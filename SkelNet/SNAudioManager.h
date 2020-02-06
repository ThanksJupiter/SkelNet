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
	Mix_Music* midnaLament;

	const char* whip_startPath = "Whip_StartUp.wav";
	const char* whip_hitPath = "Whip_Hit.wav";
	const char* whip_missPath = "Whip_Miss.wav";
	const char* jumpPath = "Jump.wav";
	const char* landPath = "Land.wav";
	const char* midnaLamentPath = "Midnas_Lament_-_Twilight_Princess_8-Bit.mp3";
	const char* chainPath = "ChainsRatteling.wav";
	const char* platformLandPath = "PlatformLanded.wav";

	void InitSounds();
	void PlayChunkOnce(Mix_Chunk* chunk);
	void LoopChunk(Mix_Chunk* chunk);
	void StopLoopigChunk();
	void PlayMusicLoop(Mix_Music* song);
};
