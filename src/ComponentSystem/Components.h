#pragma once
#include "EntityManager.h"

/////////////////////////////////////////////////
///
///This file defines all the components.
///Users can extend this list by adding custom components.
///
/////////////////////////////////////////////////
namespace ComponentSystem
{

/*
 * This Component is a simple counter based
 * on frame time.
 *
 * We can use this counter to test if the
 * component system is working.
 */
struct CCounter : Component
{
	float Counter;
	void Update(float mFT) override
	{
		Counter += mFT;
	}
};

/*
 * This Component records position, velocity
 * and size of an entity.
 *
 * We can use this compoenet to grant an entity
 * the basic information to interact with the world
 * and other entities.
 */
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

/*
 * This Component makes an entity visble in the world.
 *
 * We can use this compoenet to give an entity fancy
 * texture which is way better than plain colors.
 *
 * Also, SFML sprite is not just sprite. We can scale
 * it and change its position so it's actually a game
 * object like rectangle and circle.
 */
struct CSprite2D : Component
{
private:
	sf::RenderWindow* target { nullptr };
	CTransform* transform { nullptr };

public:
	sf::Texture Texture;
	sf::Sprite Sprite;

	CSprite2D() = default;
	CSprite2D(std::string filePath, sf::RenderWindow* window)
	{
		SetTexture(filePath);
		target = window;
	}

	~CSprite2D()
	{
		delete target;
		delete transform;
	}

	void Init() override
	{
		transform = &Entity->GetComponent<CTransform>();
		Sprite.setPosition(transform->Position);
	}

	void Update(float mFT) override
	{
		UNUSED(mFT);
		Sprite.setPosition(transform->Position);
	}

	void Render() override
	{
		target->draw(Sprite);
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

/*
 * This Component makes an entity collidable.
 *
 * We can use this compoenet to give an entity very
 * basic collider and moving force based on its velocity.
 */
struct CPhysics : Component
{
private:
	CTransform* transform { nullptr };
	sf::Vector2f halfSize;

public:
	// Use a callback to handle the "out of bounds" event.
	std::function<void(const sf::Vector2f&)> OnOutOfBounds;
	float BorderWidth, BorderHeight;

	CPhysics(const sf::Vector2f& mHalfSize, const float mBorderX, const float mBorderY) :
		halfSize(mHalfSize),
		BorderWidth(mBorderX),
		BorderHeight(mBorderY)
	{}

	~CPhysics()
	{
		delete transform;
	}

	void Init() override
	{
		//'CPhysics' obviously requires 'CTransform'.
		transform = &Entity->GetComponent<CTransform>();
	}

	void Update(float mFT) override
	{
		transform->Position += transform->Velocity * mFT;

		if (OnOutOfBounds == nullptr)
			return;

		if (Left() < 0)
			OnOutOfBounds(sf::Vector2f { 1.f, 0.f });
		else if (Right() > BorderWidth)
			OnOutOfBounds(sf::Vector2f { -1.f, 0.f });

		if (Top() < 0)
			OnOutOfBounds(sf::Vector2f { 0.f, 1.f });
		else if (Bottom() > BorderHeight)
			OnOutOfBounds(sf::Vector2f { 0.f, -1.f });
	}

	float x() const noexcept
	{
		return transform->Position.x;
	}
	float y() const noexcept
	{
		return transform->Position.y;
	}

	float Left() const noexcept
	{
		return x() - halfSize.x;
	}
	float Right() const noexcept
	{
		return x() + halfSize.x;
	}

	//Note: In SFML origin (0,0) is at the top left corner.
	float Top() const noexcept
	{
		return y() - halfSize.y;
	}
	float Bottom() const noexcept
	{
		return y() + halfSize.y;
	}
};

/*
 * This Component is player controller.
 *
 * We can use this compoenet to make an entity
 * controlled by the player.
 */
struct CPlayerControl : Component
{
private:
	CPhysics* physics { nullptr };
	CTransform* transform { nullptr };
	float playerSpeed;

public:
	CPlayerControl(const float& mPlayerSpeed) :
		playerSpeed(mPlayerSpeed)
	{}

	~CPlayerControl()
	{
		delete physics;
		delete transform;
	}

	void Init() override
	{
		physics = &Entity->GetComponent<CPhysics>();
		transform = &Entity->GetComponent<CTransform>();
	}

	void Update(float mFT)
	{
		UNUSED(mFT);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && physics->Left() > 0)
			transform->Velocity.x = -playerSpeed;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && physics->Right() < physics->BorderWidth)
			transform->Velocity.x = playerSpeed;
		else
			transform->Velocity.x = 0;

		//Note: In SFML origin (0,0) is at the top left corner.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && physics->Top() > 0)
			transform->Velocity.y = -playerSpeed;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && physics->Bottom() < physics->BorderHeight)
			transform->Velocity.y = playerSpeed;
		else
			transform->Velocity.y = 0;
	}
};

}
