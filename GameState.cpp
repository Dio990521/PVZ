#include "GameState.h"

GameState::GameState(sf::RenderWindow* window) : State(window)
{

}

GameState::~GameState()
{
}

void GameState::update(const float& deltaTime)
{
	this->updateKeybinds(deltaTime);
	this->player.Update(deltaTime);
}

void GameState::render(sf::RenderTarget* target)
{
	this->player.Render(this->window);
}

void GameState::checkForEnd()
{
}

void GameState::updateKeybinds(const float& deltaTime)
{
	this->checkForQuit();
}

void GameState::exit()
{
	std::cout << "End Game State ! fuck you !" << std::endl;
}
