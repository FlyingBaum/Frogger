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
		if (Game::event.type == SDL_EVENT_KEY_DOWN) {
			switch (Game::event.key.key)
			{
			case SDLK_W:
				transform->velocity.y = -1;
				sprite->Play("JumpingNorth");
				break;
			case SDLK_S:
				transform->velocity.y = 1;
				sprite->Play("JumpingSouth");
				break;
			case SDLK_A:
				transform->velocity.x = -1;
				sprite->Play("JumpingWest");
				sprite->spriteFlipMode = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_D:
				transform->velocity.x = 1;
				sprite->Play("JumpingEast");
				sprite->spriteFlipMode = SDL_FLIP_NONE;
				break;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_EVENT_KEY_UP) {
			switch (Game::event.key.key)
			{
			case SDLK_W:
				transform->velocity.y = 0;
				sprite->Play("IdleNorth");
				break;
			case SDLK_S:
				transform->velocity.y = 0;
				sprite->Play("IdleSouth");
				break;
			case SDLK_A:
				transform->velocity.x = 0;
				sprite->Play("IdleWest");
				break;
			case SDLK_D:
				transform->velocity.x = 0;
				sprite->Play("IdleEast");
				break;
			default:
				break;
			}
		}
	}
};