#include "Game.hpp"
#include "TextureManager.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "ECS/TileComponent.hpp"

int initialMap[10][10] = {
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

auto& player(manager.addEntity());
auto& car(manager.addEntity());

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

	// ECS implementation.
	player.addComponent<TransformComponent>(2);
	player.addComponent<SpriteComponent>(true, "assets/player.png");
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	car.addComponent<TransformComponent>(300.0f, 300.0f, 32, 32, 2);
	car.addComponent<SpriteComponent>("assets/car.png");
	car.addComponent<ColliderComponent>("car");
	car.addGroup(groupObstacles);

	Game::LoadInitialMap();
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
}

void Game::update() {
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update();

	for (auto cc : colliders) Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& obstacles(manager.getGroup(groupObstacles));

// Clear everything with render color, draw and then present newly rendered stuff.
void Game::render() {
	SDL_RenderClear(renderer);
	for(auto& t : tiles)
	{
		t->draw();
	}
	for(auto& p : players)
	{
		p->draw();
	}
	for(auto& o : obstacles)
	{
		o->draw();
	}
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
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