#include "ImageAsset.h"

bool ImageAsset::Load(const string& filename)
{
	if (!this->texture.loadFromFile(filename))
	{
		std::cerr << "Failed to load image file." << std::endl;
		return false;
	}
	return true;
}

void ImageAsset::Release()
{
}
