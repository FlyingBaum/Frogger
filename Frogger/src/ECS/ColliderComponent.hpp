#pragma once
#include <string>
#include "SDL3/SDL.h"
#include "Components.hpp"
#include "TransformComponent.hpp"
#include "../Game.hpp"

class ColliderComponent : public Component {
public:
    SDL_FRect collider;
    std::string tag;

    TransformComponent* transform;

    int hitboxWidth;
    int hitboxHeight;
    float offsetX; // Offset from center.
    float offsetY;

    ColliderComponent(std::string t) {
        tag = t;
        hitboxWidth = 32;
        hitboxHeight = 32;
        offsetX = 0.0f;
        offsetY = 0.0f;
    }

    ColliderComponent(std::string t, int w, int h, float ox = 0.0f, float oy = 0.0f) {
        tag = t;
        hitboxWidth = w;
        hitboxHeight = h;
        offsetX = ox;
        offsetY = oy;
    }

    void init() override {
        if (!entity->hasComponent<TransformComponent>()) {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();

        Game::colliders.push_back(this);

        collider.x = transform->position.x;
        collider.y = transform->position.y;
        collider.w = static_cast<float>(hitboxWidth * transform->scale);
        collider.h = static_cast<float>(hitboxHeight * transform->scale);
    }

    void update() override {
        float spriteWorldW = transform->width * transform->scale;
        float spriteWorldH = transform->height * transform->scale;
        float centerX = transform->position.x + spriteWorldW * 0.5f;
        float centerY = transform->position.y + spriteWorldH * 0.5f;

        float collW = static_cast<float>(hitboxWidth) * transform->scale;
        float collH = static_cast<float>(hitboxHeight) * transform->scale;

        float worldOffsetX = offsetX * transform->scale;
        float worldOffsetY = offsetY * transform->scale;

        collider.x = centerX - collW * 0.5f + worldOffsetX;
        collider.y = centerY - collH * 0.5f + worldOffsetY;
        collider.w = collW;
        collider.h = collH;
    }
};
