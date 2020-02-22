#pragma once
#include <SDL_mixer.h>
#include <string>
#include <list>

using namespace std;

//new datatype made up of a sound and a look up name
struct SoundListing{
	Mix_Chunk* sound;
	string name;
};

class SoundManager
{
public:
	list<SoundListing> sounds;

	SoundManager();
	~SoundManager();
	void loadSound(string name, string file);
	void playSound(string name);

	//singleton pattern
	static SoundManager soundManager;
};

