#include <vector>

#include "loaders.h"
#include "../SDL2/include/SDL.h"

#include "interface\window.h"
#include "interface\settings.h"
using namespace ezGame;

struct MouseData
{
	#define MouseKeysMaxSize 10
	MouseData() : buttons(MouseKeysMaxSize), pointer(-1, -1) { }
	std::vector<KeyState> buttons;
	Position pointer;
};

struct Loaders {
	TextLoader text;
	TextureLoader texture;
	Loader<MusicWrapper> music; 
	Loader<ChunkWrapper> chunk;
	Loaders(SDL_Renderer& renderer): texture(renderer), text(renderer) {}
};

struct SDLData {
	SDL_Window& window;
	SDL_Renderer& renderer;
	SDLData(SDL_Window& window,SDL_Renderer& renderer) : window(window),renderer(renderer) {}
};


class WindowSDL : public virtual Window, private virtual Settings {

	enum class LoopState {
		Running,
		Stopped
	};

	SDLData sdlData;
	Loaders loaders;
	MouseData mouse;
	std::vector<KeyState> keyboard;
	LoopState loopState = LoopState::Running;
	Color backgroundColor = BLACK;

	void setDraw(const Color&);
	const KeyState button(Mouse::Key) const;
	const KeyState button(Keyboard::Key) const;
public:	
	WindowSDL(SDLData&);

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