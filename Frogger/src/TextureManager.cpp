#include "TextureManager.hpp"

// Create a temporary surface to load into the texture and destroy it.
SDL_Texture* TextureManager::LoadTexture(const char* texturePath, SDL_FlipMode flipMode) {
	SDL_Surface* tempSurface = IMG_Load(texturePath);
	SDL_FlipSurface(tempSurface, flipMode);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST); // Set scale mode to nearest for sharp pixel art.
	SDL_DestroySurface(tempSurface);

	return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_FRect src, SDL_FRect dest, SDL_FlipMode flipMode)
{
	SDL_RenderTextureRotated(Game::renderer, texture, &src, &dest, NULL, NULL, flipMode);
}
