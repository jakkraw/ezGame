#pragma once
#include <atomic>
#include <memory>
#include "SDL.h"

#include "interface/window.h"

class WindowI : 
	public virtual ezGame::Window,
	public virtual ezGame::Window::Settings
{

	using WindowSDL = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;
	WindowSDL windowSDL = { nullptr , SDL_DestroyWindow };

	using RendererSDL = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
	RendererSDL rendererSDL = { nullptr, SDL_DestroyRenderer };

	std::atomic<bool> open = true;
public:	

	WindowI(Title title, Resolution res, ezGame::Size size);

	SDL_Renderer& renderer() const;

	void update(const SDL_Event& event);
	bool isOpen() const;

	void close() override;

	// Inherited via Window
	Window::Settings & settings() const override;

	// Inherited via Settings
	void set(Title title) override;
	Title title() const override;

	void set(CursorVisibility cv) override;
	CursorVisibility cursorVisibility() const override;

	void set(Type type) override;
	Type type() const override;

	void set(Resolution resolution) override;
	Resolution resolution() const override;

};