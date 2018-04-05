#include <vector>

#include "loaders.h"
#include <atomic>
#include <memory>
#include "../SDL2/include/SDL.h"

#include "interface\window.h"
#include "interface\settings.h"
using namespace ezGame;

struct MouseData
{
	const size_t mouseKeysNumber = 10;
	std::vector<KeyState> buttons = std::vector<KeyState>(mouseKeysNumber);
	Position pointer = { -1, -1 };
};

struct KeyboardData {
	const size_t keyboardKeysNumber = 300;
	std::vector<KeyState> keys = std::vector<KeyState>(keyboardKeysNumber);
};

struct Loaders {
	TextLoader text;
	TextureLoader texture;
	Loader<MusicWrapper> music; 
	Loader<ChunkWrapper> chunk;
	Loaders(SDL_Renderer& renderer): texture(renderer), text(renderer) {}
};

struct SDL {
	using Window = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;
	Window window = { nullptr, SDL_DestroyWindow };

	using Renderer = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
	Renderer renderer = { nullptr, SDL_DestroyRenderer };

	using Loaders = std::unique_ptr<Loaders>;
	Loaders loaders = { nullptr };

	SDL() {
		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
		Title title("ezGame");
		Resolution res(640, 480);
		Size size(res);
		
		window.reset(SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.width, size.height, NULL));
		renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED | SDL_RendererFlags::SDL_RENDERER_TARGETTEXTURE));
		loaders.reset(new ::Loaders(*renderer));
		auto icon = IMG_Load_RW(SDL_RWFromConstMem(ez_ico, ez_ico_size), false);
		SDL_SetWindowIcon(window.get(), icon);
		SDL_FreeSurface(icon);

		SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BlendMode::SDL_BLENDMODE_BLEND);
		SDL_RenderSetLogicalSize(renderer.get(), 640, 480);

		//SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_SetWindowGrab(window.get(), SDL_TRUE);
		SDL_WarpMouseInWindow(window.get(), res.width / 2, res.height / 2);
		//SDL_ShowCursor(0);
	}

	~SDL() {
		Mix_CloseAudio();
	}
};


class WindowSDL : public virtual Window, private virtual Settings {

	enum class LoopState {
		Running,
		Stopped
	};

	SDL sdl;
	MouseData mouse;
	KeyboardData keyboard;
	std::atomic<LoopState> loopState = LoopState::Running;
	Color backgroundColor = BLACK;

	void setDraw(const Color&);
	const KeyState button(Mouse::Key) const;
	const KeyState button(Keyboard::Key) const;
public:	

	virtual void update();
	void drawAndClearRenderer();
	void setPointer(Position&);
	bool isRunning();

	void keyUp(Keyboard::Key);
	void keyDown(Keyboard::Key);
	void keyUp(Mouse::Key);
	void keyDown(Mouse::Key);

	// Inherited via Window
	void draw(const Rectangle&) override;
	void draw(const Line&) override;
	void draw(const Point&) override;
	void draw(const Image&) override;

	const Mouse::Position cursor() const override;
	void play(const Music&) override;
	void play(const Effect&) override;
	void stopMusic() override;
	void quit() override;
	void draw(const Text&) override;

	// Inherited via Draw
	virtual const Resolution resolution() const override;

	// Inherited via Window
	virtual Settings & settings() const override;


	// Inherited via Setup
	virtual void set(Title) override;


	// Inherited via Setup
	 virtual void set(Background) override;


	 // Inherited via Window
	 virtual const bool pressed(Mouse::Key) const override;

	 virtual const bool pressed(Keyboard::Key) const override;

	 virtual const bool down(Mouse::Key) const override;

	 virtual const bool down(Keyboard::Key) const override;

	 virtual const bool released(Mouse::Key) const override;

	 virtual const bool released(Keyboard::Key) const override;


	 // Inherited via Window
	 virtual void stopAllEffects() override;

	 virtual void stopAllSound() override;


	 // Inherited via Settings
	 virtual void set(CursorVisibility) override;

	 virtual void set(Window::Type) override;

	 virtual void set(Resolution) override;


	 // Inherited via Settings
	 virtual const CursorVisibility cursorVisibility() const override;

	 virtual const Window::Type windowType() const override;

	 virtual const Background background() const override;

	 virtual const Title title() const override;

};