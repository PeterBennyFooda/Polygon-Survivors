#include "Game.h"
#include <catch2/catch.hpp>

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
#if defined(_DEBUG)
	std::cout << "Debug Mode Activated!" << std::endl;
#endif

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

void Game::Update()
{
	//Event Polling
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

void Game::Render()
{
	//clear previous frame
	this->window->clear();

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