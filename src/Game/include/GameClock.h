#pragma once
#include "GlobalEventCenter.h"
#include "GlobalGameSettings.h"

class GameClock
{
private:
	sf::Clock clock;
	float timeLimit { 0 };

	sf::Font font;
	sf::Text text;

	bool isWin { false };

	void Reset();
	void DrawNormal();
	void DrawWin();
	void DrawLose();

public:
	float CurrentTime { 0 };
	bool Stop { true };
	bool GameStart { false };

	GameClock();
	void StartTimer(float limit);
	void RunTimer();
	void DrawText(sf::RenderWindow& window);
};
