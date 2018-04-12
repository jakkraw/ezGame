#pragma once
#include <memory>
#include "stage.h"
#include "settings.h"

namespace ezGame {
	struct Settings {
		virtual Window::Settings& window() const = 0;
	};

	struct ezGame {
		virtual const Window& window() const = 0;
		virtual const Input& input() const = 0;
		virtual const Audio& audio() const = 0;
		virtual const Settings& settings() const = 0;
	};

	struct Game {
		virtual void setup(Settings&) {};
		virtual ~Game() = default;
		virtual std::unique_ptr<Stage> start() = 0;
	};

	
	struct Stage {
		virtual void setup(Settings&) {}
		virtual void cleanup(Settings&) {}
		virtual void logic(ezGame&) = 0;
		virtual ~Stage() = default;
	};

}