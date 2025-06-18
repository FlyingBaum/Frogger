#pragma once

#include "../Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class KeyboardController : public Component {
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {
		if (transform->isPlayerMoving) return; // If player has not reached target location then do not process input.

		if (Game::event.type == SDL_EVENT_KEY_DOWN) {
			switch (Game::event.key.key)
			{
			case SDLK_W:
				transform->velocity.y = -1;
				transform->setPlayerInMotion();
				sprite->Play("JumpingNorth", true);
				break;
			case SDLK_S:
				transform->velocity.y = 1;
				transform->setPlayerInMotion();
				sprite->Play("JumpingSouth", true);
				break;
			case SDLK_A:
				transform->velocity.x = -1;
				transform->setPlayerInMotion();
				sprite->Play("JumpingWest", true);
				sprite->spriteFlipMode = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_D:
				transform->velocity.x = 1;
				transform->setPlayerInMotion();
				sprite->Play("JumpingEast", true);
				sprite->spriteFlipMode = SDL_FLIP_NONE;
				break;
			default:
				break;
			}
		}
	}
};