#include "Map.hpp"
#include "TextureManager.hpp"

int lvl1[15][10] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

};

Map::Map() {
	// Load tiles.
	grass = TextureManager::LoadTexture("assets/grass.png");			// 0.
	water = TextureManager::LoadTexture("assets/water.png");			// 1.
	streetTop = TextureManager::LoadTexture("assets/street.png", true); // 2.
	streetBottom = TextureManager::LoadTexture("assets/street.png");	// 3.

	// Load map in.
	LoadMap(lvl1);

	// Set source / dest tile initial x and y.
	src.x = src.y = 0;
	dest.x = dest.y = 0;

	// Add source tile width and height(32x32).
	src.w = 32;
	src.h = 32;

	// Scale up dest tile width and height by factor 2.
	dest.w = src.w * 2;
	dest.h = src.h * 2;
}

void Map::LoadMap(int arr[10][10]) {
	for (int row = 0; row < 10; row++)
		for (int column = 0; column < 10; column++) {
			map[row][column] = arr[row][column];
		}
}

// Draws the loaded in map tiles.
void Map::DrawMap() {
	int type = 0;

	for (int row = 0; row < 10; row++)
		for (int column = 0; column < 10; column++) {
			type = map[row][column];

			// Dest x and y should reflect position of the tile to be inserted.
			dest.x = column * 64;
			dest.y = row * 64;
			
			switch (type)
			{
			case 0:
				TextureManager::Draw(grass, src, dest);
				break;
			case 1:
				TextureManager::Draw(water, src, dest);
				break;
			case 2:
				TextureManager::Draw(streetTop, src, dest);
				break;
			case 3:
				TextureManager::Draw(streetBottom, src, dest);
				break;
			default:
				// TODO: Handle invalid types.
				break;
			}
		}
}
