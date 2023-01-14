#include "include/EnemySpawner.h"
using namespace std;

void EnemySpawner::GenerateEnemy(int count)
{
	GenerateEnemy(count, EnemySpawnMode::Easy);
}
void EnemySpawner::GenerateEnemy(EnemySpawnMode mode)
{
	GenerateEnemy(5, mode);
}
void EnemySpawner::GenerateEnemy(int count, EnemySpawnMode mode)
{
	default_random_engine generator(time(NULL));
	uniform_real_distribution<float> unif(-300, 300);
	float randomOffestX = unif(generator);
	float randomOffestY = unif(generator);

	//Init chasers.
	for (int i = 0; i < count; i++)
	{
		factory.CreateEnemy(sf::Vector2f(ScreenWidth / 2 + randomOffestX, ScreenHeight / 2 + randomOffestY), window);
		randomOffestX = unif(generator);
		randomOffestY = unif(generator);
	}
	if (mode == EnemySpawnMode::Easy)
		return;

	//Init cowards.
	for (int i = 0; i < count; i++)
	{
		factory.CreateEnemy(sf::Vector2f(ScreenWidth / 2 + randomOffestX, ScreenHeight / 2 + randomOffestY),
			window,
			5.f,
			EnemyMoveType::AvoidPlayer);
		randomOffestX = unif(generator);
		randomOffestY = unif(generator);
	}
	if (mode == EnemySpawnMode::Normal)
		return;

	//Init ping pong guys.
	for (int i = 0; i < count; i++)
	{
		factory.CreateEnemy(sf::Vector2f(ScreenWidth / 2 + randomOffestX, ScreenHeight / 2 + randomOffestY),
			window,
			0.75f,
			EnemyMoveType::PingPong);
		randomOffestX = unif(generator);
		randomOffestY = unif(generator);
	}
	if (mode == EnemySpawnMode::Hard)
		return;
}