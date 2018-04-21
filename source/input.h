#pragma once
#include <vector>
#include "sdl.h"
#include "interface/input.h"

struct InputI : virtual ezGame::Input {
	using KeyState = ezGame::KeyState;
	using Mouse = ezGame::Mouse;
	using Keyboard = ezGame::Keyboard;
	using Position = ezGame::Position;

	class Keys {
		using KeyState = ezGame::KeyState;
		std::vector<KeyState> keys;
	public:
		Keys(size_t size) : keys(size, { false,false,false }) {}
		void setUp(size_t k) {
			auto& key = keys[k];
			key.released = true;
			key.down = false;
			key.pressed = false;
		}
		void setDown(size_t k) {
			auto& key = keys[k];
			key.pressed = !key.down;
			key.down = true;
			key.released = false;
		}
		void reset() {
			for (auto& key : keys) {
				key.released = false;
				key.pressed = false;
			}
		}
		KeyState operator[](size_t k) const { return keys[k]; }
	};

	struct MouseI {
		using Key = ezGame::Mouse::Key;
		using Position = ezGame::Position;
		using KeyState = ezGame::KeyState;
		Keys keys = Keys(Key::HIGHEST + 1);
		Position _cursor = { -1, -1 };
		void reset() { keys.reset(); }
		void setUp(Uint8 m) { keys.setUp(m); }
		void setDown(Uint8 m) { keys.setDown(m); }
		void setCursor(Position c) { _cursor = c; }
		Position cursor() const { return _cursor; }
		KeyState operator[](Key k) const { return keys[k]; }
	};

	struct KeyboardI {
		using KeyState = ezGame::KeyState;
		using Key = ezGame::Keyboard::Key;
		Keys keys = Keys(Key::HIGHEST + 1);
		void reset() { keys.reset(); }
		void setUp(SDL_Scancode k) { keys.setUp(k); }
		void setDown(SDL_Scancode k) { keys.setDown(k); }
		KeyState operator[](Key k) const { return keys[k]; }
	};

	KeyboardI _keyboard;
	MouseI _mouse;
public:
	void reset() {
		_keyboard.reset();
		_mouse.reset();
	}

	void update(const SDL_Event& event) {
		switch (event.type) {
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
	KeyState key(ezGame::Mouse::Key k) const override { return _mouse[k]; }
	Position cursor() const override { return _mouse.cursor(); }
	KeyState key(ezGame::Keyboard::Key k) const override { return _keyboard[k]; }
};