#include "Collision.hpp"

bool Collision::AABB(const SDL_FRect& rectA, const SDL_FRect& rectB) {
	if (
		rectA.x + rectA.w >= rectB.x &&
		rectB.x + rectB.w >= rectA.x &&
		rectA.y + rectA.h >= rectB.y &&
		rectB.y + rectB.h >= rectA.y
		) {
		return true;
	}

	return false;
}