#pragma once
#include "EntityFactory.h"
#include "GlobalGameSettings.h"

class EnemySpawner
{
private:
	const int Xmax = ScreenWidth / 2 + 100;
	const int Ymax = ScreenHeight / 2 + 100;
	const int min = 0;
	const int dangerRadius = Xmax;
	const sf::Vector2f center { ScreenWidth / 2.f, ScreenHeight / 2.f };

	EntityFactory& factory;
	sf::RenderWindow& window;

	std::random_device randDevice {};
	std::default_random_engine randGenerator { randDevice() };

public:
	EnemySpawner(EntityFactory& mFactory, sf::RenderWindow& mWindow) :
		factory(mFactory),
		window(mWindow)
	{}

	void Init();

	void GenerateEnemy(int count);
	void GenerateEnemy(EnemySpawnMode mode);
	void GenerateEnemy(int count, EnemySpawnMode mode);

	int RandomX();
	int RandomY();
	int RandomSign();
	void CheckTooClose(int& x, int& y);
};
