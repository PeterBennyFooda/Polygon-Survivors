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

enum EnemyMoveType
{
	ChasePlayer,
	AvoidPlayer,
	PingPong
};

/*
 * (1) CCounter
 *
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
 * (2) CTransform
 *
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
 * (3) CSprite2D
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
 * (4) CPhysics
 *
 * This Component makes an entity collidable.
 *
 * We can use this compoenet to give an entity very
 * basic collider and moving force based on its velocity.
 */
struct CPhysics : Component
{
private:
	CTransform* transform { nullptr };

public:
	// Use a callback to handle the "out of bounds" event.
	std::function<void(const sf::Vector2f&)> OnOutOfBounds;
	sf::Vector2f HalfSize;
	float BorderWidth, BorderHeight;

	CPhysics(const sf::Vector2f& mHalfSize, const float mBorderX, const float mBorderY) :
		HalfSize(mHalfSize),
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
		return x() - HalfSize.x;
	}
	float Right() const noexcept
	{
		return x() + HalfSize.x;
	}

	//Note: In SFML origin (0,0) is at the top left corner.
	float Top() const noexcept
	{
		return y() - HalfSize.y;
	}
	float Bottom() const noexcept
	{
		return y() + HalfSize.y;
	}
};

/*
 * (5) CPlayerControl
 *
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

public:
	float PlayerSpeed;

	CPlayerControl(const float& mPlayerSpeed) :
		PlayerSpeed(mPlayerSpeed)
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

		//Register out of bound event to prevent player from going out of borders.
		physics->OnOutOfBounds = [this](const sf::Vector2f& mSide) {
			this->OnOutOfBoundsEvent(mSide);
		};
	}

	void Update(float mFT)
	{
		UNUSED(mFT);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			transform->Velocity.x = -PlayerSpeed;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			transform->Velocity.x = PlayerSpeed;
		else
			transform->Velocity.x = 0;

		//Note: In SFML origin (0,0) is at the top left corner.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			transform->Velocity.y = -PlayerSpeed;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			transform->Velocity.y = PlayerSpeed;
		else
			transform->Velocity.y = 0;
	}

	void OnOutOfBoundsEvent(const sf::Vector2f& mSide)
	{
		if (mSide.x != 0.f)
		{
			transform->Velocity.x = 0;
			if (mSide.x == 1)
				transform->Position.x = physics->HalfSize.x;
			else if (mSide.x == -1)
				transform->Position.x = physics->BorderWidth - physics->HalfSize.x;
		}

		if (mSide.y != 0.f)
		{
			transform->Velocity.y = 0;
			if (mSide.y == 1)
				transform->Position.y = physics->HalfSize.y;
			else if (mSide.y == -1)
				transform->Position.y = physics->BorderHeight - physics->HalfSize.y;
		}
	}
};

/*
 * (6) CSimpleEnemyControl
 *
 * This Component is enemy controller.
 *
 * Enemies with this componenet have
 * some simple movesets and AI logic.
 */
struct CSimpleEnemyControl : Component
{
public:
	float EnemySpeed;

private:
	const float avoidRadius { 200.f };

	CPhysics* physics { nullptr };
	CTransform* transform { nullptr };
	sf::Vector2f direction;
	sf::Vector2f& targetPos; //Refernce of the target position so we can keep tracking it.
	EnemyMoveType moveType { EnemyMoveType::ChasePlayer };

public:
	CSimpleEnemyControl(const float mEnemySpeed, sf::Vector2f& mTarget) :
		EnemySpeed(mEnemySpeed),
		targetPos(mTarget)
	{}

	CSimpleEnemyControl(const float& mEnemySpeed, sf::Vector2f& mTarget, EnemyMoveType mMoveType) :
		EnemySpeed(mEnemySpeed),
		targetPos(mTarget),
		moveType(mMoveType)
	{}

	~CSimpleEnemyControl()
	{
		delete physics;
		delete transform;
	}

	void Init() override
	{
		physics = &Entity->GetComponent<CPhysics>();
		transform = &Entity->GetComponent<CTransform>();

		//Since ping pong move has no target we
		//need to set a initial force.
		if (moveType == EnemyMoveType::PingPong)
		{
			transform->Velocity.x = EnemySpeed;
			transform->Velocity.y = EnemySpeed;
		}

		//Register out of bound event to prevent enemy from going out of borders.
		physics->OnOutOfBounds = [this](const sf::Vector2f& mSide) {
			this->OnOutOfBoundsEvent(mSide);
		};
	}

	void Update(float mFT) override
	{
		UNUSED(mFT);
		switch (moveType)
		{
			case EnemyMoveType::ChasePlayer:
				ChasePlayerMove();
				break;
			case EnemyMoveType::AvoidPlayer:
				AvoidPlayerMove();
				break;
			case EnemyMoveType::PingPong:
				PingPongMove();
				break;
			default:
				ChasePlayerMove();
				break;
		}
	}

	void ChasePlayerMove()
	{
		direction = targetPos - transform->Position;
		if (direction.x == 0 && direction.y == 0)
			return;

		//The length of the vector
		float length = sqrt((direction.x * direction.x) + (direction.y * direction.y));
		if (length != 0)
		{
			float normalX = direction.x / length;
			float normalY = direction.y / length;
			sf::Vector2f directionNormalized(normalX, normalY);
			direction = directionNormalized;
		}

		transform->Velocity.x = EnemySpeed * direction.x;
		transform->Velocity.y = EnemySpeed * direction.y;
	}

	void AvoidPlayerMove()
	{
		direction = targetPos - transform->Position;
		if (direction.x == 0 && direction.y == 0)
			return;

		//The length of the vector
		float length = sqrt((direction.x * direction.x) + (direction.y * direction.y));
		float distance = length < 0 ? length * -1 : length;

		if (length != 0)
		{
			float normalX = direction.x / length;
			float normalY = direction.y / length;
			sf::Vector2f directionNormalized(normalX, normalY);
			direction = directionNormalized;
		}

		if (distance <= avoidRadius)
		{
			transform->Velocity.x = EnemySpeed * -direction.x;
			transform->Velocity.y = EnemySpeed * -direction.y;
		}
		PingPongMove();
	}

	void PingPongMove()
	{
		if (physics->Left() <= 0.1f)
			transform->Velocity.x = EnemySpeed;
		else if (physics->Right() >= physics->BorderWidth - 0.1f)
			transform->Velocity.x = -EnemySpeed;

		//Note: In SFML origin (0,0) is at the top left corner.
		if (physics->Top() <= 0.1f)
			transform->Velocity.y = EnemySpeed;
		else if (physics->Bottom() >= physics->BorderHeight - 0.1f)
			transform->Velocity.y = -EnemySpeed;
	}

	void OnOutOfBoundsEvent(const sf::Vector2f& mSide)
	{
		if (moveType == EnemyMoveType::PingPong)
			return;

		if (mSide.x != 0.f)
		{
			transform->Velocity.x = 0;
			if (mSide.x == 1)
				transform->Position.x = physics->HalfSize.x;
			else if (mSide.x == -1)
				transform->Position.x = physics->BorderWidth - physics->HalfSize.x;
		}

		if (mSide.y != 0.f)
		{
			transform->Velocity.y = 0;
			if (mSide.y == 1)
				transform->Position.y = physics->HalfSize.y;
			else if (mSide.y == -1)
				transform->Position.y = physics->BorderHeight - physics->HalfSize.y;
		}
	}
};

}
