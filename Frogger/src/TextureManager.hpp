#pragma once

#include "Game.hpp"

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* fileName, SDL_FlipMode flipMode = SDL_FLIP_NONE);
	static void Draw(SDL_Texture* texture, SDL_FRect src, SDL_FRect dest, SDL_FlipMode flipMode = SDL_FLIP_NONE);
};