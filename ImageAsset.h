#pragma once
#include "Asset.h"
class ImageAsset : public Asset
{
private:
	sf::Sprite* sprite;
public:
	bool Load(const string& filename);
	void Release();
	sf::Sprite* GetSprite();
};

