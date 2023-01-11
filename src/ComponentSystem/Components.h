#pragma once
#include "EntityManager.h"

namespace ComponentSystem
{

struct CTransform : Component
{
	sf::Vector2f Position {};
	sf::Vector2f Velocity {};
	sf::Vector2f Size {};
	float Rotation {};

	CTransform() = default;
	CTransform(const sf::Vector2f& position) :
		Position(position)
	{}
};

struct CPhysics : Component
{
	CTransform* cTransform { nullptr };
	sf::Vector2f velocity, halfSize;
	float windowWidth, windowHeight;

	// Use a callback to handle the "out of bounds" event.
	std::function<void(const sf::Vector2f&)> OnOutOfBounds;

	CPhysics(const sf::Vector2f& mHalfSize) :
		halfSize { mHalfSize }
	{}

	void Init() override
	{
		// `CPhysics` obviously requires `CTransform`.
		cTransform = &Entity->GetComponent<CTransform>();
	}

	void Update(float mFT) override
	{
		cTransform->Position += velocity * mFT;

		if (OnOutOfBounds == nullptr)
			return;

		if (left() < 0)
			OnOutOfBounds(sf::Vector2f { 1.f, 0.f });
		else if (right() > windowWidth)
			OnOutOfBounds(sf::Vector2f { -1.f, 0.f });

		if (top() < 0)
			OnOutOfBounds(sf::Vector2f { 0.f, 1.f });
		else if (bottom() > windowHeight)
			OnOutOfBounds(sf::Vector2f { 0.f, -1.f });
	}

	float x() const noexcept
	{
		return cTransform->Position.x;
	}
	float y() const noexcept
	{
		return cTransform->Position.y;
	}
	float left() const noexcept
	{
		return x() - halfSize.x;
	}
	float right() const noexcept
	{
		return x() + halfSize.x;
	}
	float top() const noexcept
	{
		return y() - halfSize.y;
	}
	float bottom() const noexcept
	{
		return y() + halfSize.y;
	}
};

}
