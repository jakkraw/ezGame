#include "window.h"

#include "icon.h"

using namespace ezGame;

WindowI::WindowI(Title t, Resolution r, Size s) {
	windowSDL.reset(SDL_CreateWindow(t,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		s.width, s.height, NULL));

	resource::setIcon(*windowSDL);
	//SDL_SetWindowGrab(&*windowSDL, SDL_TRUE);
	SDL_WarpMouseInWindow(&*windowSDL, r.width / 2, r.height / 2);

	rendererSDL.reset(SDL_CreateRenderer(&*windowSDL, -1,
		SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_TARGETTEXTURE));

	SDL_SetRenderDrawBlendMode(&*rendererSDL, SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(&*rendererSDL, r.width, r.height);
}

SDL_Renderer & WindowI::renderer() const{
	return *rendererSDL;
}

void WindowI::update(const SDL_Event & event) {
	switch (event.type){
	case SDL_WINDOWEVENT:
		if (event.window.event == SDL_WINDOWEVENT_CLOSE)
			close();
		break;
	}
}

bool WindowI::isOpen() const { return open; }

void WindowI::close() { open = false; }

Window::Settings & WindowI::settings() const { 
	return (Window::Settings&)*this;
}

void WindowI::set(Title t) {
	SDL_SetWindowTitle(&*windowSDL, t);
}

Window::Settings::Title WindowI::title() const {
	return SDL_GetWindowTitle(&*windowSDL);
}

void WindowI::set(CursorVisibility cv) {
	switch (cv) {
	case CursorVisibility::HIDDEN: SDL_ShowCursor(0); break;
	case CursorVisibility::SHOWN: SDL_ShowCursor(1); break;
	}
}

Window::Settings::CursorVisibility WindowI::cursorVisibility() const {
	return SDL_ShowCursor(-1) ? CursorVisibility::SHOWN : CursorVisibility::HIDDEN;
}

void WindowI::set(Type type) {
	switch (type) {
	case Type::FULLSCREEN:
		SDL_SetWindowFullscreen(&*windowSDL, SDL_WINDOW_FULLSCREEN); break;
	case Type::WINDOWED:
		SDL_SetWindowFullscreen(&*windowSDL, SDL_WINDOW_SHOWN); break;
	}
}

Window::Settings::Type WindowI::type() const {
	SDL_GetWindowFlags(&*windowSDL);
	return SDL_GetWindowFlags(&*windowSDL) & SDL_WINDOW_FULLSCREEN
		? Type::FULLSCREEN : Type::WINDOWED;
}

void WindowI::set(Resolution r) {
	SDL_SetWindowSize(&*windowSDL, r.width, r.height);
	SDL_RenderSetLogicalSize(&*rendererSDL, r.width, r.height);
}

Window::Settings::Resolution WindowI::resolution() const {
	int x, y;
	SDL_RenderGetLogicalSize(&*rendererSDL, &x, &y);
	return Resolution( x,y );
}
