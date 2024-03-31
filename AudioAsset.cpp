#include "AudioAsset.h"

bool AudioAsset::Load(const string& filename)
{
	this->buffer = new sf::SoundBuffer();
	if (!this->buffer->loadFromFile(filename))
	{
		std::cerr << "Failed to load audio file." << std::endl;
		return false;
	}
	
	return true;
}

void AudioAsset::Release()
{
}

sf::SoundBuffer* AudioAsset::GetBuffer()
{
	return this->buffer;
}
