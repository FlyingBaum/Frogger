#pragma once

#include "Components.hpp"
#include "SDL3/SDL.h"
#include "../TextureManager.hpp"
#include "Animation.hpp"
#include <map>

class SpriteComponent : public Component {
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_FRect srcRect, destRect;

	bool isAnimated = false;
	int frames = 0;
	int speed = 100; // Delay between frames in ms.

public:
	int animationIndex = 0;
	std::map<const char*, Animation> animations;

	SDL_FlipMode spriteFlipMode = SDL_FLIP_NONE;

	SpriteComponent() = default;

	SpriteComponent(const char* path, SDL_FlipMode flipMode = SDL_FLIP_NONE) {
		spriteFlipMode = flipMode;
		setTexture(path);
	}

	SpriteComponent(bool isAnimated, const char* path, SDL_FlipMode flipMode = SDL_FLIP_NONE) {
		spriteFlipMode = flipMode;
		this->isAnimated = isAnimated;

		// Create a list of animations with their respective keys.
		std::vector<std::pair<const char*, Animation>> animationList = {
			{"IdleSouth", Animation(0, 2, 500)},
			{"JumpingSouth", Animation(1, 3, 400)},
			{"IdleNorth", Animation(2, 2, 500)},
			{"JumpingNorth", Animation(3, 3, 400)},
			{"IdleEast", Animation(4, 2, 500)},
			{"JumpingEast", Animation(5, 3, 400)},
			{"IdleWest", Animation(4, 2, 500)},
			{"JumpingWest", Animation(5, 3, 400)}
		};
		// Emplace all animations into the map.
		for (const auto& animPair : animationList) {
			animations.emplace(animPair);
		}

		Play("IdleSouth");

		setTexture(path);
	}

	~SpriteComponent() {
		SDL_DestroyTexture(texture);
	}

	void setTexture(const char* path) {
		texture = TextureManager::LoadTexture(path, spriteFlipMode);
	}

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override {
		if (isAnimated) {
			int currentFrame = (SDL_GetTicks() / speed) % frames;
			srcRect.x = static_cast<int>(srcRect.w) * currentFrame;
		}
		// Get the correct y for the spritesheet if the animationIndex has changed.
		srcRect.y = animationIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x);
		destRect.y = static_cast<int>(transform->position.y);
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override {
		TextureManager::Draw(texture, srcRect, destRect, spriteFlipMode);
	}

	void Play(const char* animationName) {
		frames = animations[animationName].frames;
		animationIndex = animations[animationName].index;
		speed = animations[animationName].speed;
	}
};