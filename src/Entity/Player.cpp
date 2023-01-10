#include "Player.h"

Player::Player()
{
	this->sprite = new Sprite2DComponent(texturePath);
}

Player::~Player()
{
	delete this->sprite;
}

void Player::Update()
{
}

void Player::Render(sf::RenderWindow* target)
{
	this->sprite->Render(target);
}

void Player::Init()
{
}
