#pragma once

#include "SDL.h"
#pragma comment(lib, "SDL2")
#include "SDL_mixer.h"
#pragma comment(lib, "SDL2_mixer")
#include "SDL_image.h"
#pragma comment(lib,"SDL2_image")
#include "SDL_ttf.h"
#pragma comment(lib, "SDL2_ttf")

struct SDL2{
	SDL2() {
		SDL_Init(SDL_INIT_VIDEO);
		TTF_Init();
		Mix_Init(MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_OGG);
		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
		IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_JPG);
	}
	~SDL2() {
		Mix_CloseAudio();
		Mix_Quit();
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
	}
};