#pragma once

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include <stdio.h>
#include <iostream>
#include <vector>

class ColliderComponent;

class Game {
public:
	bool getIsRunning() { return isRunning; }
	int getScreenWidth() { return SCREEN_WIDTH; }
	int getScreenHeight() { return SCREEN_HEIGHT; }

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;

	Game();
	~Game();

	void init(const char* title, int width, int height, bool isFullscreen = false);

	void handleEvents();
	void update();
	void render();
	void clean();

	static void AddTile(int id, int x, int y);
	static void LoadInitialMap();
	static void SpawnCars();
	static void SpawnCarInLane(int yIndex, int dir);
	void ResetGame();
	void PositionPlayerAtStart();

private:
	int nCount = 0;
	bool isRunning;
	bool isGameOver = false;
	SDL_Window* window;

	static const int SCREEN_WIDTH = 640;
	static const int SCREEN_HEIGHT = 640;
};
