#pragma once
#include "IComponent.h"

class Sprite2DComponent : public IComponent
{
private:
	sf::Texture texture;
	sf::Sprite sprite;

	//Functions
	bool SetTexture(std::string filepath);
	void SetSprite();

public:
	Sprite2DComponent(std::string filepath);
	virtual ~Sprite2DComponent();

	//Functions
	void Render(sf::RenderWindow* window);
};
