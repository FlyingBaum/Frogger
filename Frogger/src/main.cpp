#include "Game.hpp"

Game* game = nullptr;

int main(int argc, char* argv[]) {
	game = new Game();

	game->init("FroggerEngine", 800, 600, false);

	while (game->getIsRunning()) {
		game->handleEvents();
		game->update();
		game->render();
	}

	game->clean();

	return 0;
}
