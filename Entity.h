#pragma once

#include<ctime>
#include<cstdlib>
#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<stack>
#include<map>
#include<string>
#include<unordered_map>
using std::string;

#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"
#include "SFML\System.hpp"
#include "SFML\Window.hpp"
#include "SFML\Network.hpp"

class Entity
{
private:
	int x, y;
	int width, height;
	int frameIndex;
protected:
	sf::RectangleShape shape;
	float movementSpeed;
	sf::Sprite sprite;

public:
	Entity();
	virtual ~Entity();

	virtual void update(const float& deltaTime);
	virtual void render(sf::RenderTarget* target);

	virtual void move(const float& deltaTime, const float dirX, const float dirY);
};

