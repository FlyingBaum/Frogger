#include "GameObject.hpp"
#include "TextureManager.hpp"

GameObject::GameObject(const char* textureSheetPath, SDL_Renderer* ren, int x, int y) {
	// Set initial position of object.
	xPos = x;
	yPos = y;

	renderer = ren;
	objTexture = TextureManager::LoadTexture(textureSheetPath, ren);
}

void GameObject::Update() {
	xPos++;
	yPos++;

	// Whole source sprite.
	srcRect.h = 32;
	srcRect.w = 32;
	srcRect.x = 0;
	srcRect.y = 0;
	
	// Scaled up to double its size.
	destRect.h = srcRect.h * 2;
	destRect.w = srcRect.h * 2;
	destRect.x = xPos;
	destRect.y = yPos;
}

void GameObject::Render() {
	SDL_RenderTexture(renderer, objTexture, &srcRect, &destRect);
}