#include "interface/extern.h"
#include "gameSDL.h"

void ezGame::start(Game*&& g)
{
	SDL sdl;
	InputImpl input;
	DrawImpl draw(sdl);
	AudioImpl audio(sdl);
	WindowImpl window(sdl);
	GameImpl game(std::unique_ptr<Game>(g));



	g.setup()

	FlowImpl flow()

	while (window.isOpen()) {
		draw.reset();
		input.reset();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			input.update(event);
			draw.update(event);
			window.update(event);
		}



	}



}
