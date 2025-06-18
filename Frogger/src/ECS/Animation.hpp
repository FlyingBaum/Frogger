#pragma once

struct Animation {
	int index;
	int frames;
	int speed;

	Animation() {}

	Animation(int i, int frames, int speed) {
		index = i;
		this->frames = frames;
		this->speed = speed;
	}
};