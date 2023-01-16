#pragma once
#include "ComponentSystem/EntityManager.h"
#include "Components.h"
#include "GlobalGameSettings.h"
#include "eventpp/eventdispatcher.h"

class HUDManager
{
private:
	ComponentSystem::EntityManager& manager;
	eventpp::EventDispatcher<int, void(int)>& gameDispatcher;
	int currentScore { 0 };
	int currentHealth { 0 };
	sf::Text currentScoreText;
	sf::Font currentScoreFont;
	sf::Text currentHealthText;
	sf::Font currentHealthFont;

	sf::Font hintFont;
	sf::Text hintText;

	void Init();
	void UpdateScore();
	void UpdateHealth();

public:
	HUDManager(ComponentSystem::EntityManager& manager, eventpp::EventDispatcher<int, void(int)>& dispatcher);

	void Reset();
	void DrawHUD(sf::RenderWindow& window);
};