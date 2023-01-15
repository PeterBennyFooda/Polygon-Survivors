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
	int min = 100;
	int max = 300;
	int offset = 150;

	default_random_engine generator(time(NULL));
	uniform_int_distribution<int> unif(-1, 1);
	int sign = unif(generator);
	sign = sign == 0 ? 1 : sign;
	int randomOffestX = (rand() % (max - min + offset) + min) * sign;
	int randomOffestY = (rand() % (max - min + offset) + min) * sign;

	//Init chasers.
	for (int i = 0; i < count; i++)
	{
		factory.CreateEnemy(sf::Vector2f(ScreenWidth / 2 + randomOffestX, ScreenHeight / 2 + randomOffestY), window, 0.8f, 1);
		sign = unif(generator);
		sign = sign == 0 ? 1 : sign;
		randomOffestX = (rand() % (max - min + offset) + min) * sign;
		randomOffestY = (rand() % (max - min + offset) + min) * sign;
	}
	if (mode == EnemySpawnMode::Easy)
		return;

	//Init cowards.
	for (int i = 0; i < count; i++)
	{
		factory.CreateEnemy(sf::Vector2f(ScreenWidth / 2 + randomOffestX, ScreenHeight / 2 + randomOffestY),
			window,
			2.f,
			EnemyMoveType::AvoidPlayer,
			2);
		randomOffestX = (rand() % (max - min + offset) + min) * sign;
		randomOffestY = (rand() % (max - min + offset) + min) * sign;
	}
	if (mode == EnemySpawnMode::Normal)
		return;

	//Init ping pong guys.
	for (int i = 0; i < count; i++)
	{
		factory.CreateEnemy(sf::Vector2f(ScreenWidth / 2 + randomOffestX, ScreenHeight / 2 + randomOffestY),
			window,
			0.75f,
			EnemyMoveType::PingPong,
			3);
		randomOffestX = (rand() % (max - min + offset) + min) * sign;
		randomOffestY = (rand() % (max - min + offset) + min) * sign;
	}
	if (mode == EnemySpawnMode::Hard)
		return;
}