#pragma once
#include "ComponentSystem/EntityManager.h"
#include "GlobalGameSettings.h"

/////////////////////////////////////////////////
///
///This file defines all the components.
///Users can extend this list by adding custom components.
///
/////////////////////////////////////////////////
namespace ComponentSystem
{
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
	sf::Vector2f Size;
	float Rotation;

	CTransform() :
		Position(0, 0),
		Size(1.f, 1.f),
		Rotation(0.f)
	{}
	CTransform(const sf::Vector2f& position) :
		Position(position),
		Size(1.f, 1.f),
		Rotation(0.f)
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
	sf::RenderWindow& target;
	CTransform* transform { nullptr };
	sf::Texture texture;
	sf::Sprite sprite;

public:
	bool Visable { true };
	sf::Vector2f Origin;

	CSprite2D() = default;
	CSprite2D(std::string filePath, sf::RenderWindow& window) :
		target(window)
	{
		SetTexture(filePath);
	}

	void Init() override
	{
		transform = &Entity->GetComponent<CTransform>();
		sprite.setPosition(transform->Position);
		sprite.setRotation(transform->Rotation);
		sprite.setScale(transform->Size);
		Origin = sprite.getOrigin();
	}

	void Update(float mFT) override
	{
		UNUSED(mFT);
		sprite.setPosition(transform->Position);
		sprite.setRotation(transform->Rotation);
	}

	void Render() override
	{
		if (Visable)
			target.draw(sprite);
	}

	void ChangeColor(sf::Color color)
	{
		sprite.setColor(color);
	}

	bool SetTexture(std::string filepath)
	{
		if (!texture.loadFromFile(filepath))
		{
			std::cout << "Error! Player texture not found!" << std::endl;
			return false;
		}
		else
		{
			sprite.setTexture(texture);
			sprite.scale(sf::Vector2f(1.f, 1.f));
			sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5f, sprite.getTexture()->getSize().y * 0.5f));
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
	sf::Vector2f Velocity;
	float BorderWidth, BorderHeight;

	CPhysics(const sf::Vector2f& mHalfSize, const float mBorderX, const float mBorderY) :
		HalfSize(mHalfSize),
		Velocity(0, 0),
		BorderWidth(mBorderX),
		BorderHeight(mBorderY)
	{}

	void Init() override
	{
		//'CPhysics' obviously requires 'CTransform'.
		transform = &Entity->GetComponent<CTransform>();
	}

	void Update(float mFT) override
	{
		transform->Position += Velocity * mFT;

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
	bool Stop { false };

	CPlayerControl(const float& mPlayerSpeed) :
		PlayerSpeed(mPlayerSpeed)
	{}

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
		//std::cout << Stop << std::endl;
		if (Stop)
		{
			physics->Velocity.x = 0;
			physics->Velocity.y = 0;
			return;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			physics->Velocity.x = -PlayerSpeed;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			physics->Velocity.x = PlayerSpeed;
		else
			physics->Velocity.x = 0;

		//Note: In SFML origin (0,0) is at the top left corner.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			physics->Velocity.y = -PlayerSpeed;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			physics->Velocity.y = PlayerSpeed;
		else
			physics->Velocity.y = 0;
	}

private:
	void OnOutOfBoundsEvent(const sf::Vector2f& mSide)
	{
		if (mSide.x != 0.f)
		{
			physics->Velocity.x = 0;
			if (mSide.x == 1)
				transform->Position.x = physics->HalfSize.x;
			else if (mSide.x == -1)
				transform->Position.x = physics->BorderWidth - physics->HalfSize.x;
		}

		if (mSide.y != 0.f)
		{
			physics->Velocity.y = 0;
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
	bool Stop { false };

private:
	const float avoidRadius { 150.f };

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

	void Init() override
	{
		physics = &Entity->GetComponent<CPhysics>();
		transform = &Entity->GetComponent<CTransform>();

		//Since ping pong move has no target we
		//need to create a initial force.
		if (moveType == EnemyMoveType::PingPong)
		{
			physics->Velocity.x = EnemySpeed;
			physics->Velocity.y = EnemySpeed;
		}

		//Register out of bound event to prevent enemy from going out of borders.
		physics->OnOutOfBounds = [this](const sf::Vector2f& mSide) {
			this->OnOutOfBoundsEvent(mSide);
		};
	}

	void Update(float mFT) override
	{
		if (Stop && moveType != EnemyMoveType::PingPong)
		{
			physics->Velocity.x = 0;
			physics->Velocity.y = 0;
			return;
		}

		switch (moveType)
		{
			case EnemyMoveType::ChasePlayer:
				ChasePlayerMove(mFT);
				break;
			case EnemyMoveType::AvoidPlayer:
				AvoidPlayerMove(mFT);
				break;
			case EnemyMoveType::PingPong:
				PingPongMove(mFT);
				break;
			default:
				ChasePlayerMove(mFT);
				break;
		}
	}

private:
	void ChasePlayerMove(float mFT)
	{
		direction = targetPos - transform->Position;
		float angle = std::atan2(direction.y, direction.x);
		angle = angle * (180 / std::acos(-1)); //PI: acos(-1)
		if (angle < 0)
		{
			angle = 360 - (-angle);
		}
		angle += 90;
		//The 0 degree of atan2 is pointed the right
		//but enemy head is pointed up so we adjust it.
		transform->Rotation = Lerp(transform->Rotation, angle, EnemySpeed * mFT);
		if (std::abs(angle - transform->Rotation) <= 1.f)
			transform->Rotation = angle;

		//The length of the vector.
		float length = std::sqrt((direction.x * direction.x) + (direction.y * direction.y));

		if (length != 0)
		{
			float normalX = direction.x / length;
			float normalY = direction.y / length;
			sf::Vector2f directionNormalized(normalX, normalY);
			direction = directionNormalized;
		}

		physics->Velocity.x = EnemySpeed * direction.x;
		physics->Velocity.y = EnemySpeed * direction.y;
	}

	void AvoidPlayerMove(float mFT)
	{
		direction = targetPos - transform->Position;
		float length = sqrt((direction.x * direction.x) + (direction.y * direction.y));
		float distance = length < 0 ? length * -1 : length;

		if (length != 0)
		{
			float normalX = direction.x / length;
			float normalY = direction.y / length;
			sf::Vector2f directionNormalized(normalX, normalY);
			direction = directionNormalized;
		}
		float angle = std::atan2(direction.y, direction.x);
		angle = angle * (180 / std::acos(-1)); //PI: acos(-1)
		if (angle < 0)
		{
			angle = 360 - (-angle);
		}
		angle += 90;
		transform->Rotation = Lerp(transform->Rotation, angle, EnemySpeed * mFT);
		if (std::abs(angle - transform->Rotation) <= 1.f)
			transform->Rotation = angle;

		float delta = std::abs(avoidRadius - distance);
		if (delta < 0.1f)
		{
			physics->Velocity.x = 0;
			physics->Velocity.y = 0;
			return;
		}

		if (distance < avoidRadius)
		{
			physics->Velocity.x = EnemySpeed * -direction.x;
			physics->Velocity.y = EnemySpeed * -direction.y;
		}
		else if (distance > avoidRadius)
		{
			physics->Velocity.x = EnemySpeed * direction.x * 0.5f;
			physics->Velocity.y = EnemySpeed * direction.y * 0.5f;
		}
	}

	void PingPongMove(float mFT)
	{
		direction = targetPos - transform->Position;
		float angle = std::atan2(direction.y, direction.x);
		angle = angle * (180 / std::acos(-1)); //PI: acos(-1)
		if (angle < 0)
		{
			angle = 360 - (-angle);
		}
		angle += 90;
		transform->Rotation = Lerp(transform->Rotation, angle, EnemySpeed * mFT);
		if (std::abs(angle - transform->Rotation) <= 1.f)
			transform->Rotation = angle;

		if (physics->Left() <= 0.1f)
			physics->Velocity.x = EnemySpeed;
		else if (physics->Right() >= physics->BorderWidth - 0.1f)
			physics->Velocity.x = -EnemySpeed;

		//Note: In SFML origin (0,0) is at the top left corner.
		if (physics->Top() <= 0.1f)
			physics->Velocity.y = EnemySpeed;
		else if (physics->Bottom() >= physics->BorderHeight - 0.1f)
			physics->Velocity.y = -EnemySpeed;
	}

	void OnOutOfBoundsEvent(const sf::Vector2f& mSide)
	{
		if (moveType == EnemyMoveType::PingPong)
			return;

		if (mSide.x != 0.f)
		{
			physics->Velocity.x = 0;
			if (mSide.x == 1)
				transform->Position.x = physics->HalfSize.x;
			else if (mSide.x == -1)
				transform->Position.x = physics->BorderWidth - physics->HalfSize.x;
		}

		if (mSide.y != 0.f)
		{
			physics->Velocity.y = 0;
			if (mSide.y == 1)
				transform->Position.y = physics->HalfSize.y;
			else if (mSide.y == -1)
				transform->Position.y = physics->BorderHeight - physics->HalfSize.y;
		}
	}

private:
	float Lerp(float a, float b, float f)
	{
		return a * (1.0 - f) + (b * f);
	}
};

/*
 * (7) CStat
 *
 * This Component provides character stats.
 *
 * We can extend CStat to give character a
 * variety of modifications such as Health,
 * SpeedMod, Armor, etc.
 */
struct CStat : Component
{
private:
	float hitCoolDown = { 0.5f };
	float hitTimer = { 0.f };
	CSprite2D* sprite { nullptr };

public:
	int Health { 3 };
	float SpeedMod { 1 };
	bool IsDead { false };
	bool IsInvincible { false };
	bool CanBeProtect { true };

	CStat() = default;
	CStat(const int& mHP, const float& mSpeedMod) :
		Health(mHP),
		SpeedMod(mSpeedMod)
	{}

	void Init() override
	{
		sprite = &Entity->GetComponent<CSprite2D>();
		hitCoolDown = HitCoolDown;
	}

	void Update(float mFT) override
	{
		UNUSED(mFT);
		CheckDeath();
		HitProtectionTimer(mFT);
	}

	void Hit(int damage)
	{
		if (!IsDead && !IsInvincible)
		{
			HitProtection();
			Health -= damage;
		}
		CheckDeath();
	}

private:
	void CheckDeath()
	{
		if (Health <= 0)
		{
			Health = 0;
			IsDead = true;

			if (CanBeProtect)
				sprite->ChangeColor(sf::Color::Red);
		}
	}

	void HitProtection()
	{
		if (!IsInvincible && CanBeProtect)
			IsInvincible = true;
	}

	void HitProtectionTimer(float mFT)
	{
		if (!IsInvincible || IsDead || !CanBeProtect)
			return;

		hitTimer += mFT / 1000;
		if (hitTimer < hitCoolDown)
		{
			sprite->ChangeColor(sf::Color::Green);
			IsInvincible = true;
		}
		else
		{
			hitTimer = 0;
			sprite->ChangeColor(sf::Color::White);
			IsInvincible = false;
		}
	}
};

/*
 * (8) CProjectile
 *
 * This Component is projectile controller.
 *
 * Entity with this component is considered
 * a projectile like bullet.
 */
struct CProjectile : Component
{
private:
	CPhysics* physics { nullptr };
	CTransform* transform { nullptr };
	bool isDead { false };

public:
	float Speed;
	sf::Vector2f Direction;
	bool Stop { false };
	int Damage { 1 };

	CProjectile(const float& mSpeed, const sf::Vector2f mDirection) :
		Speed(mSpeed),
		Direction(mDirection)
	{}
	CProjectile(const float& mSpeed, const sf::Vector2f mDirection, const int& mDamage) :
		Speed(mSpeed),
		Direction(mDirection),
		Damage(mDamage)
	{}

	void Init() override
	{
		physics = &Entity->GetComponent<CPhysics>();
		transform = &Entity->GetComponent<CTransform>();
		Damage = 1;

		//Register out of bound event to prevent player from going out of borders.
		physics->OnOutOfBounds = [this](const sf::Vector2f& mSide) {
			this->OnOutOfBoundsEvent(mSide);
		};
	}

	void Update(float mFT)
	{
		UNUSED(mFT);
		if (Stop || isDead)
		{
			physics->Velocity.x = 0;
			physics->Velocity.y = 0;
			return;
		}
		Fly();
	}

	void Fly()
	{
		physics->Velocity.x = Speed * Direction.x;
		physics->Velocity.y = Speed * Direction.y;
	}

	void Die()
	{
		Stop = true;
		isDead = true;
		Entity->Destroy();
	}

	void OnOutOfBoundsEvent(const sf::Vector2f& mSide)
	{
		UNUSED(mSide);
		if (!isDead)
			Die();
	}
};
}
