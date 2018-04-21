#pragma once
#include "2d.h"
#include "relative2d.h"
#include <string>
#include <sstream>

namespace ezGame {

	struct Color {
		//[0,1]
		using Opacity = double;
		using Component = unsigned char;
		Component red, green, blue; 
		Opacity opacity = 1.f;

		Color(const Component red, const Component green, const Component blue, const Opacity opacity = 1.f) :
			red(red), green(green), blue(blue), opacity(opacity) {}
		
		Color with(const Opacity opacity) const { return { red, green, blue, opacity }; }
	};

	const Color RED(255, 0, 0), GREEN(0, 255, 0), BLUE(0, 0, 255), WHITE(255, 255, 255), BLACK(0, 0, 0);


	struct Text{
		Text(const char* c) : s(c){}
		Text(const double c) : s(::std::to_string(c)) {}
		Text(const int c) : s(::std::to_string(c)) {}
		Text(const bool c) : s(::std::to_string(c)) {}
		Text(std::string s) : s(std::move(s)){}
		operator std::string() const { return s; }
		operator const char* () const { return s.c_str(); }
	private:
		std::string s;
	};

	inline Text operator + (Text a, Text b){
		::std::stringstream s;
		s << a << b;
		return Text(s.str());
	}

	struct Draw 
	{
		using Text = const char*;
		virtual void text(Text, const Area& maximal, const Color& font = WHITE) = 0;
		virtual void text(Text, const RelativeArea& maximal, const Color& font = WHITE) = 0;

		enum class Type { FILLED, OUTLINE };
		virtual void rectangle(const Area&, const Color& = BLACK, Type = Type::FILLED) = 0;
		virtual void rectangle(const RelativeArea&, const Color& = BLACK, Type = Type::FILLED) = 0;

		virtual void point(const Position&, const Color& = WHITE) = 0;
		virtual void point(const RelativePosition&, const Color& = WHITE) = 0;

		virtual void line(const Line&, const Color& = WHITE) = 0;
		virtual void line(const RelativeLine&, const Color& = WHITE) = 0;

		using Path = const char*; using Degrees = double;
		virtual void image(Path, const Area&, Degrees rotation = 0) = 0;
		virtual void image(Path, const RelativeArea&, Degrees rotation = 0) = 0;
	};

}