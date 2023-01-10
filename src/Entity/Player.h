#pragma once
#include "Component/Sprite2DComponent.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Player
{
private:
	const std::string texturePath = "Resources/Texture/Character/player.png";
	Sprite2DComponent* sprite;

	//Functions
	void Init();

public:
	Player();
	virtual ~Player();

	//Functions
	void Update();
	void Render(sf::RenderWindow* target);
};