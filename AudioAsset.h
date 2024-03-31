#pragma once
#include "Asset.h"
class AudioAsset : public Asset
{
private:
	sf::SoundBuffer* buffer;
public:
	bool Load(const string& filename);
	void Release();
	sf::SoundBuffer* GetBuffer();
};

