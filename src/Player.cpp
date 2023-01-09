#include "Player.h"

Player::Player()
{
	if (SetTexture())
	{
		SetSprite();
	}
}

Player::~Player()
{
}

void Player::Update()
{
}

void Player::Render(sf::RenderTarget& target)
{
	target.draw(playerSprite);
}

bool Player::SetTexture()
{
	if (!playerTexture.loadFromFile("Resources/Texture/Character/player.png"))
	{
		std::cout << "Error! Player texture not found!" << std::endl;
		return false;
	}
	else
	{
		return true;
	}
}

void Player::SetSprite()
{
	playerSprite.setTexture(playerTexture);
	playerSprite.scale(sf::Vector2f(0.25f, 0.25f));
	playerSprite.setOrigin(sf::Vector2f(playerSprite.getTexture()->getSize().x * 0.5f, playerSprite.getTexture()->getSize().y * 0.5f));
}