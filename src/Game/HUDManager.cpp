#include "include/HUDManager.h"
using namespace std;
using namespace ComponentSystem;

HUDManager::HUDManager(ComponentSystem::EntityManager& mManager, eventpp::EventDispatcher<int, void(const MyEvent&), MyEventPolicies>& mDispatcher) :
	manager(mManager),
	gameDispatcher(mDispatcher)
{
	Init();
}
void HUDManager::Init()
{
	//Hint
	if (!hintFont.loadFromFile(fontPath2))
	{
		// error...
	}
	else
	{
		hintText.setFont(hintFont);
		hintText.setCharacterSize(22);
		hintText.setFillColor(sf::Color::White);

		sf::FloatRect textRect = hintText.getLocalBounds();
		hintText.setOrigin(textRect.left / 2.0f, textRect.top / 2.0f);
		hintText.setPosition(ScreenWidth / 2.0f - 90.f, ScreenHeight / 3.0f);
		hintText.setStyle(sf::Text::Regular);
		hintText.setString("[WASD] Move\n[LMB] Shoot\n[LSHIFT] Slow\n[LCTRL] Fast\n[ENTER] Continue");
	}

	//Score
	if (!currentScoreFont.loadFromFile(fontPath2))
	{
		// error...
	}
	else
	{
		currentScoreText.setFont(currentScoreFont);
		currentScoreText.setCharacterSize(24);
		currentScoreText.setFillColor(sf::Color::White);

		sf::FloatRect textRect = currentScoreText.getLocalBounds();
		currentScoreText.setOrigin(textRect.left / 2.0f, textRect.top / 2.0f);
		currentScoreText.setPosition(15.f, ScreenHeight / 20.f);
		currentScoreText.setStyle(sf::Text::Regular);
		currentScoreText.setString("");
	}

	//HP
	if (!currentHealthFont.loadFromFile(fontPath2))
	{
		// error...
	}
	else
	{
		currentHealthText.setFont(currentHealthFont);
		currentHealthText.setCharacterSize(24);
		currentHealthText.setFillColor(sf::Color::White);

		sf::FloatRect textRect = currentHealthText.getLocalBounds();
		currentHealthText.setOrigin(textRect.left / 2.0f, textRect.top / 2.0f);
		currentHealthText.setPosition(15.f, ScreenHeight / 10.f);
		currentHealthText.setStyle(sf::Text::Regular);
		currentHealthText.setString("");
	}

	gameDispatcher.appendListener(EventNames::ScoreChange, [this](const MyEvent& e) {
		currentScore += e.param;
		if (currentScore < 0)
			currentScore = 0;
		UpdateScore();
	});
	gameDispatcher.appendListener(EventNames::PlayerHPChange, [this](const MyEvent& e) {
		currentHealth += e.param;
		if (currentHealth < 0)
			currentHealth = 0;
		UpdateHealth();
	});
}

void HUDManager::Reset()
{
	currentScore = 0;

	auto& players(manager.GetEntitiesByGroup(EntityGroup::Player));
	auto& p(players[0]);
	auto& pStat(p->GetComponent<CStat>());

	currentHealth = pStat.Health;
	UpdateScore();
	UpdateHealth();

	hintText.setCharacterSize(18);
	hintText.setPosition(ScreenWidth / 2.0f + 225.f, ScreenHeight / 20.f);
}

void HUDManager::UpdateScore()
{
	currentScoreText.setString("Score: " + to_string(currentScore));
}

void HUDManager::UpdateHealth()
{
	currentHealthText.setString("Health: " + to_string(currentHealth));
}

void HUDManager::DrawHUD(sf::RenderWindow& window)
{
	window.draw(currentScoreText);
	window.draw(currentHealthText);
	window.draw(hintText);
}
