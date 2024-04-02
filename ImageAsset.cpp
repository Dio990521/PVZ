#include "ImageAsset.h"

bool ImageAsset::Load(const string& filename)
{
	sf::Texture texture;
	if (!texture.loadFromFile(filename))
	{
		std::cerr << "Failed to load image file." << std::endl;
		return false;
	}
	this->sprite = new sf::Sprite(texture);
	return true;
}

void ImageAsset::Release()
{
	delete this->sprite;
}

sf::Sprite* ImageAsset::GetSprite()
{
	return this->sprite;
}
