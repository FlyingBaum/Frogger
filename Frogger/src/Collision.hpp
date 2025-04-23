#pragma once
#include <SDL3/SDL.h>
#include "ECS/ColliderComponent.hpp"

class Collision {
public:
	static bool AABB(const SDL_FRect& rectA, const SDL_FRect& rectB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
};