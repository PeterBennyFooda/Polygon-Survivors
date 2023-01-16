#pragma once
#include "ComponentSystem/EntityManager.h"
#include "Components.h"
#include "EntityFactory.h"
#include "eventpp/eventdispatcher.h"

class WeaponController
{
public:
	WeaponType Type;
	float FireInterval { 0.2f };
	float FireWaitTimer { 0.f };

private:
	EntityFactory& factory;

	ComponentSystem::EntityManager& manager;
	eventpp::EventDispatcher<int, void()>& gameDispatcher;
	sf::RenderWindow& window;
	sf::Vector2f& weaponMountPoint;
	bool stop { true };

public:
	WeaponController(const WeaponType mType, EntityFactory& mFactory, ComponentSystem::EntityManager& mManager,
		eventpp::EventDispatcher<int, void()>& mDispatcher, sf::RenderWindow& mWindow, sf::Vector2f& mPos);

	void Init();
	void Update(float mFT);
	void Attack();

private:
	void GunAttack();
	void KnifeAttack();
	void OrbitalAttack();
};