#pragma once
#include "Asset.h"
class ImageAsset : public Asset
{
private:
	sf::Texture texture;
public:
	bool Load(const string& filename);
	void Release();
};

