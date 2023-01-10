#include "Sprite2DComponent.h"

Sprite2DComponent::Sprite2DComponent(std::string filepath)
{
	if (SetTexture(filepath))
	{
		SetSprite();
	}
}

Sprite2DComponent::~Sprite2DComponent()
{
}

bool Sprite2DComponent::SetTexture(std::string filepath)
{
	if (!texture.loadFromFile(filepath))
	{
		std::cout << "Error! Player texture not found!" << std::endl;
		return false;
	}
	else
	{
		return true;
	}
}

void Sprite2DComponent::SetSprite()
{
	sprite.setTexture(texture);
	sprite.scale(sf::Vector2f(1.f, 1.f));
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5f, sprite.getTexture()->getSize().y * 0.5f));
}

void Sprite2DComponent::Render(sf::RenderWindow* target)
{
	target->draw(sprite);
}