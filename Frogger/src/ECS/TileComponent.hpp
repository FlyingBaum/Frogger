#pragma once

#include "ECS.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "SDL3/SDL.h"

class TileComponent : public Component {
private:
	bool isFlipped = false;
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	SDL_FRect tileRect;
	int tileID;
	const char* path;

	TileComponent() = default;

	TileComponent(int x, int y, int w, int h, int id) {
		tileRect.x = x;
		tileRect.y = y;
		tileRect.w = w;
		tileRect.h = h;
		tileID = id;

		switch (tileID)
		{
		case 0:
			path = "assets/grass.png";
			break;
		case 1:
			path = "assets/water.png";
			break;
		case 2: // Fall through to case 3.
		case 3:
			path = "assets/street.png";
			isFlipped = true;
			break;
		default:
			break;
		}
	}

	void init() override {
		entity->addComponent<TransformComponent>(tileRect.x, tileRect.y, tileRect.w, tileRect.h, 2);
		transform = &entity->getComponent<TransformComponent>();

		entity->addComponent<SpriteComponent>(path, isFlipped);
		sprite = &entity->getComponent<SpriteComponent>();
	}
};