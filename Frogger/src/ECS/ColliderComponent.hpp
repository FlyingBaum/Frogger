#pragma once
#include <string>
#include "SDL3/SDL.h"
#include "Components.hpp"
#include "TransformComponent.hpp"

class ColliderComponent : public Component {
public:
	SDL_FRect collider;
	std::string tag;

	TransformComponent* transform;

	ColliderComponent(std::string t) {
		tag = t;
	}

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
	}
};