#include "AudioAsset.h"

bool AudioAsset::Load(const string& filename)
{
	if (!this->audio.loadFromFile(filename))
	{
		std::cerr << "Failed to load audio file." << std::endl;
		return false;
	}
	return true;
}

void AudioAsset::Release()
{
}
