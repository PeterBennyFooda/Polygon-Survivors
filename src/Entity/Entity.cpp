#include "Entity.h"

Entity::Entity(int id, std::string name)
{
	this->id = id;
	this->name = name;
}

Entity::~Entity()
{
}

void Entity::SetPosition(float x, float y)
{
	xPos = x;
	yPos = y;
}

void Entity::AddComponent()
{
}