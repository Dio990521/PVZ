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
	static std::unordered_map<string, Entity> entities;

	void initWindow();
	void initStates();

public:
	GameEngine();
	virtual ~GameEngine();

	static std::unordered_map<string, Entity>& GetEntities();

	void endApplication();

	//Functions
	void AwakeEntities();
	void UpdateEntities();
	void DestroyEntites();
	void updateDeltaTime();
	void updateSFMLEvents();
	void update();
	void render();
	void run();
};

#endif
