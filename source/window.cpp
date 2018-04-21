#include "window.h"

#include "icon.h"

using namespace ezGame;

WindowI::WindowI(Title title, Resolution res, Size size) {
	SDL_Init(SDL_INIT_VIDEO);

	windowSDL.reset(SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		size.width, size.height, NULL));

	resource::setIcon(*windowSDL);
	SDL_SetWindowGrab(&*windowSDL, SDL_TRUE);
	SDL_WarpMouseInWindow(&*windowSDL, res.width / 2, res.height / 2);

	rendererSDL.reset(SDL_CreateRenderer(&*windowSDL, -1,
		SDL_RendererFlags::SDL_RENDERER_ACCELERATED |
		SDL_RendererFlags::SDL_RENDERER_TARGETTEXTURE));

	SDL_SetRenderDrawBlendMode(&*rendererSDL, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(&*rendererSDL, res.width, res.height);
}

WindowI::~WindowI() { SDL_Quit(); }

SDL_Renderer & WindowI::renderer() const { return *rendererSDL; }

void WindowI::update(const SDL_Event & event) {
	switch (event.type)
	{
	case SDL_WINDOWEVENT:
		if (event.window.event == SDL_WINDOWEVENT_CLOSE)
			close();
		break;
	}
}

bool WindowI::isOpen() const { return open; }

void WindowI::close() { open = false; }

// Inherited via Window

Window::Settings & WindowI::settings() const { return (Window::Settings&)*this; }

// Inherited via Settings

void WindowI::set(Title title) {
	SDL_SetWindowTitle(&*windowSDL, title);
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

void WindowI::set(Resolution resolution) {
	SDL_SetWindowSize(&*windowSDL, resolution.width, resolution.height);
	SDL_RenderSetLogicalSize(&*rendererSDL, resolution.width, resolution.height);
}

Window::Settings::Resolution WindowI::resolution() const {
	int x, y;
	SDL_RenderGetLogicalSize(&*rendererSDL, &x, &y);
	return{ x,y };
}
