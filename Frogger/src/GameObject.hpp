#pragma once
#include "Game.hpp"

class GameObject {
public:
	GameObject(const char* textureSheetPath, int x, int y);
	~GameObject();

	void Update();
	void Render();

private:
	int xPos, yPos;

	SDL_Texture* objTexture;
	SDL_FRect srcRect, destRect;
};