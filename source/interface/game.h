#pragma once
#include <memory>
#include "window.h"
#include "input.h"
#include "audio.h"

namespace ezGame {
	struct Settings {
		virtual Window::Settings& window() const = 0;
	};

	struct StageFlow {
		virtual void moveTo(Stage*&&) = 0;
		using Seconds = double;
		virtual Seconds delta() const = 0;
	};

	struct ezGamea {
		virtual const Window& window() const = 0;
		virtual const Input& input() const = 0;
		virtual const Audio& audio() const = 0;
		virtual const Settings& settings() const = 0;
	};

	namespace Internal {
		struct Game {
			virtual void setup(Settings&) = 0;
			virtual std::unique_ptr<Stage> start() = 0;
		};
	}

	template<typename StartStage>
	class Game : public Internal::Game {
		virtual std::unique_ptr<Stage> start() {
			return std::unique_ptr<StartStage>();
		}
		virtual ~Game() = default;
	};

	
	struct Stage {
		virtual void setup(Settings&) {}
		virtual void cleanup(Settings&) {}
		virtual void logic(ezGamea&) = 0;
		virtual ~Stage() = default;
	};

}