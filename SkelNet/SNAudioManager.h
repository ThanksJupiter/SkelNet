#pragma once
#include "SDL_mixer.h"

class SNAudioManager
{
public:
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
