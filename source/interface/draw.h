#pragma once
#include "common.h"
#include "info.h"
#include "color.h"

namespace ezGame {

	struct Color {
		using Alpha = float;
		Color() : red(255), blue(255), green(0), alpha(1.f) {  }
		Color(unsigned char r, unsigned char g, unsigned char b, Alpha alpha = 1.f) : red(r), green(g), blue(b), alpha(alpha) {	}
		unsigned char red, green, blue; Alpha alpha;
		Color with(Alpha alpha) const {
			return{ red,green,blue, alpha };
		}
	};

	const Color
		RED(255, 0, 0),
		GREEN(0, 255, 0),
		BLUE(0, 0, 255),
		WHITE(255, 255, 255),
		BLACK(0, 0, 0);
	
	struct Point {
		Point(const Position& p, const Color& color = WHITE) : x(p.x), y(p.y), color(color) {}
		Pixel x, y; Color color;
	};

	enum class ShapeType {
		Filled,
		Outline
	};

	struct Rectangle {
		Rectangle(const Position& topLeft, const Size& size, Color color = BLACK, ShapeType type = ShapeType::Filled) : topLeft(topLeft), size(size), color(color), type(type) {}
		Position topLeft; Size size; Color color; ShapeType type;
	};

	struct Line {
		Line::Line(const Position& begin, const Position& end, Color color = WHITE) : begin(begin), end(end), color(color) {}
		Position begin, end; Color color;
	};

	struct Circle {
		Circle(const Position& center, Pixel radius, Color color = WHITE, ShapeType type = ShapeType::Outline) : mouse(center), radius(radius), color(color), type(type) {}
		Position mouse; Pixel radius; Color color; ShapeType type;
	};

	
	struct Image {
		using Path = std::string;
		Image(Path path, const Position& topLeft, const Size& size = DEFAULT_SIZE, Angle angle = Radian(0)) : topLeft(topLeft), size(size), path(path), angle(angle) {}
		Path path; Position topLeft; Size size; Angle angle;

		const char* cPath() const {
			return path.c_str();
		}

	};

	struct Text {
		using TextString = std::string;
		TextString textString;
		Color color;
		Position topLeft;
		Size max;
		Text(TextString textString, const Position& topLeft, const Size& max = DEFAULT_SIZE, Color color = WHITE) : textString(textString), color(color), max(max), topLeft(topLeft) {}

		const char* cText() const {
			return textString.c_str();
		}
	};

	struct Draw : public virtual ResolutionInfo {
		virtual void draw(const Rectangle&) = 0;
		virtual void draw(const Line&) = 0;
		virtual void draw(const Point&) = 0;
		virtual void draw(const Image&) = 0;
		virtual void draw(const Text&) = 0;
	};


}