#pragma once
#include <vector>
#include "sdl.h"
#include "interface\input.h"

using namespace ezGame;
class Input : public virtual ezGame::Input {
	class Keys {
		std::vector<KeyState> keys;

		void reset(std::vector<KeyState>& buttons) {
			for (auto& button : buttons) {
				button.released = false;
				button.pressed = false;
			}
		}
		void keyDown(KeyState& button) {
			button.pressed = !button.down;
			button.down = true;
			button.released = false;
		}
		void keyUp(KeyState& button) {
			button.released = true;
			button.down = false;
			button.pressed = false;
		}
	public:
		Keys(size_t size) : keys(size) {}
		void setUp(size_t k) { keyUp(keys[k]); }
		void setDown(size_t k) { keyDown(keys[k]); }
		void reset() { reset(keys); }
		KeyState operator[](size_t k) const { return keys[k]; }
	};
	struct Mouse : public ezGame::Mouse {
		using Key = ::Mouse::Key;
		Keys keys = Keys(10);
		Position _cursor = { -1, -1 };
		void reset() { keys.reset(); }
		void setUp(Uint8 m) { keys.setUp(m); }
		void setDown(Uint8 m) { keys.setDown(m); }
		void setCursor(Position c) { _cursor = c; }

		// Inherited via Mouse
		virtual const KeyState key(Key k) const override {
			return keys[k];
		}

		virtual const Position cursor() const override {
			return _cursor;
		}

	};
	struct Keyboard : public ezGame::Keyboard {
		using Key = ::Keyboard::Key;
		Keys keys = Keys(300);
		void reset() { keys.reset(); }
		void setUp(SDL_Scancode k) { keys.setUp(k); }
		void setDown(SDL_Scancode k) { keys.setUp(k); }

		// Inherited via Keyboard
		virtual const KeyState key(Key k) const override {
			return keys[k];
		}
	};

	Keyboard _keyboard;
	Mouse _mouse;

	void reset() {
		_keyboard.reset();
		_mouse.reset();
	}

	void update(const SDL_Event& event) {
		switch (event.type){
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