#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include <vector>

#define DEFAULT_MUSIC_FADE_TIME 0.0f

struct _Mix_Music;
struct Mix_Chunk;

class SoundEffect {
public:
	SoundEffect(const char* path);
	~SoundEffect();

	string path;
	Mix_Chunk* fx;
};

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	void StopMusic();

	void PauseMusic();
	void ResumeMusic();

	void DefaultVolume();
	void ChangeVolume(int volume);
	bool MusicPlaying();

	void RestartAudio();

private:

	_Mix_Music*			    music = NULL;
	std::list<SoundEffect*>	fx;

	int def_volume = 75;
};

#endif // __j1AUDIO_H__
