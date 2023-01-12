#pragma once
#include "EntityManager.h"

namespace ComponentSystem
{

struct CCounter : Component
{
	float Counter;
	void Update(float mFT) override
	{
		Counter += mFT;
	}
};

struct CKillCounter : Component
{
	CCounter* Counter;

	~CKillCounter() override
	{
		delete Counter;
	}

	void Init() override
	{
		Counter = &Entity->GetComponent<CCounter>();
	}

	void Update(float mFT) override
	{
		UNUSED(mFT);
		if (Counter->Counter >= 100)
			Entity->Destroy();
	}
};

struct CTransform : Component
{
	sf::Vector2f Position;
	sf::Vector2f Velocity;
	sf::Vector2f Size;
	float Rotation;

	CTransform() :
		Position(0, 0),
		Velocity(0, 0),
		Size(0, 0)
	{}
	CTransform(const sf::Vector2f& position) :
		Position(position)
	{}
};

struct CSprite2D : Component
{
	sf::Texture Texture;
	sf::Sprite Sprite;
	sf::RenderWindow* Target { nullptr };
	CTransform* Transform { nullptr };

	CSprite2D() = default;
	CSprite2D(std::string filePath, sf::RenderWindow* window)
	{
		SetTexture(filePath);
		Target = window;
	}

	~CSprite2D()
	{
		delete Target;
		delete Transform;
	}

	void Init() override
	{
		Transform = &Entity->GetComponent<CTransform>();
		Sprite.setPosition(Transform->Position);
	}

	void Update(float mFT) override
	{
		UNUSED(mFT);
	}

	void Render() override
	{
		Target->draw(Sprite);
	}

	bool SetTexture(std::string filepath)
	{
		if (!Texture.loadFromFile(filepath))
		{
			std::cout << "Error! Player texture not found!" << std::endl;
			return false;
		}
		else
		{
			Sprite.setTexture(Texture);
			Sprite.scale(sf::Vector2f(1.f, 1.f));
			Sprite.setOrigin(sf::Vector2f(Sprite.getTexture()->getSize().x * 0.5f, Sprite.getTexture()->getSize().y * 0.5f));
			return true;
		}
	}
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
