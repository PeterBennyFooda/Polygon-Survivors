#include "WeaponController.h"
using namespace std;

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
		Attack();
		FireWaitTimer += mFT;
	}
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
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window));
	sf::Vector2f direction = mousePos - weaponMountPoint;
	float length = sqrt((direction.x * direction.x) + (direction.y * direction.y));
	if (length != 0)
	{
		float normalX = direction.x / length;
		float normalY = direction.y / length;
		sf::Vector2f directionNormalized(normalX, normalY);
		direction = directionNormalized;
	}
	factory.CreateProjectile(weaponMountPoint, direction, window, 0.5f);
}

void WeaponController::KnifeAttack()
{
}

void WeaponController::OrbitalAttack()
{
}