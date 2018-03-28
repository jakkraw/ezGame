#include <memory>
#include "interface/extern.h"
#include "gameSDL.h"
#include "resources.h"

void ezGame::start(Game*&& game)
{
	Title title = "ezGame";
	Resolution res = { 640,480 };
	Size size = res;

	SDL_Init(SDL_INIT_VIDEO);
	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window = {
		SDL_CreateWindow("ezGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,  size.width, size.height, NULL),
		SDL_DestroyWindow
	};
	auto icon = IMG_Load_RW(SDL_RWFromConstMem(ez_ico, ez_ico_size), true);
	SDL_SetWindowIcon(window.get(), icon);
	SDL_FreeSurface(icon);

	

	std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer = {
		SDL_CreateRenderer(window.get(), -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED | SDL_RendererFlags::SDL_RENDERER_TARGETTEXTURE),
		SDL_DestroyRenderer
	};

	SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(renderer.get(), 640, 480);

	//SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_SetWindowGrab(window.get(), SDL_TRUE);
	SDL_WarpMouseInWindow(window.get(), res.width / 2, res.height / 2);
	//SDL_ShowCursor(0);

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_JPG);

	Mix_Init(MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_OGG);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	TTF_Init();

	{
		GameSDL gameSDL(std::move(game), SDLData(*window, *renderer));
		while (gameSDL.isRunning()) gameSDL.update();
	}

	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}