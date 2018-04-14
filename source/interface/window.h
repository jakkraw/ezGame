#pragma once
#include "2d.h"

namespace ezGame {

	struct Window
	{
		virtual void close() = 0;

		struct Settings {
			enum class Type { Windowed, Fullscreen };
			virtual const Type type() const = 0;
			virtual void set(Type) = 0;

			enum class CursorVisibility { Hidden, Shown };
			virtual const CursorVisibility cursorVisibility() const = 0;
			virtual void set(CursorVisibility) = 0;

			using Title = const char*;
			virtual const Title title() const = 0;
			virtual void set(Title) = 0;
				
			virtual Resolution resolution() const = 0;
			virtual void set(Resolution) = 0;
		};
		virtual Settings& settings() const = 0;		
	};

}


