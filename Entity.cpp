#include "Entity.h"

Entity::Entity()
{
	this->shape.setSize(sf::Vector2f(50.f, 50.f));
	this->shape.setFillColor(sf::Color::White);
	this->movementSpeed = 100.f;
}

Entity::~Entity()
{
}

void Entity::update(const float& deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->move(deltaTime, -1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->move(deltaTime, 1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->move(deltaTime, 0.f, -1.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->move(deltaTime, 0.f, 1.f);
}

void Entity::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
}

void Entity::move(const float& deltaTime, const float dirX, const float dirY)
{
	this->shape.move(dirX * this->movementSpeed * deltaTime,
		dirY * this->movementSpeed * deltaTime);
}
