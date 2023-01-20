#include "include/EnemySpawner.h"
using namespace std;

void EnemySpawner::SetCenter(sf::Vector2f& mCenter)
{
	center = mCenter;
}

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
	GenerateChasers(count);
	if (mode == EnemySpawnMode::Easy)
		return;

	GenerateCowards(count);
	if (mode == EnemySpawnMode::Normal)
		return;

	GeneratePongs(count);
	if (mode == EnemySpawnMode::Hard)
		return;
	GenerateChasers(count / 2);
}

void EnemySpawner::GenerateChasers(int count)
{
	int randomOffestX = RandomX() * RandomSign();
	int randomOffestY = RandomY() * RandomSign();

	//Init chasers.
	for (int i = 0; i < count; ++i)
	{
		factory.CreateEnemy(center + sf::Vector2f(randomOffestX, randomOffestY), window, 0.8f, EnemyBaseHealth);
		randomOffestX = RandomX();
		randomOffestY = RandomY();
		//cout << randomOffestX << "+" << center.x << "||" << randomOffestY << "+" << center.y << endl;
		CheckTooClose(randomOffestX, randomOffestY);
		randomOffestX *= RandomSign();
		randomOffestY *= RandomSign();
		//cout << randomOffestX << "+" << center.x << "==" << randomOffestY << "+" << center.y << endl;
	}
}
void EnemySpawner::GenerateCowards(int count)
{
	int randomOffestX = RandomX() * RandomSign();
	int randomOffestY = RandomY() * RandomSign();

	//Init cowards.
	for (int i = 0; i < count; ++i)
	{
		factory.CreateEnemy(center + sf::Vector2f(randomOffestX, randomOffestY),
			window,
			1.5f,
			EnemyMoveType::AvoidPlayer,
			EnemyBaseHealth * 2);
		randomOffestX = RandomX();
		randomOffestY = RandomY();
		CheckTooClose(randomOffestX, randomOffestY);
		randomOffestX *= RandomSign();
		randomOffestY *= RandomSign();
	}
}
void EnemySpawner::GeneratePongs(int count)
{
	int randomOffestX = RandomX() * RandomSign();
	int randomOffestY = RandomY() * RandomSign();

	for (int i = 0; i < count; ++i)
	{
		factory.CreateEnemy(center + sf::Vector2f(randomOffestX, randomOffestY),
			window,
			0.55f,
			EnemyMoveType::PingPong,
			EnemyBaseHealth * 2);

		randomOffestX = RandomX();
		randomOffestY = RandomY();
		CheckTooClose(randomOffestX, randomOffestY);
		randomOffestX *= RandomSign();
		randomOffestY *= RandomSign();
	}
}
void EnemySpawner::GenerateChargers(int count)
{
	int randomOffestX = RandomX() * RandomSign();
	int randomOffestY = RandomY() * RandomSign();

	for (int i = 0; i < count; ++i)
	{
		factory.CreateEnemy(center + sf::Vector2f(randomOffestX, randomOffestY),
			window,
			1.1f,
			EnemyMoveType::Charger,
			EnemyBaseHealth * 5);

		randomOffestX = RandomX();
		randomOffestY = RandomY();
		CheckTooClose(randomOffestX, randomOffestY);
		randomOffestX *= RandomSign();
		randomOffestY *= RandomSign();
	}
}

int EnemySpawner::RandomX()
{
	int result = 0;
	result = (rand() % (Xmax - Xmin + 1) + Xmin);

	return result;
}

int EnemySpawner::RandomY()
{
	int result = 0;
	result = (rand() % (Ymax - Ymin + 1) + Ymin);

	return result;
}

int EnemySpawner::RandomSign()
{
	uniform_int_distribution<int> unif(-1, 1);

	int sign = unif(randGenerator);
	sign = sign == 0 ? 1 : sign;

	return sign;
}

void EnemySpawner::CheckTooClose(int& x, int& y)
{
	if (x < dangerRadius && y < dangerRadius)
	{
		uniform_int_distribution<int> unif(-1, 1);
		int sign = unif(randGenerator);
		sign = sign == 0 ? 1 : sign;

		if (sign > 0)
			x = dangerRadius;
		else
			y = dangerRadius;
	}
}