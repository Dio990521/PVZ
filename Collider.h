#pragma once
#include "GameState.h"
class Collider
{
private:
	int x, y;
	int width, height;

public:
	Collider(int x, int y, int width, int height);
	~Collider();

	void update(const float& deltaTime);
	void onCollide(const Collider other);
	void checkClick(sf::Vector2f mousePos);
	void onClick();
};

