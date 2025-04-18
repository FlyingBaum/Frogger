#pragma once

#include "Components.hpp"

class PositionComponent : public Component {
private:
	int xPos;
	int yPos;

public:
	PositionComponent() {
		xPos = 0;
		yPos = 0;
	}

	PositionComponent(int x, int y) {
		xPos = x;
		yPos = y;
	}

	// Getters.
	int getXPos() { return xPos; }
	int getYPos() { return yPos; }

	// Setters
	int setXPos(int x) { xPos = x; }
	int setYPos(int y) { yPos = y; }

	void setPos(int x, int y) {
		xPos = x;
		yPos = y;
	}

	void update() override {
		xPos++;
		yPos++;
	}

};