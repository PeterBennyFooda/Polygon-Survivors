#include "include/GameClock.h"
using namespace std;

GameClock::GameClock(eventpp::EventDispatcher<int, void(const MyEvent&), MyEventPolicies>& mDispatcher) :
	gameDispatcher(mDispatcher)
{
	Reset();
	text.setPosition(ScreenWidth / 2.0f - 155.f, ScreenHeight / 20.f);
	text.setStyle(sf::Text::Bold);

	int timeLimit = (int)DefaultTimeLimit;
	text.setString("    Press Enter to Start\nSurvive for " + to_string(timeLimit) + " Seconds");

	gameDispatcher.appendListener(EventNames::Win, [this](const MyEvent&) {
		DrawWin();
		isWin = true;
		stop = true;
		inGame = false;
	});
	gameDispatcher.appendListener(EventNames::GameOver, [this](const MyEvent&) {
		DrawLose();
		isWin = false;
		stop = true;
		inGame = false;
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
	inGame = true;
	stop = false;
	timeLimit = limit;
	Reset();
	clock.restart();
}

void GameClock::RunTimer()
{
	if (!stop && inGame)
	{
		if (CurrentTime <= timeLimit)
			CurrentTime = clock.getElapsedTime().asSeconds();
		else
		{
			stop = true;
			CurrentTime = 0;
			gameDispatcher.dispatch(MyEvent { EventNames::Win, "Win", 0 });
		}
	}
}

void GameClock::DrawText(sf::RenderWindow& window)
{
	if (inGame && !stop)
		DrawNormal();

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
	text.setFillColor(sf::Color::Yellow);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left / 2.0f, textRect.top / 2.0f);
	text.setPosition(ScreenWidth / 3.6f, ScreenHeight / 4.f);
	text.setStyle(sf::Text::Bold);
	text.setString("You Win!");
}