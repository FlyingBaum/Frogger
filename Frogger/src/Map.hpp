#pragma once
#include "Game.hpp"

class Map {
public:
	Map();
	~Map();

	void LoadMap(int arr[10][10]);
	void DrawMap();

private:
	SDL_FRect src, dest;

	SDL_Texture* streetTop;
	SDL_Texture* streetBottom;
	SDL_Texture* grass;
	SDL_Texture* water;

	int map[20][25];
};