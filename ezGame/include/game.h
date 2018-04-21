#pragma once
#include <utility>
#include <memory>
#include "window.h"
#include "input.h"
#include "audio.h"
#include "draw.h"

namespace ezGame {
	struct Stage;

	struct Settings {
		virtual Window::Settings& window() const = 0;
	};

	class Ez {
		virtual void _moveTo(Stage*) = 0;
	public:
		struct Interfaces {
			virtual Draw& draw() const = 0;
			virtual Input& input() const = 0;
			virtual Audio& audio() const = 0;
		};

		using Seconds = double;
		virtual Seconds delta() const = 0;

		template<class NextStage, class... ConstructorParameters>
		void moveTo(ConstructorParameters&&... cp) { _moveTo(new NextStage(std::forward<ConstructorParameters>(cp)...)); }
		virtual void quit() = 0;

		virtual void drawLine(const Line&, const Color& = BLACK) = 0;
		virtual void drawLine(const RelativeLine&, const Color& = BLACK) = 0;

		virtual void drawRectangle(const Area&, const Color& = BLACK, Draw::Type = Draw::Type::FILLED) = 0;
		virtual void drawRectangle(const RelativeArea&, const Color& = BLACK, Draw::Type = Draw::Type::FILLED) = 0;

		virtual void drawImage(Draw::Path, const Area&, Draw::Degrees rotation = 0) = 0;
		virtual void drawImage(Draw::Path, const RelativeArea&, Draw::Degrees rotation = 0) = 0;

		virtual void drawText(Draw::Text, const Area& maximal, const Color& font = WHITE) = 0;
		virtual void drawText(Draw::Text, const RelativeArea& maximal, const Color& font = WHITE) = 0;

		virtual KeyState key(Keyboard::Key) const = 0;
		virtual KeyState key(Mouse::Key) const = 0;
		virtual Position cursor() const = 0;
		virtual RelativePosition cursorRelative() const = 0;

		virtual Interfaces& interfaces() const = 0;
		virtual Settings& settings() const = 0;
	};

	struct Stage {
		virtual void setup(Settings&) {}
		virtual void cleanup(Settings&) {}
		virtual void logic(Ez&) = 0;
		virtual ~Stage() = default;
	};

	struct Game {
		using EntryStage = std::unique_ptr<Stage>;
		virtual EntryStage entry() const = 0;
		virtual void setup(Settings&) const {};

		Stage* createEntry() const { return entry().release(); }

		protected:
			template<class FirstStage, class...ConstructorParameters>
			EntryStage Entry(ConstructorParameters&&... cp) const {
				return std::make_unique<FirstStage>(std::forward<ConstructorParameters>(cp)...);
			}
	};
}