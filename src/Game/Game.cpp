#include "Game.h"
using namespace ComponentSystem;
using namespace std;

Game::Game()
{
	Init();
	InitPlayer();
}

Game::~Game()
{
	delete this->window;
}

void Game ::Init()
{
	//Init the window.
	this->window = new sf::RenderWindow(sf::VideoMode(screentWidth, screenHeight), "Polygon Survivors", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(144);
	this->window->setVerticalSyncEnabled(false);
	platform.setIcon(this->window->getSystemHandle());

	//For calculating delta time.
	timePoint1 = std::chrono::steady_clock::now();
	timePoint2 = std::chrono::steady_clock::now();
}

void Game::InitPlayer()
{
	auto& entity = manager.AddEntity();
	string path = "Resources/Texture/Character/player.png";

	entity.AddComponent<CTransform>(sf::Vector2f(screentWidth / 2, screenHeight / 2));
	auto& playerSprite(entity.AddComponent<CSprite2D>(path, window));
	sf::Vector2f halfSize(playerSprite.Sprite.getOrigin());
	entity.AddComponent<CPhysics>(halfSize, screentWidth, screenHeight);
	entity.AddComponent<CPlayerControl>(1.f);
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
	for (; currentSlice >= ftSlice; currentSlice -= ftSlice)
	{
		//Passing 'ftStep' instead of 'lastFrameTime' because
		//we want to ensure the ideal 'frame time' is constant.
	}
}

void Game::Update()
{
	manager.Refresh();
	manager.Update(lastFrameTime);
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