#include "Game.hpp"
#include "TextureManager.hpp"

SDL_Texture* playerTexture;
SDL_FRect srcR, destR;

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
			// TODO: Once SDL3 version 3.40 is out use "SDL_SetDefaultTextureScaleMode" here.
			std::cout << "Renderer created!" << std::endl;
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}

	playerTexture = TextureManager::LoadTexture("assets/dad.png", renderer);
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
	nCount++;

	destR.w = 64;
	destR.h = 64;
	destR.x = nCount;

	std::cout << nCount << std::endl;
}

void Game::render() {
	SDL_RenderClear(renderer);								// Clear everything with the render color.
	SDL_RenderTexture(renderer, playerTexture, NULL, &destR); // Render player.
	SDL_RenderPresent(renderer);							// Present all the newly rendered stuff.
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}