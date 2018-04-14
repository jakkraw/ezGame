#include <atomic>
#include "SDL.h"

#include "interface\window.h"
using namespace ezGame;
class Window : 
	public virtual ::ezGame::Window,
	public virtual ::ezGame::Window::Settings 
{
	SDL_Window& window;
	SDL_Renderer& renderer;
	std::atomic<bool> open = true;
public:	
	Window(SDL_Window& window, SDL_Renderer& renderer)
		: renderer(renderer), window(window) {}

	virtual void update(const SDL_Event& event) {
		switch (event.type)
		{
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_CLOSE)
				close();
			break;
		}
	}

	bool isOpen() { return open; }

	virtual void close() override { open = false; }

	// Inherited via Window
	virtual Settings & settings() const override { return (Settings&)*this; }

	// Inherited via Settings
	virtual void set(Title title) override {
		SDL_SetWindowTitle(&window, title);
	}
	virtual const Title title() const override {
		SDL_GetWindowTitle(&window);
	}

	virtual void set(CursorVisibility cv) override {
		switch (cv) {
		case CursorVisibility::Hidden: SDL_ShowCursor(0); break;
		case CursorVisibility::Shown: SDL_ShowCursor(1); break;
		}
	}
	virtual const CursorVisibility cursorVisibility() const override {
		return SDL_ShowCursor(-1) ? CursorVisibility::Shown : CursorVisibility::Hidden;
	}

	virtual void set(Type type) override {
		switch (type) {
		case Type::Fullscreen: 
			SDL_SetWindowFullscreen(&window, SDL_WINDOW_FULLSCREEN); break;
		case Type::Windowed: 
			SDL_SetWindowFullscreen(&window, SDL_WINDOW_SHOWN); break;
		}
	}
	virtual const Type type() const override {
		SDL_GetWindowFlags(&window);
		return SDL_GetWindowFlags(&window) & SDL_WINDOW_FULLSCREEN
			? Type::Fullscreen : Type::Windowed;
	}

	virtual void set(Resolution resolution) override {
		SDL_SetWindowSize(&window, resolution.width, resolution.height);
		SDL_RenderSetLogicalSize(&renderer, resolution.width, resolution.height);
	}
	virtual Resolution resolution() const override {
		int x,y;
		SDL_RenderGetLogicalSize(&renderer, &x, &y);
		return { x,y };
	}

};