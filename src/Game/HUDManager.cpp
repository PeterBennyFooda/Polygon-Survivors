#include "include/HUDManager.h"
using namespace std;
using namespace ComponentSystem;

HUDManager::HUDManager(ComponentSystem::EntityManager& mManager, eventpp::EventDispatcher<int, void(int)>& mDispatcher) :
	manager(mManager),
	gameDispatcher(mDispatcher)
{
	Init();
}
void HUDManager::Init()
{
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

	gameDispatcher.appendListener(EventNames::ScoreChange, [this](int score) {
		currentScore += score;
		if (currentScore < 0)
			currentScore = 0;
		UpdateScore();
	});
	gameDispatcher.appendListener(EventNames::PlayerHPChange, [this](int hp) {
		currentHealth += hp;
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
}
