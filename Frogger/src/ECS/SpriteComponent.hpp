#pragma once

#include "Components.hpp"
#include "SDL3/SDL.h"
#include "../TextureManager.hpp"

class SpriteComponent : public Component {
private:
	PositionComponent* position;
	SDL_Texture* texture;
	SDL_FRect srcRect, destRect;

public:
	SpriteComponent() = default;
	SpriteComponent(const char* path) {
		setTexture(path);
	}

	void setTexture(const char* path) {
		texture = TextureManager::LoadTexture(path);
	}

	void init() override {
		position = &(entity->getComponent<PositionComponent>());

		srcRect.h = srcRect.w = 32;
		srcRect.x = srcRect.y = 0;
		destRect.h = destRect.w = srcRect.h * 2; // Scaled up to double its size.
	}

	void update() override {
		destRect.x = position->getXPos();
		destRect.y = position->getYPos();
	}

	void draw() override {
		TextureManager::Draw(texture, srcRect, destRect);
	}
};