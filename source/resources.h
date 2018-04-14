#pragma once
#pragma warning( push )
#pragma warning( disable : 4838)
#pragma warning( disable : 4309)

#include "../resources/exampleFont.ttf.h"
#include "../resources/ez.ico.h"

#pragma warning( pop ) 

#include "SDL_ttf.h"

namespace resource {
	TTF_Font* loadFont() {
		return TTF_OpenFontRW(SDL_RWFromConstMem(exampleFont, exampleFont_size), true, 86);
	}

	void setWindowIcon(SDL_Window& window) {
		auto icon = IMG_Load_RW(SDL_RWFromConstMem(ez_ico, ez_ico_size), true);
		SDL_SetWindowIcon(&window, icon);
		SDL_FreeSurface(icon);
	}
}