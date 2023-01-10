#pragma once
#include "Component/IComponent.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <ctime>
#include <map>
#include <stack>

class Entity
{
public:
	Entity(int id, std::string name);
	~Entity();

	//Functions
	void SetPosition(float x, float y);
	void AddComponent();
	void Render();

private:
	int id;
	float xPos, yPos;
	std::string name;
	std::vector<IComponent> components;
};