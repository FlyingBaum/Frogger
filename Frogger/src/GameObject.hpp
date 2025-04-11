#pragma once
#include "Game.hpp"

class GameObject {
public:
	GameObject(const char* textureSheetPath, SDL_Renderer* ren, int x, int y);
	~GameObject();

	void Update();
	void Render();

private:
	int xPos, yPos;

	SDL_Texture* objTexture;
	SDL_FRect srcRect, destRect;
	SDL_Renderer* renderer;
};