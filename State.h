#pragma once

#include "Entity.h"

class State
{
private:

protected:
	sf::RenderWindow* window;
	std::vector<sf::Texture> textures;
	bool quit;

public:
	State(sf::RenderWindow* window);
	virtual ~State();

	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;
	virtual void checkForQuit();
	virtual void updateKeybinds(const float& deltaTime) = 0;
	const bool& getQuit();
	virtual void exit() = 0;
	

};
