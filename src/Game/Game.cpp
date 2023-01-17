#include "include/Game.h"
using namespace ComponentSystem;
using namespace std;

Game::Game()
{
	Init();
}

Game::~Game()
{
	delete this->window;
	delete this->playerWeapon;
	delete this->collisionManager;
	delete this->entityFactory;
	delete this->enemySpawner;
	delete this->hudManager;
}

void Game ::Init()
{
	//Init the window.
	this->window = new sf::RenderWindow(sf::VideoMode(ScreenWidth, ScreenHeight), "Polygon Survivors", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(144);
	this->window->setVerticalSyncEnabled(false);
	platform.setIcon(this->window->getSystemHandle());

	//For calculating delta time.
	timePoint1 = std::chrono::steady_clock::now();
	timePoint2 = std::chrono::steady_clock::now();

	//Create entity factory.
	this->entityFactory = new EntityFactory(manager);

	//Create collision manager.
	this->collisionManager = new CollisionManager(manager, gameDispatcher);

	//Create enemy Spawner.
	this->enemySpawner = new EnemySpawner(*entityFactory, *window);

	//Create Game Timer.
	this->gameClock = new GameClock(gameDispatcher);

	//Create HUD.
	this->hudManager = new HUDManager(manager, gameDispatcher);

	GameState = GameStates::Menu;

	gameDispatcher.appendListener(EventNames::GameStart, [this](int n) {
		UNUSED(n);
		this->OnGameStateChange(EventNames::GameStart);
	});
	gameDispatcher.appendListener(EventNames::Win, [this](int n) {
		UNUSED(n);
		this->OnGameStateChange(EventNames::Win);
	});
	gameDispatcher.appendListener(EventNames::GameOver, [this](int n) {
		UNUSED(n);
		this->OnGameStateChange(EventNames::GameOver);
	});
}

void Game::InitLevel()
{
	//GenerateLevel();
	gameClock->StartTimer(DefaultTimeLimit);
	hudManager->Reset();
}

void Game::InitPlayer()
{
	auto& player(entityFactory->CreatePlayer(sf::Vector2f(ScreenWidth / 2, ScreenHeight / 2), *window));
	auto& tPlayer(player.GetComponent<CTransform>());
	sf::Vector2f& playerPos(tPlayer.Position);

	this->playerWeapon = new WeaponController(WeaponType::Gun, *entityFactory, manager, gameDispatcher, *window, playerPos);
}

void Game::InitEnemy()
{
	enemySpawner->GenerateEnemy(EnemySpawnMode::Easy);
	currentSpawnCount = 5;
	currentWaveMode = EnemySpawnMode::Normal;
	spawnLock = false;
}

void Game::GenerateLevel()
{
	default_random_engine generator(time(NULL));
	uniform_real_distribution<float> unif(-300, 300);
	float randomOffestX = unif(generator);
	float randomOffestY = unif(generator);

	//Init obstcles.
	for (int i = 0; i < 10; i++)
	{
		entityFactory->CreateObstacle(sf::Vector2f(ScreenWidth / 2 + randomOffestX, ScreenHeight / 2 + randomOffestY), *window);
		randomOffestX = unif(generator);
		randomOffestY = unif(generator);
	}
}

void Game::GenerateEnemyWave()
{
	int interval = (int)gameClock->CurrentTime % WaveInterval;
	int modeInterval = (int)gameClock->CurrentTime % WaveModeInterval;

	if (gameClock->CurrentTime < 1)
		return;

	if (modeInterval == 0)
	{
		if (currentWaveMode == EnemySpawnMode::Easy)
			currentWaveMode = EnemySpawnMode::Normal;
		else if (currentWaveMode == EnemySpawnMode::Normal)
			currentWaveMode = EnemySpawnMode::Hard;
	}

	if (interval == 0 && !spawnLock)
	{
		spawnLock = true;
		enemySpawner->GenerateEnemy(currentSpawnCount, currentWaveMode);
		currentSpawnCount++;
	}
	else if (interval > 0)
	{
		spawnLock = false;
	}
}

void Game::PollingEvent()
{
	sf::Event event;
	while (this->window->pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				this->window->close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Enter)
				{
					if (GameState != GameStates::Stage)
					{
						ClearStage();
						InitPlayer();
						InitEnemy();
						gameDispatcher.dispatch(EventNames::GameStart);
						InitLevel();
					}
				}
				break;
			default:
				break;
		}
	}
}

void Game::ClearStage()
{
	//Clear Player
	auto& players(manager.GetEntitiesByGroup(EntityGroup::Player));
	for (size_t i = 0; i < players.size(); i++)
	{
		auto& p(players[i]);
		p->Destroy();
	}

	//Clear Enemies
	auto& enemies(manager.GetEntitiesByGroup(EntityGroup::Enemy));
	for (size_t i = 0; i < enemies.size(); i++)
	{
		auto& e(enemies[i]);
		e->Destroy();
	}

	//Clear Projectiles
	auto& projectiles(manager.GetEntitiesByGroup(EntityGroup::Projectile));
	for (size_t i = 0; i < projectiles.size(); i++)
	{
		auto& pj(projectiles[i]);
		pj->Destroy();
	}

	//Clear Obstacles
	auto& obstacles(manager.GetEntitiesByGroup(EntityGroup::Obstacle));
	for (size_t i = 0; i < obstacles.size(); i++)
	{
		auto& o(obstacles[i]);
		o->Destroy();
	}
	manager.Refresh(); //MUST DO THIS so that entities really get deteled.
}

void Game::PauseStage()
{
	auto& enemies(manager.GetEntitiesByGroup(EntityGroup::Enemy));
	for (size_t i = 0; i < enemies.size(); i++)
	{
		auto& e(enemies[i]);
		auto& cE(e->GetComponent<CSimpleEnemyControl>());
		cE.Stop = true;
	}
}

void Game::OnGameStateChange(EventNames state)
{
	switch (state)
	{
		case EventNames::GameStart:
			//START
			GameState = GameStates::Stage;
			break;
		case EventNames::Win:
			//WIN
			PauseStage();
			GameState = GameStates::Result;
			break;
		case EventNames::GameOver:
			//DIE
			PauseStage();
			GameState = GameStates::Result;
			break;
		default:
			break;
	}
}

//#pragma region GameLoop
void Game::FixedUpdate()
{
	//Accumulating frame time into 'currentSlice'.
	currentSlice += lastFrameTime;

	//If 'currentSlice' is greater than or equals 'ftSlice',
	//we update the game until 'currentSlice' is less than
	//'ftSlice'.
	int loop = 0;
	for (; currentSlice >= ftSlice; currentSlice -= ftSlice)
	{
		loop++;
		//Check physics in fixed update no matter what.
		if (GameState == GameStates::Stage)
			collisionManager->TestAllCollision();

		if (UseDeltaTime)
			continue;
		if (GameState == GameStates::Stage)
		{
			if (this->playerWeapon != nullptr)
				this->playerWeapon->Update(ftStep / 1000);
			//Passing 'ftStep' instead of 'lastFrameTime' because
			//we want to ensure the ideal 'frame time' is constant.
			manager.Refresh();
			manager.Update(ftStep / 1000);
		}
	}
}

void Game::Update()
{
	if (GameState == GameStates::Stage)
	{
		gameClock->RunTimer();
		GenerateEnemyWave();
	}

	if (!UseDeltaTime)
		return;
	if (GameState == GameStates::Stage)
	{
		if (this->playerWeapon != nullptr)
			this->playerWeapon->Update(frameTimeSeconds);
		manager.Refresh();
		manager.Update(frameTimeSeconds);
	}
}

void Game::Render()
{
	//clear previous frame
	this->window->clear();

	manager.Render();
	gameClock->DrawText(*window);
	hudManager->DrawHUD(*window);

	//draw the game
	this->window->display();
}

void Game::Run()
{
	//Game Loop
	while (this->window->isOpen())
	{
		timePoint1 = chrono::steady_clock::now();

		PollingEvent();
		FixedUpdate();
		Update();
		Render();

		timePoint2 = chrono::steady_clock::now();
		auto elapsedTime(timePoint2 - timePoint1);
		float frameTime {
			chrono::duration_cast<chrono::duration<float, milli>>(
				elapsedTime)
				.count()
		};
		lastFrameTime = frameTime;

		//Note: these are just for monitoring performance.
		frameTimeSeconds = (frameTime / 1000.f);
		framePerSecond = (1.f / frameTimeSeconds);
		window->setTitle(
			"[Polygon Survivors] FrameTime: " + to_string(frameTimeSeconds) + " / FPS: " + to_string(framePerSecond));
	}
	//#pragma endregion
}