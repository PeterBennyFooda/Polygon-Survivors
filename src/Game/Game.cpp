#include "Game.h"
using namespace ComponentSystem;
using namespace std;

Game::Game()
{
	Init();
	InitPlayer();
	InitEnemy();
}

Game::~Game()
{
	delete this->window;
	delete this->entityFactory;
}

void Game ::Init()
{
	//Init the window.
	this->window = new sf::RenderWindow(sf::VideoMode(ScreentWidth, ScreenHeight), "Polygon Survivors", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(144);
	this->window->setVerticalSyncEnabled(false);
	platform.setIcon(this->window->getSystemHandle());

	//For calculating delta time.
	timePoint1 = std::chrono::steady_clock::now();
	timePoint2 = std::chrono::steady_clock::now();

	//Create entity manager and factory.
	this->entityFactory = new EntityFactory(manager);
}

void Game::InitPlayer()
{
	entityFactory->CreatePlayer(sf::Vector2f(ScreentWidth / 2, ScreenHeight / 2), this->window);
}

void Game::InitEnemy()
{
	default_random_engine generator(time(NULL));
	uniform_real_distribution<float> unif(-300, 300);
	float randomOffestX = unif(generator);
	float randomOffestY = unif(generator);

	//Init chasers.
	for (int i = 0; i < 5; i++)
	{
		entityFactory->CreateEnemy(sf::Vector2f(ScreentWidth / 2 + randomOffestX, ScreenHeight / 2 + randomOffestY), this->window);
		randomOffestX = unif(generator);
		randomOffestY = unif(generator);
	}

	//Init cowards.
	for (int i = 0; i < 5; i++)
	{
		entityFactory->CreateEnemy(sf::Vector2f(ScreentWidth / 2 + randomOffestX, ScreenHeight / 2 + randomOffestY),
			this->window,
			3.f,
			EnemyMoveType::AvoidPlayer);
		randomOffestX = unif(generator);
		randomOffestY = unif(generator);
	}

	//Init ping pong guys.
	for (int i = 0; i < 5; i++)
	{
		entityFactory->CreateEnemy(sf::Vector2f(ScreentWidth / 2 + randomOffestX, ScreenHeight / 2 + randomOffestY),
			this->window,
			1.5f,
			EnemyMoveType::PingPong);
		randomOffestX = unif(generator);
		randomOffestY = unif(generator);
	}
}

void Game::PollingEvent()
{
	sf::Event event;
	while (this->window->pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				this->window->close();
				break;
			default:
				break;
		}
	}
}

void Game::FixedUpdate()
{
	//Accumulating frame time into 'currentSlice'.
	currentSlice += lastFrameTime;

	//If 'currentSlice' is greater than or equals 'ftSlice',
	//we update the game until 'currentSlice' is less than
	//'ftSlice'.
	int loop = 0;
	for (; currentSlice >= ftSlice && loop < maxLoop; currentSlice -= ftSlice)
	{
		//Passing 'ftStep' instead of 'lastFrameTime' because
		//we want to ensure the ideal 'frame time' is constant.
		manager.Refresh();
		manager.Update(ftStep);
		loop++;
	}
}

void Game::Update()
{
	// manager.Refresh();
	// manager.Update(lastFrameTime);
}

void Game::Render()
{
	//clear previous frame
	this->window->clear();

	manager.Render();

	//draw the game
	this->window->display();
}

void Game::Run()
{
	//Game Loop
	while (this->window->isOpen())
	{
		timePoint1 = chrono::steady_clock::now();

		PollingEvent();
		FixedUpdate();
		Update();
		Render();

		timePoint2 = chrono::steady_clock::now();
		auto elapsedTime(timePoint2 - timePoint1);
		float frameTime {
			chrono::duration_cast<chrono::duration<float, milli>>(
				elapsedTime)
				.count()
		};
		lastFrameTime = frameTime;

		//Note: these are just for monitoring performance.
		frameTimeSeconds = (frameTime / 1000.f);
		framePerSecond = (1.f / frameTimeSeconds);
		window->setTitle(
			"Polygon Survivors |||| FrameTime: " + to_string(frameTime) + " / FPS: " + to_string(framePerSecond) + " / CurrentTimeSlice: " + to_string(currentSlice));
	}
}