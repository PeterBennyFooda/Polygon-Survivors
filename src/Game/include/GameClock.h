#pragma once
#include "GlobalGameSettings.h"
#include "eventpp/eventdispatcher.h"

class GameClock
{
private:
	eventpp::EventDispatcher<int, void(int)>& gameDispatcher;
	sf::Clock clock;
	float timeLimit { 0 };

	sf::Font font;
	sf::Text text;

	bool stop { true };
	bool inGame { false };
	bool isWin { false };

	void Reset();
	void DrawNormal();
	void DrawWin();
	void DrawLose();

public:
	float CurrentTime { 0 };

	GameClock(eventpp::EventDispatcher<int, void(int)>& dispatcher);
	void StartTimer(float limit);
	void RunTimer();
	void DrawText(sf::RenderWindow& window);
};
