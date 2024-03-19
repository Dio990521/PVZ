#pragma once

#include "State.h"

class GameState : public State
{
private:
	Entity player;

public:
	GameState(sf::RenderWindow* window);
	virtual ~GameState();

	void update(const float& deltaTime);
	void render(sf::RenderTarget* target = nullptr);
	void checkForEnd();
	void updateKeybinds(const float& deltaTime);
	void exit();
};

