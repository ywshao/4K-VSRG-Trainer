#include "game.h"

int main(int argc, char** args) {
	Game game;
	game.init();
	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
			{
				game.exit();
				return 0;
			}
		}
		game.update();
	}
	return 0;
}
