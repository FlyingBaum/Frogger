#include "Game.hpp"
#include "TextureManager.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "ECS/TileComponent.hpp"
#include <ctime>
const int GAME_SCALE = 2;
const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 10;

int initialMap[GRID_WIDTH][GRID_HEIGHT] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

std::vector<ColliderComponent*> Game::colliders;

Entity* player = nullptr;

// Each member is size_t type.
enum groupLabels : std::size_t {
	groupMap,
	groupPlayers,
	groupObstacles,
	groupColliders
};

Game::Game() {}
Game::~Game() {}

void Game::init(const char* title, int width, int height, bool isFullscreen) {
	// Seed random generator.
	srand(static_cast<unsigned int>(time(nullptr)));

	// Set flags for creating the window.
	int flags = isFullscreen ? SDL_WINDOW_FULLSCREEN : 0;

	// Create the window if the initialization of SDL worked out. Then render.
	if (SDL_Init(0) == true) {
		std::cout << "Subsystem initialized! ..." << std::endl;

		window = SDL_CreateWindow(title, width, height, flags);
		if (window) {
			std::cout << "Window created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, NULL);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created!" << std::endl;
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}

	int isPlayerTransform = true;
	int playerHurtboxWidth = 10;
	int playerHurtboxHeight = 28;

	// ECS implementation.
	player = &manager.addEntity();
	player->addComponent<TransformComponent>(GAME_SCALE, isPlayerTransform);
	player->addComponent<SpriteComponent>(true, "assets/player.png");
	player->addComponent<KeyboardController>();
	player->addComponent<ColliderComponent>("player", playerHurtboxWidth, playerHurtboxHeight);
	player->addGroup(groupPlayers);

	Game::LoadInitialMap();
	Game::SpawnCars();
	PositionPlayerAtStart();
}

void Game::handleEvents() {
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_EVENT_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}

	// If game over, check for SPACE to reset.
	if (isGameOver && event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_SPACE) {
		ResetGame();
		isGameOver = false;
	}

}

void Game::update() {
	manager.refresh();

	if (!isGameOver) {
	manager.update();

	auto& playerCollider = player->getComponent<ColliderComponent>();
		for (auto cc : colliders) {
			if (cc->tag == "car" && Collision::AABB(playerCollider, *cc)) {
				std::cout << "Player hit by car!" << std::endl;
				std::cout << "Hit space to continue!" << std::endl;
				isGameOver = true;
				break;
			}
		}
	}
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& obstacles(manager.getGroup(groupObstacles));

void Game::render() {
	SDL_RenderClear(renderer);

	for (auto& t : tiles) {
		t->draw();
	}
	for (auto& p : players) {
		p->draw();
	}
	for (auto& o : obstacles) {
		o->draw();
	}

	if (isGameOver) {
		RenderGameOverScreen();
	}

	SDL_RenderPresent(renderer);
}



void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}

void Game::RenderGameOverScreen() {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
	SDL_FRect overlay = { 0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
	SDL_RenderFillRect(renderer, &overlay);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_FRect box = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 25, 200, 50 };
	SDL_RenderRect(renderer, &box);
}

void Game::AddTile(int id, int x, int y) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 32, 32, id);
	tile.addGroup(groupMap);
}

void Game::LoadInitialMap()
{
	int type = 0;

	for (int yIndex = 0; yIndex < 10; yIndex++)
		for (int xIndex = 0; xIndex < 10; xIndex++) {
			type = initialMap[yIndex][xIndex];
			Game::AddTile(type, xIndex * 64, yIndex * 64);
		}
}

void Game::SpawnCars() {
	int tileSize = 32 * GAME_SCALE;
	float minSpeed = 2.0f;
	float maxSpeed = 10.0f;

	for (int yIndex = 0; yIndex < 10; yIndex++) {
		// Road tiles are type 2, 3, or 4.
		int tileType = initialMap[yIndex][0];
		if (tileType == 2 || tileType == 3 || tileType == 4) {
			int dir = (rand() % 2 == 0) ? -1 : 1;

			float carY = yIndex * tileSize;
			float carX = (dir < 0) ? SCREEN_WIDTH + 64 : -64;

			auto& car(manager.addEntity());
			car.addComponent<TransformComponent>(carX, carY, 32, 32, 2);
			auto& sprite = car.addComponent<SpriteComponent>("assets/car.png");
			if (dir > 0) {
				sprite.spriteFlipMode = SDL_FLIP_HORIZONTAL;
			}

			car.addComponent<ColliderComponent>("car", 30, 16);
			car.addComponent<CarComponent>(minSpeed, maxSpeed, SCREEN_WIDTH, dir, yIndex);
			car.addGroup(groupObstacles);
		}
	}
}

void Game::SpawnCarInLane(int yIndex, int dir) {
	int tileSize = 32 * GAME_SCALE;
	float minSpeed = 2.0f;
	float maxSpeed = 10.0f;

	float carY = yIndex * tileSize;
	float carX = (dir < 0) ? SCREEN_WIDTH + 64 : -64;

	auto& car(manager.addEntity());
	car.addComponent<TransformComponent>(carX, carY, 32, 32, 2);

	auto& sprite = car.addComponent<SpriteComponent>("assets/car.png");
	if (dir > 0) {
		sprite.spriteFlipMode = SDL_FLIP_HORIZONTAL;
	}

	car.addComponent<ColliderComponent>("car", 30, 16);
	car.addComponent<CarComponent>(minSpeed, maxSpeed, SCREEN_WIDTH, dir, yIndex);
	car.addGroup(groupObstacles);
}

void Game::ResetGame() {
	// Destroy old cars.
	for (auto& o : obstacles) {
		o->destroy();
	}

	// Reset player position and animation.
	PositionPlayerAtStart();

	// Re-spawn cars.
	SpawnCars();

	// Reset state flags.
	isGameOver = false;
}


void Game::PositionPlayerAtStart() {
	int tileSize = 32 * GAME_SCALE;

	auto& transform = player->getComponent<TransformComponent>();
	transform.resetPlayerPositionToStart(tileSize, GRID_WIDTH, GRID_HEIGHT);

	auto& sprite = player->getComponent<SpriteComponent>();
	sprite.Play("IdleSouth");
	sprite.spriteFlipMode = SDL_FLIP_NONE;
}
