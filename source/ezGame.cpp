#include "interface/ezGame.h"

#include "draw.h"
#include "input.h"
#include "audio.h"
#include "window.h"

void ::ezGame::start(const Game& g)
{
	using Title = ezGame::Window::Settings::Title;
	Title title("ezGame");
	Resolution res(640, 480);
	Size size(res);

	SDL_Init(SDL_INIT_VIDEO);

	using WindowSDL = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;
	WindowSDL windowSDL = { 
		SDL_CreateWindow(title, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		size.width, size.height, NULL), 
		SDL_DestroyWindow 
	};
	resource::setWindowIcon(*windowSDL);
	SDL_SetWindowGrab(&*windowSDL, SDL_TRUE);
	SDL_WarpMouseInWindow(&*windowSDL, res.width / 2, res.height / 2);

	using RendererSDL = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
	RendererSDL rendererSDL = {
		SDL_CreateRenderer(&*windowSDL, -1,
		SDL_RendererFlags::SDL_RENDERER_ACCELERATED | 
		SDL_RendererFlags::SDL_RENDERER_TARGETTEXTURE),
		SDL_DestroyRenderer 
	};
	SDL_SetRenderDrawBlendMode(&*rendererSDL, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(&*rendererSDL, res.width, res.height);


	::Input input;
	::Draw draw(*rendererSDL);
	::Audio audio;
	::Window window(*windowSDL,*rendererSDL);
	::Game game(std::unique_ptr<Game>(g));

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

	SDL_Quit();
}
