#pragma once
#include "Asset.h"
class AudioAsset : public Asset
{
private:
	sf::SoundBuffer audio;
public:
	bool Load(const string& filename);
	void Release();
};

