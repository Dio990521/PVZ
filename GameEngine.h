#ifndef GAME_H
#define GAME_H

#include "GameState.h"


class GameEngine
{
private:

	sf::RenderWindow* window;
	sf::Event sfEvent;

	sf::Clock deltaTimeClock;
	float deltaTime;

	std::stack<State*> states;

	void initWindow();
	void initStates();

public:
	GameEngine();
	virtual ~GameEngine();

	void endApplication();

	//Functions
	void updateDeltaTime();
	void updateSFMLEvents();
	void update();
	void render();
	void run();
};

#endif
