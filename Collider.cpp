#include "Collider.h"

Collider::Collider(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

Collider::~Collider()
{
}

void Collider::update(const float& deltaTime)
{
}

void Collider::onCollide(const Collider other)
{
}

void Collider::checkClick(sf::Vector2f mousePos)
{
}

void Collider::onClick()
{
}
