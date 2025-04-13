#include "Game.hpp"

Game* game = nullptr;

int main(int argc, char* argv[]) {
	// FPS and delay per frame.
	const int FPS = 60;
	const int FRAME_DELAY = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	game = new Game();

	game->init("FroggerEngine", 640, 640, false);

	while (game->getIsRunning()) {
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		// Delay the frame further for consistent frame durations.
		if (FRAME_DELAY > frameTime) {
			SDL_Delay(FRAME_DELAY - frameTime);
		}
	}

	game->clean();

	return 0;
}
