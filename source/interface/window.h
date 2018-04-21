#pragma once
#include "2d.h"

namespace ezGame {

	struct Window
	{
		virtual void close() = 0;

		struct Settings {
			enum class Type { WINDOWED, FULLSCREEN };
			virtual Type type() const = 0;
			virtual void set(Type) = 0;

			enum class CursorVisibility { HIDDEN, SHOWN };
			virtual CursorVisibility cursorVisibility() const = 0;
			virtual void set(CursorVisibility) = 0;

			using Title = const char*;
			virtual Title title() const = 0;
			virtual void set(Title) = 0;
				
			using Resolution = Size;
			virtual Resolution resolution() const = 0;
			virtual void set(Resolution) = 0;
		};
		virtual Settings& settings() const = 0;		
	};

}