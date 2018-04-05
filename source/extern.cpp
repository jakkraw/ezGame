#include "interface/extern.h"
#include "gameSDL.h"

void ezGame::start(Game*&& game)
{
	GameSDL(std::unique_ptr<Game>(game)).run();
}

struct SDL_Lifetime {
	SDL_Lifetime() {
		SDL_Init(SDL_INIT_VIDEO);
		IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_JPG);
		Mix_Init(MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_OGG);
		TTF_Init();
	}
	~SDL_Lifetime() {
		TTF_Quit();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
	}
};
extern const SDL_Lifetime lifetime = SDL_Lifetime();