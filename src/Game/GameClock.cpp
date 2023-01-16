#include "include/GameClock.h"
using namespace std;

GameClock::GameClock(eventpp::EventDispatcher<int, void()>& mDispatcher) :
	gameDispatcher(mDispatcher)
{
	Reset();
	text.setPosition(ScreenWidth / 2.0f - 150.f, ScreenHeight / 20.f);
	text.setStyle(sf::Text::Bold);

	int timeLimit = (int)DefaultTimeLimit;
	text.setString("    Press Enter to Start\nSurvive for " + to_string(timeLimit) + " Seconds");

	gameDispatcher.appendListener(EventNames::GameStart, [this]() {
		isWin = false;
		stop = true;
		inGame = true;
	});
	gameDispatcher.appendListener(EventNames::Win, [this]() {
		isWin = true;
		stop = true;
	});
	gameDispatcher.appendListener(EventNames::GameOver, [this]() {
		isWin = false;
		stop = true;
	});
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
}

void GameClock::StartTimer(float limit)
{
	if (inGame)
	{
		stop = false;
		timeLimit = limit;
		Reset();
		clock.restart();
	}
}

void GameClock::RunTimer()
{
	if (CurrentTime <= timeLimit)
		CurrentTime = clock.getElapsedTime().asSeconds();
	else
		stop = true;
}

void GameClock::DrawText(sf::RenderWindow& window)
{
	if (inGame)
	{
		if (stop)
		{
			if (isWin)
				DrawWin();
			else
				DrawLose();
			inGame = false;
		}
		else
		{
			DrawNormal();
		}
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
	text.setPosition(ScreenWidth / 4.f, ScreenHeight / 4.f);
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