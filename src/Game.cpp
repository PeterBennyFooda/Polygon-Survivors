#include "Game.h"
#include "Platform/Platform.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <catch2/catch.hpp>

Game::Game()
{
}

Game::~Game()
{
}

void Game::Run()
{
	util::Platform platform;

#if defined(_DEBUG)
	std::cout << "Debug Mode Activated!" << std::endl;
#endif

	sf::RenderWindow window;
	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	const float screentWidth = 800.0f * screenScalingFactor;
	const float screenHeight = 600.0f * screenScalingFactor;
	// Use the screenScalingFactor
	window.create(sf::VideoMode(screentWidth, screenHeight), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	platform.setIcon(window.getSystemHandle());

	sf::Event event;

	//Game Loop
	while (window.isOpen())
	{
		//Event Polling
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				default:
					break;
			}
		}

		//Update

		//Render
		window.clear(); //clear previous frame

		sf::Texture playerTexture;
		sf::Sprite playerSprite;

		if (!playerTexture.loadFromFile("Resources/Texture/Character/player.png"))
		{
			std::cout << "Error! Player texture not found!" << std::endl;
		}
		else
		{
			playerSprite.setTexture(playerTexture);
			playerSprite.scale(sf::Vector2f(0.25f, 0.25f));
			playerSprite.setOrigin(sf::Vector2f(playerSprite.getTexture()->getSize().x * 0.5f, playerSprite.getTexture()->getSize().y * 0.5f));
			playerSprite.setPosition(sf::Vector2f(screentWidth * 0.5f, screenHeight * 0.5f));
		}

		window.draw(playerSprite);

		//Draw the game
		window.display();
	}
}