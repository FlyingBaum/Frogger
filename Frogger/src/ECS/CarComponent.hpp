#pragma once
#include "Components.hpp"
#include "../Vector2D.hpp"
#include "../Game.hpp"
#include <cstdlib>
#include <ctime>

class CarComponent : public Component {
public:
    TransformComponent* transform;
    float speed;
    int direction;
    int screenWidth;
    int laneIndex;

    CarComponent(float minSpeed, float maxSpeed, int screenW, int dir = 1, int lane = 0) {
        float r = static_cast<float>(rand()) / RAND_MAX;
        speed = minSpeed + r * (maxSpeed - minSpeed);
        direction = dir;
        screenWidth = screenW;
        laneIndex = lane;
    }

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override {
        transform->position.x += speed * direction;

        // When car moves fully offscreen.
        bool offLeft = (direction < 0 && transform->position.x + transform->width * transform->scale < 0);
        bool offRight = (direction > 0 && transform->position.x > screenWidth);

        if (offLeft || offRight) {
            // Respawn new car on opposite edge.
            Game::SpawnCarInLane(laneIndex, direction);
            entity->destroy();
        }
    }
};
