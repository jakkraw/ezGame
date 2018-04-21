#pragma once
#include "interface/game.h"
#include "flow.h"
#include "timer.h"
using namespace ezGame;

struct SettingsI : Settings {
	Window::Settings& _window;
	explicit SettingsI(Window::Settings& window) : _window(window) {}

	// Inherited via Settings
	Window::Settings & window() const override {
		return _window;
	}
};

struct EzI : Ez, Ez::Interfaces {
	Draw& _draw;
	Input& _input;
	Audio& _audio;
	Settings& _settings;
	Window& _window;
	Flow& _flow;
	Timer& _timer;

	EzI(Timer& timer, Flow& flow, Draw& draw,
		Input& input, Audio& audio,
		Window& window, Settings& settings) :
		_draw(draw), _input(input), _audio(audio), _settings(settings),
		_window(window), _flow(flow), _timer(timer) {}

	// Inherited via Ez
	void _moveTo(Stage* stage) override {
		_flow.moveTo(Flow::StagePtr(stage));
	}
	Seconds delta() const override {
		return _timer.delta();
	}

	void quit() override {
		_window.close();
	}

	Ez::Interfaces& interfaces() const override {
		return *(Ez::Interfaces*)this;
	}

	Draw & draw() const override {
		return _draw;
	}
	Input & input() const override {
		return _input;
	}
	Audio & audio() const override {
		return _audio;
	}
	Settings & settings() const override {
		return _settings;
	}
	KeyState key(Keyboard::Key key) const override {
		return _input.key(key);
	}
	KeyState key(Mouse::Key key) const override {
		return _input.key(key);
	}
	Position cursor() const override {
		return _input.cursor();
	}

	RelativePosition cursorRelative() const override
	{
		auto r = _settings.window().resolution();
		auto c = cursor();
		return{ c.x / (double)r.width, c.y / (double)r.height };
	}

	// Inherited via Ez
	void drawRectangle(const Area &a, const Color &c, Draw::Type t) override {
		_draw.rectangle(a, c, t);
	}
	void drawRectangle(const RelativeArea &a, const Color& c, Draw::Type t) override {
		_draw.rectangle(a, c, t);
	}
	void drawImage(Draw::Path p, const Area & a, Draw::Degrees r) override {
		_draw.image(p, a, r);
	}
	void drawImage(Draw::Path p, const RelativeArea & a, Draw::Degrees r) override {
		_draw.image(p, a, r);
	}
	void drawText(Draw::Text t, const Area & a, const Color & c) override {
		_draw.text(t, a, c);
	}
	void drawText(Draw::Text t, const RelativeArea & a, const Color & c) override {
		_draw.text(t, a, c);
	}
	void drawLine(const Line &a, const Color &c) override {
		_draw.line(a, c);
	}
	void drawLine(const RelativeLine &a, const Color &c) override {
		_draw.line(a, c);
	}
};