#pragma once
#include "ComponentSystem/EntityManager.h"
#include "GlobalGameSettings.h"
#include "eventpp/eventdispatcher.h"

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
 * (4) CParticle
 *
 * This Component is a particle emitter.
 *
 * Entity with this component can emit particles.
 */
struct Particle
{
	sf::Vector2f pos; // Position
	sf::Vector2f vel; // Velocity
	sf::Color color;  // RGBA
};
enum Shape
{
	CIRCLE,
	SQUARE
};
struct CParticle : Component
{
private:
	sf::Vector2f position; // Particle origin (pixel co-ordinates)
	sf::Vector2f gravity;  // Affects particle velocities
	sf::Color transparent; // sf::Color( 0, 0, 0, 0 )
	sf::Image* image;
	sf::Texture texture;
	sf::Sprite sprite; // Connected to Texture
	sf::Color color;
	float particleSpeed { 1.f };
	bool dissolve { false }; // Dissolution enabled?
	unsigned char dissolutionRate { 4 };
	unsigned char shape { Shape::SQUARE };

	sf::RenderWindow& window;

	std::list<Particle*> particles;
	std::random_device randDevice {};
	std::default_random_engine randGenerator { randDevice() };
	CTransform* transform { nullptr };

	bool emitting { false };

public:
	CParticle(sf::RenderWindow& target) :
		window(target)
	{
		image = NULL;
	}

	~CParticle()
	{
		for (std::list<Particle*>::iterator it = particles.begin(); it != particles.end(); it++)
		{
			delete *it;
		}
		delete image;
	};

	void Init() override
	{
		//If the entity has transform, set the defulat position to where it is.
		if (Entity->HasComponent<CTransform>())
		{
			transform = &Entity->GetComponent<CTransform>();
			position = transform->Position;
		}
		else
		{
			position.x = ScreenWidth / 2;
			position.y = ScreenHeight / 2;
		}
		transparent = sf::Color(0, 0, 0, 0);
	}

	// Updates position, velocity and opacity of all particles.
	void Update(float mFT) override
	{
		if (!emitting || image == NULL)
			return;

		Remove();
		for (std::list<Particle*>::iterator it = particles.begin(); it != particles.end(); it++)
		{
			(*it)->vel.x += gravity.x * mFT;
			(*it)->vel.y += gravity.y * mFT;

			(*it)->pos.x += (*it)->vel.x * mFT * particleSpeed;
			(*it)->pos.y += (*it)->vel.y * mFT * particleSpeed;

			if (dissolve)
				(*it)->color.a -= dissolutionRate;

			if ((*it)->pos.x > image->getSize().x || (*it)->pos.x < 0
				|| (*it)->pos.y > image->getSize().y || (*it)->pos.y < 0 || (*it)->color.a < 10)
			{
				delete (*it);
				it = particles.erase(it);
				if (it == particles.end())
					return;
			}
		}
	};

	// Renders all particles onto image.
	void Render() override
	{
		if (!emitting || image == NULL)
			return;

		for (std::list<Particle*>::iterator it = particles.begin(); it != particles.end(); it++)
		{
			if (image == NULL)
				return;
			image->setPixel((int)(*it)->pos.x, (int)(*it)->pos.y, (*it)->color);
		}
		texture.loadFromImage(*image);
		sprite.setTexture(texture, true);
		window.draw(sprite);
	};

private:
	// Removes all particles from image.
	void Remove()
	{
		if (!emitting || image == NULL)
			return;

		for (std::list<Particle*>::iterator it = particles.begin(); it != particles.end(); it++)
		{
			if (image == NULL)
				return;
			image->setPixel((int)(*it)->pos.x, (int)(*it)->pos.y, transparent);
		}

		if (particles.size() <= 0)
			emitting = false;
	};

	float Randomizer(float min, float max)
	{
		std::uniform_real_distribution<float> unif(min, max);
		float result = unif(randGenerator);

		return result;
	}

public:
	// Adds new particles to particles.
	void Fuel(int count)
	{
		float angle;
		Particle* particle;

		if (image == NULL)
		{
			image = new sf::Image();
			image->create(ScreenWidth, ScreenHeight, transparent);
		}

		if (count > 0)
			emitting = true;

		for (int i = 0; i < count; ++i)
		{
			particle = new Particle();
			particle->pos.x = position.x;
			particle->pos.y = position.y;

			switch (shape)
			{
				case Shape::CIRCLE:
					dissolve = true;
					angle = Randomizer(0.0f, 6.2832f);
					particle->vel.x = Randomizer(0.0f, cos(angle));
					particle->vel.y = Randomizer(0.0f, sin(angle));
					break;
				case Shape::SQUARE:
					dissolve = false;
					particle->vel.x = Randomizer(-1.0f, 1.0f);
					particle->vel.y = Randomizer(-1.0f, 1.0f);
					break;
				default:
					particle->vel.x = 0.5f; // Easily detected
					particle->vel.y = 0.5f; // Easily detected
			}

			if (particle->vel.x == 0.0f && particle->vel.y == 0.0f)
			{
				delete particle;
				continue;
			}

			particle->color.r = color.r;
			particle->color.g = color.g;
			particle->color.b = color.b;
			particle->color.a = 255;
			particles.push_back(particle);
		}
	};

	void SetPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	void SetColor(sf::Color mColor)
	{
		color = mColor;
	}

	void SetGravity(float x, float y)
	{
		gravity.x = x;
		gravity.y = y;
	}

	void SetParticleSpeed(float speed)
	{
		particleSpeed = speed;
	}

	void SetDissolve(bool enable)
	{
		dissolve = enable;
	}

	void SetDissolutionRate(unsigned char rate)
	{
		dissolutionRate = rate;
	}

	void SetShape(unsigned char shape)
	{
		shape = shape;
	}

	int GetNumberOfParticles()
	{
		return particles.size();
	}

	std::string GetNumberOfParticlesString()
	{
		std::ostringstream oss;
		oss << particles.size();
		return oss.str();
	};

	sf::Sprite& GetSprite()
	{
		return sprite;
	}
};

/*
 * (5) CStat
 *
 * This Component provides character stats.
 *
 * We can extend CStat to give character a
 * variety of modifications such as Health,
 * SpeedMod, Armor, etc.
 */
struct CStat : Component
{
protected:
	const float baseHitcoolDown = { 0.5f };
	float hitTimer = { 0.f };
	float hitCoolDown = { 0.5f };
	float deathTimer = { 0.f };
	float deathCoolDown = { 5.f };

	CSprite2D* sprite { nullptr };
	CParticle* particleEmitter { nullptr };
	CTransform* transform { nullptr };

	//std::random_device randDevice {};
	std::seed_seq randSeed { std::chrono::system_clock::now().time_since_epoch().count() };
	std::default_random_engine randGenerator { randSeed };

public:
	int Health { 3 };
	int Score { 1 };
	float SpeedMod { 1 };
	bool IsDead { false };
	bool IsInvincible { false };
	bool CanBeProtect { true };
	bool CanGiveScore { true };
	bool CanBeControl { false };

protected:
	eventpp::EventDispatcher<int, void(const MyEvent&), MyEventPolicies>& gameDispatcher;

public:
	CStat() = default;
	CStat(const int& mHP, const float& mSpeedMod, eventpp::EventDispatcher<int, void(const MyEvent&), MyEventPolicies>& mDispatcher) :
		Health(mHP),
		SpeedMod(mSpeedMod),
		gameDispatcher(mDispatcher)
	{}

	void Init() override
	{
		transform = &Entity->GetComponent<CTransform>();
		sprite = &Entity->GetComponent<CSprite2D>();

		//Particle is optional.
		if (Entity->HasComponent<CParticle>())
			particleEmitter = &Entity->GetComponent<CParticle>();

		hitCoolDown = HitCoolDown;
		Score = Health;
	}

	void Update(float mFT) override
	{
		CheckDeath();
		HitProtectionTimer(mFT);

		if (IsDead)
			DeathTimer(mFT);
	}

	virtual void Hit(int damage)
	{
		HitEffect();
		if (!IsDead && !IsInvincible)
		{
			HitProtection(baseHitcoolDown);
			Health -= damage;
			gameDispatcher.dispatch(MyEvent { EventNames::SoundEvent, HurtSoundPath, 0 });
			if (CanBeControl)
			{
				gameDispatcher.dispatch(MyEvent { EventNames::ScoreChange, "Lose Score", HurtPenalty });
				gameDispatcher.dispatch(MyEvent { EventNames::PlayerHPChange, "Lose HP", -1 });
			}
		}
		CheckDeath();
	}

	void HitProtection(float cooldDown)
	{
		if (!IsInvincible && CanBeProtect)
		{
			hitCoolDown = cooldDown;
			IsInvincible = true;
		}
	}

	int GetScore()
	{
		if (CanGiveScore)
		{
			CanGiveScore = false;
			return Score;
		}
		return 0;
	}

protected:
	virtual void CheckDeath()
	{
		if (Health <= 0)
		{
			if (!IsDead)
			{
				HitEffect();
				sprite->ChangeColor(sf::Color(0, 0, 0, 0));
				Health = 0;
				IsDead = true;
				IsInvincible = true;

				if (CanGiveScore)
					gameDispatcher.dispatch(MyEvent { EventNames::ScoreChange, "Get Score", GetScore() });
				if (CanBeControl)
				{
					gameDispatcher.dispatch(MyEvent { EventNames::SoundEvent, DieSoundPath2, 0 });
					gameDispatcher.dispatch(MyEvent { EventNames::GameOver, "Game Over", 0 });
				}
				else
					gameDispatcher.dispatch(MyEvent { EventNames::SoundEvent, DieSoundPath, 0 });
			}
		}
	}

	void HitProtectionTimer(float mFT)
	{
		if (!IsInvincible || IsDead || !CanBeProtect)
			return;

		hitTimer += mFT;
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

	void DeathTimer(float mFT)
	{
		deathTimer += mFT;

		if (deathTimer > deathCoolDown)
		{
			deathTimer = 0;
			Entity->Destroy();
		}
	}

	void HitEffect()
	{
		if (particleEmitter != nullptr)
		{
			particleEmitter->SetPosition(transform->Position.x, transform->Position.y);
			particleEmitter->SetParticleSpeed(100);

			float grvMod = Randomizer(-1, 1);
			float grvSpdX = grvMod * 5.f;
			grvMod = Randomizer(-1, 1);
			float grvSpdY = grvMod * 5.f;

			particleEmitter->SetGravity(grvSpdX, grvSpdY);
			particleEmitter->SetDissolve(true);

			if (CanBeControl)
			{
				particleEmitter->SetColor(sf::Color::Yellow);
				particleEmitter->SetShape(Shape::CIRCLE);
				particleEmitter->Fuel(25);
			}
			else
			{
				particleEmitter->SetColor(sf::Color::Red);
				particleEmitter->SetShape(Shape::CIRCLE);
				particleEmitter->Fuel(10);
			}
		}
	}

	int Randomizer(int min, int max)
	{
		std::uniform_int_distribution<> unif(min, max);
		int result = unif(randGenerator);

		return result;
	}
};

/*
 * (6) CPhysics
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
		//std::cout << std::to_string(PlayerBaseSpeed * mFT) << std::endl;

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
 * (7) CPlayerControl
 *
 * This Component is player controller.
 *
 * We can use this compoenet to make an entity
 * controlled by the player.
 */
struct CPlayerControl : Component
{
private:
	float slowMod { 0.5f };
	CPhysics* physics { nullptr };
	CTransform* transform { nullptr };
	CStat* stat { nullptr };

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
		stat = &Entity->GetComponent<CStat>();

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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
			slowMod = 0.5f;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
			slowMod = 1.5f;
		else
			slowMod = 1.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			physics->Velocity.x = -PlayerSpeed * stat->SpeedMod * slowMod;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			physics->Velocity.x = PlayerSpeed * stat->SpeedMod * slowMod;
		else
			physics->Velocity.x = 0;

		//Note: In SFML origin (0,0) is at the top left corner.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			physics->Velocity.y = -PlayerSpeed * stat->SpeedMod * slowMod;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			physics->Velocity.y = PlayerSpeed * stat->SpeedMod * slowMod;
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
 * (8) CSimpleEnemyControl
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
	float waitInterval { 1.f };
	float waitTimer { 0.f };
	bool waitFlag { false };

	CPhysics* physics { nullptr };
	CTransform* transform { nullptr };
	CStat* stat { nullptr };
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
		stat = &Entity->GetComponent<CStat>();

		//Since ping pong move has no target we
		//need to create a initial force.
		if (moveType == EnemyMoveType::PingPong)
		{
			physics->Velocity.x = EnemySpeed;
			physics->Velocity.y = EnemySpeed;
		}
		else if (moveType == EnemyMoveType::Charger)
			waitFlag = true;

		//Register out of bound event to prevent enemy from going out of borders.
		physics->OnOutOfBounds = [this](const sf::Vector2f& mSide) {
			this->OnOutOfBoundsEvent(mSide);
		};

		waitInterval = EnemySpeed * stat->SpeedMod * 0.005f;
	}

	void Update(float mFT) override
	{
		if (stat->IsDead)
			Stop = true;

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
			case EnemyMoveType::Charger:
				ChargerMove(mFT);
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
		SmoothRotate(mFT);

		//The length of the vector.
		float length = std::sqrt((direction.x * direction.x) + (direction.y * direction.y));

		if (length != 0)
		{
			float normalX = direction.x / length;
			float normalY = direction.y / length;
			sf::Vector2f directionNormalized(normalX, normalY);
			direction = directionNormalized;
		}

		physics->Velocity.x = EnemySpeed * stat->SpeedMod * direction.x;
		physics->Velocity.y = EnemySpeed * stat->SpeedMod * direction.y;
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

		SmoothRotate(mFT);

		float delta = std::abs(avoidRadius - distance);
		if (delta <= 1.f)
		{
			physics->Velocity.x = 0;
			physics->Velocity.y = 0;
			return;
		}

		if (distance < avoidRadius)
		{
			physics->Velocity.x = EnemySpeed * stat->SpeedMod * -direction.x;
			physics->Velocity.y = EnemySpeed * stat->SpeedMod * -direction.y;
		}
		else if (distance > avoidRadius)
		{
			physics->Velocity.x = EnemySpeed * stat->SpeedMod * direction.x * 0.5f;
			physics->Velocity.y = EnemySpeed * stat->SpeedMod * direction.y * 0.5f;
		}
	}

	void PingPongMove(float mFT)
	{
		direction = targetPos - transform->Position;
		SmoothRotate(mFT);

		if (physics->Left() <= 0.1f)
			physics->Velocity.x = EnemySpeed * stat->SpeedMod;
		else if (physics->Right() >= physics->BorderWidth - 0.1f)
			physics->Velocity.x = -EnemySpeed;

		//Note: In SFML origin (0,0) is at the top left corner.
		if (physics->Top() <= 0.1f)
			physics->Velocity.y = EnemySpeed;
		else if (physics->Bottom() >= physics->BorderHeight - 0.1f)
			physics->Velocity.y = -EnemySpeed * stat->SpeedMod;
	}

	void ChargerMove(float mFT)
	{
		direction = targetPos - transform->Position;
		float length = sqrt((direction.x * direction.x) + (direction.y * direction.y));

		if (length != 0)
		{
			float normalX = direction.x / length;
			float normalY = direction.y / length;
			sf::Vector2f directionNormalized(normalX, normalY);
			direction = directionNormalized;
		}

		SmoothRotate(mFT);

		if (waitTimer < waitInterval && waitFlag)
		{
			physics->Velocity.x = EnemySpeed * stat->SpeedMod * -direction.x * 0.5f;
			physics->Velocity.y = EnemySpeed * stat->SpeedMod * -direction.y * 0.5f;
		}
		else if (waitTimer < waitInterval && !waitFlag)
		{
			physics->Velocity.x = EnemySpeed * stat->SpeedMod * direction.x * 1.5f;
			physics->Velocity.y = EnemySpeed * stat->SpeedMod * direction.y * 1.5f;
		}
		else if (waitTimer >= waitInterval)
		{
			waitTimer = 0;
			waitFlag = !waitFlag;
		}
		waitTimer += mFT;
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
	void SmoothRotate(float mFT)
	{
		float smooth = EnemyRotateSpeed * mFT;

		//The 0 degree of atan2 is pointed the right
		//but enemy head is pointed up so we adjust it.
		float angle = std::atan2(direction.y, direction.x);
		angle = angle * (180 / std::acos(-1)); //PI: acos(-1)
		if (angle < 0)
		{
			angle = 360 - (-angle);
		}
		angle += 90;

		transform->Rotation = Lerp(transform->Rotation, angle, smooth);
		if (std::abs(angle - transform->Rotation) <= 0.1f)
			transform->Rotation = angle;
	}

	float Lerp(float a, float b, float f)
	{
		return a * (1.0 - f) + (b * f);
	}
};

/*
 * (9) CProjectile
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

/*
 * (10) CConsumable
 *
 * This Component defines an consumable item.
 *
 * Entity with this component is considered
 * a consumable item that gives an effect to
 * who picks it up with CReceiver.
 */
enum ConsumableType
{
	AttackSpeed,
	InstantKill,
	Invincible
};
struct ConsumableInfo
{
	float Multiplier;
	float Duration;
	float Timer;
	ConsumableType Type;
};
struct CConsumable : Component
{
private:
	ConsumableInfo info;

	CConsumable()
	{
		info.Multiplier = 1.f;
		info.Duration = 1.f;
		info.Type = ConsumableType::Invincible;
		info.Timer = 0;
	}

public:
	void SetInfo(float mult, float duration, ConsumableType type)
	{
		info.Multiplier = mult;
		info.Duration = duration;
		info.Type = type;
		info.Timer = 0;
	}

	ConsumableInfo GetInfo()
	{
		return info;
	}
};

/*
 * (11) CReceiver
 *
 * This Component defines an consumable receiver.
 *
 * Entity with this component can pick up and consum
 * an consumable item to receive effects.
 */
struct CReceiver : Component
{
private:
	std::vector<ConsumableInfo> currentEffects;
	CStat* stat;

public:
	void Init() override
	{
		stat = &Entity->GetComponent<CStat>();
	}

	void Update(float mFT) override
	{
		CleanEffect();
		ProcessEffect(mFT);
	}

	void ReceiveEffect(ConsumableInfo info)
	{
		currentEffects.push_back(info);
	}

private:
	void ProcessEffect(float mFT)
	{
		for (auto& e : currentEffects)
		{
			if (e.Timer < e.Duration)
			{
				e.Timer += mFT;

				switch (e.Type)
				{
					case ConsumableType::AttackSpeed:
						break;
					case ConsumableType::InstantKill:
						break;
					case ConsumableType::Invincible:
						break;
					default:
						break;
				}
			}
			else
				e.Timer = e.Duration;
		}
	}

	void CleanEffect()
	{
		//Remove expired effects.
		currentEffects.erase(
			std::remove_if(std::begin(currentEffects), std::end(currentEffects), [](const ConsumableInfo mEffect) {
				return mEffect.Timer >= mEffect.Duration;
			}),
			std::end(currentEffects));
	}
};
}
