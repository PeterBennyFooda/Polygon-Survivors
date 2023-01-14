#include "include/GameClock.h"
using namespace std;

GameClock::GameClock()
{
	Reset();
}

void GameClock::Reset()
{
	if (!font.loadFromFile(fontPath1))
	{
		// error...
	}
	else
	{
		text.setFont(font);
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);

		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left / 2.0f, textRect.top / 2.0f);
		text.setPosition(ScreenWidth / 2.0f - 40.f, ScreenHeight / 20.f);
		text.setStyle(sf::Text::Regular);
		text.setString("00:00");
	}
}

void GameClock::StartTimer(float limit)
{
	if (Stop)
	{
		timeLimit = limit;
		Stop = false;
		Reset();
		clock.restart();
	}
}

void GameClock::RunTimer()
{
	if (CurrentTime <= timeLimit)
		CurrentTime = clock.getElapsedTime().asSeconds();
	else
		Stop = true;
}

void GameClock::DrawText(sf::RenderWindow& window)
{
	string t = "";
	if (CurrentTime < 60)
	{
		if (CurrentTime < 10)
			t = "00:0" + to_string((int)CurrentTime);
		else
			t = "00:" + to_string((int)CurrentTime);
	}
	else
	{
		int min = (int)CurrentTime / 60;
		int sec = (int)CurrentTime - (min * 60);

		if (min < 10 && sec < 10)
			t = "0" + to_string(min) + ":0" + to_string(sec);
		else if (min >= 10 && sec < 10)
			t = to_string(min) + ":0" + to_string(sec);
		else if (min < 10 && sec >= 10)
			t = "0" + to_string(min) + ":" + to_string(sec);
		else
			t = to_string(min) + ":" + to_string(sec);
	}
	text.setString(t);

	if (Stop)
		TimesUp();
	window.draw(text);
}

void GameClock::TimesUp()
{
	text.setCharacterSize(80);
	text.setFillColor(sf::Color::White);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left / 2.0f, textRect.top / 2.0f);
	text.setPosition(ScreenWidth / 3.6f, ScreenHeight / 4.f);
	text.setStyle(sf::Text::Bold);
	text.setString("You Win!");
}