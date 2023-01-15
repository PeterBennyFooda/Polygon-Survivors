#pragma once
#include "ComponentSystem/EntityManager.h"
#include "Components.h"
#include "EntityFactory.h"

class WeaponController
{
public:
	WeaponType Type;
	float FireInterval { 0.2f };
	float FireWaitTimer { 0.f };

private:
	EntityFactory& factory;
	ComponentSystem::EntityManager& manager;
	sf::RenderWindow& window;
	sf::Vector2f& weaponMountPoint;

public:
	WeaponController(const WeaponType mType, EntityFactory& mFactory, ComponentSystem::EntityManager& mManager, sf::RenderWindow& mWindow, sf::Vector2f& mPos) :
		Type(mType),
		factory(mFactory),
		manager(mManager),
		window(mWindow),
		weaponMountPoint(mPos) {};

	void Init();
	void Update(float mFT);
	void Attack();

private:
	void GunAttack();
	void KnifeAttack();
	void OrbitalAttack();
};