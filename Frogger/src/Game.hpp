#pragma once

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include <stdio.h>
#include <iostream>
#include <vector>

class ColliderComponent;

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool isFullscreen = false);

	void handleEvents();
	void update();
	void render();
	void clean();

	bool getIsRunning() { return isRunning; }

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;

private:
	int nCount = 0;
	bool isRunning;
	SDL_Window* window;
};
