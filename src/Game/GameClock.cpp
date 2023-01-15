#include "include/GameClock.h"
using namespace std;

GameClock::GameClock()
{
	Reset();
	text.setPosition(ScreenWidth / 2.0f - 150.f, ScreenHeight / 20.f);
	text.setStyle(sf::Text::Bold);
	text.setString("Press Enter to Start\n  Survive for 3 Mins");
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
		text.setString("");
	}

	GlobalDispatcher.appendListener(EventNames::Win, [this]() {
		isWin = true;
	});
	GlobalDispatcher.appendListener(EventNames::GameOver, [this]() {
		isWin = false;
		Stop = true;
	});
}

void GameClock::StartTimer(float limit)
{
	GameStart = true;
	if (Stop && GameStart)
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
	if (Stop && GameStart)
	{
		cout << "STOP" << endl;
		if (isWin)
			DrawWin();
		else
			DrawLose();
	}
	else if (GameStart)
	{
		DrawNormal();
	}
	window.draw(text);
}

void GameClock::DrawNormal()
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
}

void GameClock::DrawLose()
{
	text.setCharacterSize(80);
	text.setFillColor(sf::Color::Red);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left / 2.0f, textRect.top / 2.0f);
	text.setPosition(ScreenWidth / 3.4f, ScreenHeight / 4.f);
	text.setStyle(sf::Text::Bold);
	text.setString("You Lose!");
}

void GameClock::DrawWin()
{
	text.setCharacterSize(80);
	text.setFillColor(sf::Color::White);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left / 2.0f, textRect.top / 2.0f);
	text.setPosition(ScreenWidth / 3.6f, ScreenHeight / 4.f);
	text.setStyle(sf::Text::Bold);
	text.setString("You Win!");
}