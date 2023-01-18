#include "include/WeaponController.h"
using namespace std;
using namespace ComponentSystem;

WeaponController::WeaponController(const WeaponType mType, EntityFactory& mFactory, ComponentSystem::EntityManager& mManager,
	eventpp::EventDispatcher<int, void(int)>& mDispatcher, sf::RenderWindow& mWindow, sf::Vector2f& mPos) :
	Type(mType),
	factory(mFactory),
	manager(mManager),
	gameDispatcher(mDispatcher),
	window(mWindow),
	weaponMountPoint(mPos)
{
	Init();
}

void WeaponController::Init()
{
	stop = false;
	gameDispatcher.appendListener(EventNames::GameStart, [this](int n) {
		UNUSED(n);
		stop = false;
	});
	gameDispatcher.appendListener(EventNames::Win, [this](int n) {
		UNUSED(n);
		stop = true;
	});
	gameDispatcher.appendListener(EventNames::GameOver, [this](int n) {
		UNUSED(n);
		stop = true;
	});
}

void WeaponController::Update(float mFT)
{
	if (FireWaitTimer > 0)
	{
		FireWaitTimer += mFT;
		if (FireWaitTimer < FireInterval)
			return;
	}
	FireWaitTimer = 0;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (!stop)
			Attack();
		FireWaitTimer += mFT;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		FireInterval = baseRate * 0.65f;
	else
		FireInterval = baseRate;
}

void WeaponController::Attack()
{
	switch (Type)
	{
		case WeaponType::Gun:
			GunAttack();
			break;
		case WeaponType::Knife:
			KnifeAttack();
			break;
		case WeaponType::Orbital:
			OrbitalAttack();
			break;
		default:
			break;
	}
}

void WeaponController::GunAttack()
{
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
	sf::Vector2f direction = mousePos - weaponMountPoint;
	float length = sqrt((direction.x * direction.x) + (direction.y * direction.y));
	if (length != 0)
	{
		float normalX = direction.x / length;
		float normalY = direction.y / length;
		sf::Vector2f directionNormalized(normalX, normalY);
		direction = directionNormalized;
	}
	float speedTemp(0.5f);
	factory.CreateProjectile(weaponMountPoint, direction, window, speedTemp, 1);
}

void WeaponController::KnifeAttack()
{
}

void WeaponController::OrbitalAttack()
{
}