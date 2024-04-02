#pragma once
#include "Entity.h"
class AudioManager
{
private:
	std::vector<sf::Sound*> sounds;
	std::vector<sf::Music*> musics;
public:
	void PlaySFX(const string& id, const int volume = 100);
	void PlayBGM(const string& id, const int volume = 100);
	void Update();
};

