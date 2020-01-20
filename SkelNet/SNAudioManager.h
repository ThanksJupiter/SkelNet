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
	void InitSounds();
	void PlayChunkOnce(Mix_Chunk* chunk);
	void PlayMusicLoop(Mix_Music* song);
};
