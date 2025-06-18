#pragma once

#include "../Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class KeyboardController : public Component {
private:
    bool hasBufferedInput = false;
    Vector2D bufferedVelocity;
    std::string bufferedAnimation;
    SDL_FlipMode bufferedFlip = SDL_FLIP_NONE;

public:
    TransformComponent* transform;
    SpriteComponent* sprite;

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override {
        // Process buffered input if player has stopped moving.
        if (hasBufferedInput && !transform->isPlayerMoving) {
            transform->velocity = bufferedVelocity;
            transform->setPlayerInMotion();
            sprite->Play(bufferedAnimation, true);
            sprite->spriteFlipMode = bufferedFlip;

            hasBufferedInput = false;
            return;
        }

        // Don't process new input if player is still moving and not close to target.
        if (transform->isPlayerMoving && !transform->isPlayerCloseToTarget()) {
            return;
        }

        // Handle new input.
        if (Game::event.type == SDL_EVENT_KEY_DOWN) {
            Vector2D newVelocity(0, 0);
            std::string animation;
            SDL_FlipMode flip = SDL_FLIP_NONE;

            switch (Game::event.key.key) {
            case SDLK_W:
                newVelocity.y = -1;
                animation = "JumpingNorth";
                break;
            case SDLK_S:
                newVelocity.y = 1;
                animation = "JumpingSouth";
                break;
            case SDLK_A:
                newVelocity.x = -1;
                animation = "JumpingWest";
                flip = SDL_FLIP_HORIZONTAL;
                break;
            case SDLK_D:
                newVelocity.x = 1;
                animation = "JumpingEast";
                flip = SDL_FLIP_NONE;
                break;
            default:
                return;
            }

            // If player is moving but close to target, buffer the input.
            if (transform->isPlayerMoving && transform->isPlayerCloseToTarget()) {
                bufferedVelocity = newVelocity;
                bufferedAnimation = animation;
                bufferedFlip = flip;
                hasBufferedInput = true;
            }
            // Otherwise process immediately.
            else {
                transform->velocity = newVelocity;
                transform->setPlayerInMotion();
                sprite->Play(animation, true);
                sprite->spriteFlipMode = flip;
            }
        }
    }
};