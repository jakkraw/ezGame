#pragma once

namespace ezGame {
	using Fraction = double;
	struct RelativePosition {
		Fraction x, y;
		RelativePosition(const Fraction x, const Fraction y) :
			x(x), y(y) {}
	};

	struct RelativeSize {
		Fraction width, height; 
		RelativeSize(const Fraction width, const Fraction height) :
			width(width), height(height) {}
	};

	struct RelativeArea {
		RelativePosition position;
		RelativeSize size;
		RelativeArea(const RelativePosition p, const RelativeSize s) : position(p), size(s) {}
	};

	struct RelativeLine {
		RelativePosition begin, end;
		RelativeLine(const RelativePosition begin, const RelativePosition end) : begin(begin), end(end) {}
	};
}