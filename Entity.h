#pragma once

#include<ctime>
#include<cstdlib>
#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<stack>
#include<map>

#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"
#include "SFML\System.hpp"
#include "SFML\Window.hpp"
#include "SFML\Network.hpp"

class Entity
{
private:

protected:
	sf::RectangleShape shape;
	float movementSpeed;

public:
	Entity();
	virtual ~Entity();

	virtual void update(const float& deltaTime);
	virtual void render(sf::RenderTarget* target);

	virtual void move(const float& deltaTime, const float dirX, const float dirY);
};

