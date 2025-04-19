#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;

auto& player(manager.addEntity());

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

	map = new Map();

	// ECS implementation.
	auto& playerTransform = player.addComponent<TransformComponent>();
	auto& playerSprite = player.addComponent<SpriteComponent>("assets/dad.png");
}

void Game::handleEvents() {
	SDL_Event event;
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
	manager.refresh();
	manager.update();

	auto& playerTransform = player.getComponent<TransformComponent>();
	playerTransform.position.Add(Vector2D(0, 1));

	if (playerTransform.position.y > 100) {
		player.getComponent<SpriteComponent>().setTexture("assets/car.png");
	}
}

void Game::render() {
	SDL_RenderClear(renderer);								// Clear everything with the render color.

	// Render map first, then the entities.
	map->DrawMap();
	manager.draw();
	SDL_RenderPresent(renderer);							// Present all the newly rendered stuff.
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}