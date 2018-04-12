#include <vector>

#include "loaders.h"
#include <atomic>
#include <memory>
#include "../SDL2/include/SDL.h"

#include "interface\window.h"
#include "interface\settings.h"
using namespace ezGame;



class SDL {
	

	struct Lifetime {
		Lifetime() {
			SDL_Init(SDL_INIT_VIDEO);
			IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_JPG);
			Mix_Init(MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_OGG);
			TTF_Init();
			Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
		}
		~Lifetime() {
			Mix_CloseAudio();
			TTF_Quit();
			Mix_Quit();
			IMG_Quit();
			SDL_Quit();
		}
	}const lifetime;

public:
	using Window = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;
	Window window = { nullptr, SDL_DestroyWindow };

	using Renderer = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
	Renderer renderer = { nullptr, SDL_DestroyRenderer };

	using Loaders = std::unique_ptr<Loaders>;
	Loaders loaders = { nullptr };

	SDL(Title title = "ezGame", Resolution res = { 640,480 }) {
		Size size(res);
		window.reset(SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.width, size.height, NULL));
		renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED | SDL_RendererFlags::SDL_RENDERER_TARGETTEXTURE));
		loaders.reset(new ::Loaders(*renderer));
		auto icon = IMG_Load_RW(SDL_RWFromConstMem(ez_ico, ez_ico_size), true);
		SDL_SetWindowIcon(window.get(), icon);
		SDL_FreeSurface(icon);

		SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BlendMode::SDL_BLENDMODE_BLEND);
		SDL_RenderSetLogicalSize(renderer.get(), 640, 480);

		//SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_SetWindowGrab(window.get(), SDL_TRUE);
		SDL_WarpMouseInWindow(window.get(), res.width / 2, res.height / 2);
		//SDL_ShowCursor(0);
	}
};




#include "interface\input.h"
class InputImpl : public virtual Input {
	static void reset(std::vector<KeyState>& buttons) {
		for (auto& button : buttons) {
			button.released = false;
			button.pressed = false;
		}
	}

	static void keyDown(KeyState& button) {
		button.pressed = !button.down;
		button.down = true;
		button.released = false;
	}
	static void keyUp(KeyState& button) {
		button.released = true;
		button.down = false;
		button.pressed = false;
	}

	struct Mouse : public ::Mouse{
		using Key = ::Mouse::Key;
		std::vector<KeyState> keys = std::vector<KeyState>(10);
		Position _cursor = { -1, -1 };
		void reset() { InputImpl::reset(keys);}
		void setUp(Uint8 m) { keyUp(keys[m]); }
		void setDown(Uint8 m) { keyDown(keys[m]); }
		void setCursor(Position c) { _cursor = c; }

		// Inherited via Mouse
		virtual const KeyState key(Key key) const override {
			return keys[key];
		}

		virtual const Position cursor() const override {
			return _cursor;
		}

	} _mouse;

	struct Keyboard : public ::Keyboard {
		using Key = ::Keyboard::Key;
		std::vector<KeyState> keys = std::vector<KeyState>(300);
		void reset() { InputImpl::reset(keys); }
		void setUp(SDL_Scancode k) { keyUp(keys[k]);}
		void setDown(SDL_Scancode k) { keyDown(keys[k]);}

		// Inherited via Keyboard
		virtual const KeyState key(Key key) const override {
			return keys[key];
		}

	} _keyboard;

public:

	void reset() {
		_keyboard.reset();
		_mouse.reset();
	}

	void update(const SDL_Event& event) {
			switch (event.type)
			{
			case SDL_KEYDOWN:
				_keyboard.setDown(event.key.keysym.scancode);
				break;
			case SDL_KEYUP:
				_keyboard.setUp(event.key.keysym.scancode);
				break;
			case SDL_MOUSEBUTTONUP:
				_mouse.setUp(event.button.button);
				break;
			case SDL_MOUSEBUTTONDOWN:
				_mouse.setDown(event.button.button);
				break;
			case SDL_MOUSEMOTION:
				_mouse.setCursor({ event.motion.x + 1, event.motion.y + 1 });
				break;
			}
	}

	// Inherited via Input
	virtual const Mouse & mouse() const override { return _mouse; }
	virtual const Keyboard & keyboard() const override { return _keyboard; }
};

#include "interface\draw.h"
class DrawImpl : public Draw {

	SDL& sdl;
	Color background = BLACK;
	
	void set(const Color & c) {
		SDL_SetRenderDrawColor(sdl.renderer.get(), c.red, c.green, c.blue, Uint8(255 * c.alpha));
	}


public:
	DrawImpl(SDL& sdl) : sdl(sdl) {}

	void reset() {
		SDL_RenderPresent(sdl.renderer.get());
		set(background);
		SDL_RenderClear(sdl.renderer.get());
	}

	void update(const SDL_Event& event) {}

	// Inherited via Draw
	virtual const Resolution resolution() const override {
		int w, h;
		SDL_RenderGetLogicalSize(sdl.renderer.get(), &w, &h);
		return{ w,h };
	}

	virtual void draw(const Rectangle& r) override {
		set(r.color);
		SDL_Rect rect{ r.topLeft.x, r.topLeft.y, r.size.width, r.size.height };
		r.type == ShapeType::Filled ? SDL_RenderFillRect(sdl.renderer.get(), &rect) : SDL_RenderDrawRect(sdl.renderer.get(), &rect);
	}

	virtual void draw(const Line &l) override {
		set(l.color);
		SDL_RenderDrawLine(sdl.renderer.get(), l.begin.x, l.begin.y, l.end.x, l.end.y);
	}

	virtual void draw(const Point &p) override {
		set(p.color);
		SDL_RenderDrawPoint(sdl.renderer.get(), p.x, p.y);
	}

	virtual void draw(const Image &t) override {
		SDL_Rect destination{ t.topLeft.x, t.topLeft.y, t.size.width, t.size.height };

		SDL_RenderCopyEx(sdl.renderer.get(), sdl.loaders->texture[t.cPath()].data, NULL,
			t.size == DEFAULT_SIZE ? nullptr : &destination,
			t.angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
	}

	virtual void draw(const Text &t) override {
		auto co = t.color;
		auto c = SDL_Color{ co.red, co.green, co.blue , Uint8(255 * co.alpha) };
		auto textSurface = TTF_RenderText_Solid(sdl.loaders->text.font, t.cText(), c);
		auto mTexture = SDL_CreateTextureFromSurface(sdl.renderer.get(), textSurface);
		SDL_FreeSurface(textSurface);

		int w, h;
		SDL_QueryTexture(mTexture, nullptr, nullptr, &w, &h);

		if (t.max != DEFAULT_SIZE && t.max.width < w || t.max.height < h) {
			auto multiplier = __min(t.max.width / (double)w, t.max.height / (double)h);
			w = int(w * multiplier);
			h = int(h * multiplier);
		}

		auto rect = SDL_Rect{ t.topLeft.x ,t.topLeft.y , w, h };
		SDL_RenderCopyEx(sdl.renderer.get(), mTexture, nullptr, &rect, 0, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
		SDL_DestroyTexture(mTexture);
	}

};

#include "interface\audio.h"
class AudioImpl : public Audio {
	SDL& sdl;
	short channel = 0;
public:
	AudioImpl(SDL& sdl) : sdl(sdl){}
	// Inherited via Audio
	virtual void play(const Music &m) override {
		Mix_PlayMusic(sdl.loaders->music[m.cPath()].data, m.loopTimes == LoopTimes::Infinite ? -1 : 0);
	}
	virtual void play(const Effect &e) override {
		channel = channel++ % 16;
		Mix_PlayChannel(channel, sdl.loaders->chunk[e.cPath()].data, e.loopTimes == LoopTimes::Infinite ? -1 : 0);
	}
	virtual void stopMusic() override {
		Mix_HaltMusic();
	}
	virtual void stopAllEffects() override {
		Mix_HaltChannel(-1);
	}
	virtual void stopAllSound() override {
		stopMusic();
		stopAllEffects();
	}
};

class WindowImpl : public virtual Window, public virtual Window::Settings {

	SDL& sdl;
	std::atomic<bool> open = true;
public:	
	WindowImpl(SDL& sdl) : sdl(sdl) {}

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

	// Inherited via Setup
	virtual void set(Title title) override {
		SDL_SetWindowTitle(sdl.window.get(), title);
	}
	virtual const Title title() const override {
		SDL_GetWindowTitle(sdl.window.get());
	}

	 // Inherited via Settings
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
			SDL_SetWindowFullscreen(sdl.window.get(), SDL_WINDOW_FULLSCREEN); break;
		case Type::Windowed: 
			SDL_SetWindowFullscreen(sdl.window.get(), SDL_WINDOW_SHOWN); break;
		}
	}
	virtual const Type type() const override {
		SDL_GetWindowFlags(sdl.window.get());
		return SDL_GetWindowFlags(sdl.window.get()) & SDL_WINDOW_FULLSCREEN
			? Type::Fullscreen : Type::Windowed;
	}

	virtual void set(Resolution resolution) override {
		SDL_SetWindowSize(sdl.window.get(), resolution.width, resolution.height);
		SDL_RenderSetLogicalSize(sdl.renderer.get(), resolution.width, resolution.height);
	}

	virtual Resolution resolution() const override {
		int x,y;
		SDL_RenderGetLogicalSize(sdl.renderer.get(), &x, &y);
		return { x,y };
	}

};