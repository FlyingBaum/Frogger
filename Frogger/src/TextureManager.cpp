#include "TextureManager.hpp"

// Create a temporary surface to load into the texture and destroy it.
SDL_Texture* TextureManager::LoadTexture(const char* texturePath, SDL_Renderer* ren) {
	SDL_Surface* tempSurface = IMG_Load(texturePath);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, tempSurface);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST); // Set scale mode to nearest for sharp pixel art.
	SDL_DestroySurface(tempSurface);

	return texture;
}