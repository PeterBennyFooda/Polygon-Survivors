#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Player
{
private:
	sf::Texture playerTexture;
	sf::Sprite playerSprite;

	//Functions
	bool SetTexture();
	void SetSprite();

public:
	Player();
	virtual ~Player();

	//Functions
	void Update();
	void Render(sf::RenderTarget& target);
};