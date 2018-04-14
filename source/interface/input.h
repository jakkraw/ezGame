#pragma once
#include "2d.h"

namespace ezGame {
	struct KeyState {
		bool pressed, released, down;
	};

	struct Mouse {
		enum Key { LEFT = 1, MIDDLE, RIGHT };
		virtual const KeyState key(Key) const = 0;
		virtual const Position cursor() const = 0;
	};

	struct Keyboard
	{
		enum Key {
			// Numbers according to SDL_Scancodes
			RIGHT = 79, LEFT, DOWN, UP,
			A = 4, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			N1 = 30, N2, N3, N4, N5, N6, N7, N8, N9, N0,
			RETURN = 40, ESCAPE, BACKSPACE, TAB, SPACE, MINUS, EQUALS, LEFTBRACKET, RIGHTBRACKET,
			BACKSLASH, NONUSHASH, SEMICOLON, APOSTROPHE, GRAVE, COMMA, PERIOD, SLASH, CAPSLOCK,
			F1 = 58, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
			PRINTSCREEN = 70, SCROLLLOCK, PAUSE, INSERT, HOME, PAGEUP, DELETE, END, PAGEDOWN,
			LCTRL = 224, LSHIFT, LALT, LGUI, RCTRL, RSHIFT, RALT, RGUI
		};
		virtual const KeyState key(Key) const = 0;
	};


	struct Input
	{
		virtual const Mouse& mouse() const = 0;
		virtual const Keyboard& keyboard() const = 0;
	};
}

