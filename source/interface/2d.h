#pragma once
namespace ezGame {
	
	using Pixel = int;
	struct Position {
		Pixel x, y;
		Position(Pixel x, Pixel y) : x(x), y(y) {}
		Position operator + (const Position& pos) { return{ x + pos.x, y + pos.y }; }
		Position operator - (const Position& pos) { return{ x - pos.x, y - pos.y }; }
	};

	struct Size {
		Size(const Size&) = default;
		Size(Pixel width, Pixel height) : width(width), height(height) { }
		Pixel width, height;
		Size operator + (const Size& size) const {return{ this->width + size.width,this->height + size.height }; }
		Size operator - (const Size& size) const { return { width - size.width,height - size.height }; }

		using Scale = double;
		struct Ratio { Scale width, height; };
		Ratio operator / (const Size& size) const { return { width/(Scale)size.width, height/(Scale)size.height }; }
	} const DEFAULT_SIZE = Size(-1, -1);
	using Resolution = Size;
	constexpr bool operator==(const Size &a, const Size &b) { return a.height == b.height && a.width == b.width; }
	constexpr bool operator!=(const Size &a, const Size &b) { return a.height != b.height || a.width != b.width; }
	
	namespace Relative {
		using Fraction = double;
		struct Size {
			Fraction width, height;
			Size(Fraction width, Fraction height) : width(width), height(height) {}
		};

		struct Position{
			Fraction x, y;
			Position(Fraction x, Fraction y) : x(x), y(y) {}
		}; 
	}

}