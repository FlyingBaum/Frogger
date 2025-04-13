#pragma once

#include "Game.hpp"

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* fileName, bool isToBeFlipped = false);
	static void Draw(SDL_Texture* texture, SDL_FRect src, SDL_FRect dest);
};