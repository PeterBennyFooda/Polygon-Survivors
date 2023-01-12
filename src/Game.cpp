#include "Game.h"
using namespace ComponentSystem;

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
	//Init the window
	this->window = new sf::RenderWindow(sf::VideoMode(screentWidth, screenHeight), "Polygon Survivors", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(144);
	this->window->setVerticalSyncEnabled(false);
	platform.setIcon(this->window->getSystemHandle());
}

void Game::InitPlayer()
{
	auto& entity = manager.AddEntity();
	std::string path = "Resources/Texture/Character/player.png";

	entity.AddComponent<CTransform>(sf::Vector2f(screentWidth / 2, screenHeight / 2));
	auto& playerSprite(entity.AddComponent<CSprite2D>(path, window));
	sf::Vector2f halfSize(playerSprite.Sprite.getOrigin());
	entity.AddComponent<CPhysics>(halfSize, screentWidth, screenHeight);
	entity.AddComponent<CPlayerControl>(0.6f);
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

void Game::Update()
{
	PollingEvent();
	manager.Refresh();
	manager.Update(1.f);
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
		Update();
		Render();
	}
}