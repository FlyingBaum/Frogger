#pragma once

#include "Components.hpp"
#include "../Vector2D.hpp"

class TransformComponent : public Component {
private: 
	const int JUMP_DISTANCE = 64;
	Vector2D targetLocation;
public:
	Vector2D position;
	Vector2D velocity;

	bool isPlayer = false;
	bool isPlayerMoving = false;

	int height = 32;
	int width = 32;
	int scale = 1;

	int speed = 2;

	TransformComponent() {
		position.Zero();
	}

	TransformComponent(int sc, bool isPlayer = false) {
		position.Zero();
		scale = sc;
		this->isPlayer = isPlayer;
	}

	TransformComponent(float x, float y, bool isPlayer = false) {
		position.x = x;
		position.y = y;
		this->isPlayer = isPlayer;
	}

	TransformComponent(float x, float y, int h, int w, int sc, bool isPlayer = false) {
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
		this->isPlayer = isPlayer;
	}

	void init() override {
		velocity.Zero();
	}

	void update() override {
		if (isPlayer) {
			playerUpdate();
			return;
		}
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

	void playerUpdate() {
		if (isPlayerMoving) {
			position.x += velocity.x * speed;
			position.y += velocity.y * speed;
		}

		if (isPlayerMoving && targetLocation.x == position.x && targetLocation.y == position.y) {
			isPlayerMoving = false;
			velocity.x = 0;
			velocity.y = 0;
		}
	}

	void setPlayerInMotion() {
		isPlayerMoving = true;

		targetLocation.x += velocity.x * JUMP_DISTANCE;
		targetLocation.y += velocity.y * JUMP_DISTANCE;
	}
};