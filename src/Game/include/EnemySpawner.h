#pragma once
#include "EntityFactory.h"

class EnemySpawner
{
private:
	EntityFactory& factory;
	sf::RenderWindow& window;

public:
	EnemySpawner(EntityFactory& mFactory, sf::RenderWindow& mWindow) :
		factory(mFactory),
		window(mWindow)
	{}

	void GenerateEnemy(int count);
	void GenerateEnemy(EnemySpawnMode mode);
	void GenerateEnemy(int count, EnemySpawnMode mode);
};
