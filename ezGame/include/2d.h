#pragma once
namespace ezGame {
	using Pixel = int;

	struct Position {
		Pixel x, y;
		Position(Pixel x, Pixel y) : x(x), y(y) {}
		Position operator + (const Position& pos) const { return{ x + pos.x, y + pos.y }; }
		Position operator - (const Position& pos) const { return{ x - pos.x, y - pos.y }; }
	};

	struct Size {
		Size(const Size&) = default;
		Size& operator=(const Size&) = default;
		Size(const Pixel width, const Pixel height) : width(width), height(height) { }
		Pixel width, height;
		Size operator + (const Size& size) const { 
			return { width + size.width, height + size.height }; 
		}
		Size operator - (const Size& size) const { 
			return { width - size.width, height - size.height }; 
		}

		struct Ratio {
			using scale = double;
			scale width, height;
		};

		Ratio operator / (const Size& b) const { 
			return{ width / Ratio::scale(b.width), height / Ratio::scale(b.height) };
		}
	};
	
	inline bool operator == (const Size& a, const Size& b) {
		return a.height == b.height && a.width == b.width;
	}

	inline bool operator != (const Size& a, const Size &b) {
		return !(a == b);
	}

	struct Line {
		Position start, end;
		Line(const Position start, const Position end) : start(start), end(end) {}
	};

	struct Area {
		Position topLeft;
		Size size;
		Area(const Position topLeft, const Size size) : topLeft(topLeft), size(size){}
	};
}