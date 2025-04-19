#pragma once

#include "Components.hpp"
#include "SDL3/SDL.h"
#include "../TextureManager.hpp"

class SpriteComponent : public Component {
private:
	TransformComponent* transform;
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
		transform = &(entity->getComponent<TransformComponent>());

		srcRect.h = srcRect.w = 32;
		srcRect.x = srcRect.y = 0;
		destRect.h = destRect.w = srcRect.h * 2; // Scaled up to double its size.
	}

	void update() override {
		destRect.x = (int)transform->position.x;
		destRect.y = (int)transform->position.y;
	}

	void draw() override {
		TextureManager::Draw(texture, srcRect, destRect);
	}
};