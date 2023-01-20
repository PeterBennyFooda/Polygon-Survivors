#pragma once
#include "EntityFactory.h"
#include "GlobalGameSettings.h"

class EnemySpawner
{
private:
	const int Xmax = ScreenWidth;
	const int Ymax = ScreenHeight;
	const int Xmin = 0;
	const int Ymin = 0;
	const int dangerRadius = Xmax;
	sf::Vector2f center { ScreenWidth / 2.f, ScreenHeight / 2.f };

	EntityFactory& factory;
	sf::RenderWindow& window;

	std::random_device randDevice {};
	std::default_random_engine randGenerator { randDevice() };

public:
	EnemySpawner(EntityFactory& mFactory, sf::RenderWindow& mWindow) :
		factory(mFactory),
		window(mWindow)
	{}

	void SetCenter(sf::Vector2f& center);

	void GenerateEnemy(int count);
	void GenerateEnemy(EnemySpawnMode mode);
	void GenerateEnemy(int count, EnemySpawnMode mode);

	void GenerateChasers(int count);
	void GenerateCowards(int count);
	void GeneratePongs(int count);
	void GenerateChargers(int count);

	int RandomX();
	int RandomY();
	int RandomSign();
	void CheckTooClose(int& x, int& y);
};
