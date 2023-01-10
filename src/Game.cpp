#include "Game.h"

Game::Game()
{
	Init();
	InitPlayer();
}

Game::~Game()
{
	delete this->window;
	delete this->player;
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
	this->player = new Player();
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
}

void Game::Render()
{
	//clear previous frame
	this->window->clear();
	this->player->Render(this->window);

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