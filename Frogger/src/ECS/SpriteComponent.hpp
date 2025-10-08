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
    bool isTile = false;

    int currentFrame = 0;
    int frames = 0;
    int speed = 200; // Delay between frames in ms.
    Uint32 lastFrameTime = 0;

    bool shouldPlayOnce = false;
    bool isAnimationCompleted = false;
    bool shouldSwitchToIdle = false;

    int jumpAnimationSpeed = 120;

public:
    int animationIndex = 0;
    std::map<std::string, Animation> animations;

    SDL_FlipMode spriteFlipMode = SDL_FLIP_NONE;

    SpriteComponent() = default;

    SpriteComponent(const char* path, SDL_FlipMode flipMode = SDL_FLIP_NONE, const bool isTile = false) {
        this->isTile = isTile;
        spriteFlipMode = flipMode;
        setTexture(path);
    }

    SpriteComponent(bool isAnimated, const char* path, SDL_FlipMode flipMode = SDL_FLIP_NONE) {
        spriteFlipMode = flipMode;
        this->isAnimated = isAnimated;

        // Create a list of animations with their respective keys.
        std::vector<std::pair<std::string, Animation>> animationList = {
            {"IdleSouth", Animation(0, 2, 500)},
            {"JumpingSouth", Animation(1, 3, jumpAnimationSpeed)},
            {"IdleNorth", Animation(2, 2, 500)},
            {"JumpingNorth", Animation(3, 3, jumpAnimationSpeed)},
            {"IdleEast", Animation(4, 2, 500)},
            {"JumpingEast", Animation(5, 3, jumpAnimationSpeed)},
            {"IdleWest", Animation(4, 2, 500)},
            {"JumpingWest", Animation(5, 3, jumpAnimationSpeed)}
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
        if (shouldSwitchToIdle) switchToIdle();

        if (isAnimated && !isAnimationCompleted) updateAnimation();

        destRect.x = static_cast<int>(transform->position.x);
        destRect.y = static_cast<int>(transform->position.y);
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;

        // Adjust the y-position for certain jump animation frames
        if (isJumpAnimation() && currentFrame % 3 == 2) {
            destRect.y -= 10;  // Adjust this offset as needed
        }
        
        // Get the correct y for the spritesheet if the animationIndex has changed.
        srcRect.y = animationIndex * transform->height;
    }

    void draw() override {
        TextureManager::Draw(texture, srcRect, destRect, !isTile ? spriteFlipMode : SDL_FLIP_NONE);
    }

    void Play(const std::string& animationName, bool shouldPlayOnce = false) {
        frames = animations[animationName].frames;
        animationIndex = animations[animationName].index;
        speed = animations[animationName].speed;
        currentFrame = 0;  // Reset the frame to start from the beginning

        this->shouldPlayOnce = shouldPlayOnce;
        this->isAnimationCompleted = false;
    }

    void switchToIdle() {
        switch (animationIndex) {
        case 1:
            Play("IdleSouth");
            break;
        case 3:
            Play("IdleNorth");
            break;
        case 5:
            Play("IdleEast");
            break;
        case 7:
            Play("IdleWest");
            break;
        default:
            break;
        }
        shouldSwitchToIdle = false;
    }

    void updateAnimation() {
        Uint32 now = SDL_GetTicks();
        if (now - lastFrameTime > speed) {
            lastFrameTime = now;
            currentFrame++;
            if (currentFrame >= frames) {
                currentFrame = 0;
                if (shouldPlayOnce) {
                    isAnimationCompleted = true;
                    shouldSwitchToIdle = true;
                }
            }
        }
        srcRect.x = currentFrame * srcRect.w;
    }

    bool isJumpAnimation() const {
        return (animationIndex == 1 || animationIndex == 3 || animationIndex == 5 || animationIndex == 7);
    }
};