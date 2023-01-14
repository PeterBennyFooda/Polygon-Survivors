#pragma once
#include "GlobalGameSettings.h"

class GameClock
{
private:
	sf::Clock clock;
	float timeLimit { 0 };

	sf::Font font;
	sf::Text text;

    void Reset();
    void TimesUp();

public:
	float CurrentTime { 0 };
	bool Stop { true };

	GameClock();

	void StartTimer(float limit);
	void RunTimer();
	void DrawText(sf::RenderWindow& window);
};
