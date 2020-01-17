#pragma once
#include "SDL_mixer.h"

class SNAudioManager
{
public:
	//When adding sounds and music 
	//1. Create a Mix variable and a path variable
	//2. Init the sound in InitSounds

	Mix_Music* music;
	char* musicPath;
	
	Mix_Chunk* punch;
	char* punchPath; 
	
	Mix_Chunk* hitSound;
	char* hitSoundPath;

	void InitSounds();
	void PlayChunk(Mix_Chunk* chunk);
	void PlayMusic(Mix_Music* song);
};
