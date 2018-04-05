#pragma once
#include <string>

namespace ezGame {
	using Pixel = int;
	using Angle = double;
	using Seconds = double;
	using Scale = double;
	using Path = std::string;
	using Title = const char*;
	using TextString = std::string;
	using Relative = double;

	constexpr Angle Radian(double r) { return r * 180. / 3.14159265358979323846; }
	constexpr Angle Degree(double d) { return d; }

	struct Ratio {
		Scale width, height;
	};

	struct Size {
		Size(const Size&) = default;
		Size(Pixel width, Pixel height) : width(width), height(height) { }
		Pixel width, height;

		Size operator + (const Size& size) const {
			return{ this->width + size.width,this->height + size.height };
		}

		Size operator - (const Size& size) const {
			return { width - size.width,height - size.height };
		}

		Ratio operator / (const Size& size) const {
			return { width/(Scale)size.width, height/(Scale)size.height };
		}


	} const DEFAULT_SIZE = Size(-1, -1);
	constexpr bool operator==(const Size &a, const Size &b) { return a.height == b.height && a.width == b.width; }
	constexpr bool operator!=(const Size &a, const Size &b) { return a.height != b.height || a.width != b.width; }
	using Resolution = Size;

	struct RelativeSize : public Size {
		RelativeSize(const RelativeSize&) = default;
		RelativeSize(Relative width, Relative height,const Resolution& res) : Size(Pixel(width * res.width),Pixel(height * res.height)) {}
	};
	using RSize = RelativeSize;

	struct Position {
		Position(Pixel x, Pixel y) : x(x), y(y) { }
		Pixel x, y;

		Position operator + (const Position& pos) {
			return { x + pos.x, y + pos.y };
		}

		Position operator - (const Position& pos) {
			return { x - pos.x, y - pos.y };
		}

	};

	using Pos = Position;

	struct RelativePosition : public Position {
		RelativePosition(const RelativePosition&) = default;
		RelativePosition(Relative x, Relative y,const Resolution& res) : Position(Pixel(x * res.width), Pixel(y * res.height)) {}
	};
	using RPos = RelativePosition;
}