#pragma once
#include "2d.h"

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

		RelativeSize operator * (const Size::Ratio& b) const {
			return{ width * b.width, height * b.height };
		}

	};


	inline Position operator * (const RelativePosition& p, const Size& s) {
		return{ p.x * s.width, p.y * s.height };
	}

	inline Size operator * (const RelativeSize& p, const Size& s) {
		return{ p.width * s.width, p.height * s.height };
	}

	struct RelativeArea {
		RelativePosition topLeft;
		RelativeSize size;
		RelativeArea(const RelativePosition p, const RelativeSize s) : topLeft(p), size(s) {}
	};

	struct RelativeLine {
		RelativePosition start, end;
		RelativeLine(const RelativePosition begin, const RelativePosition end) : start(begin), end(end) {}
	};
}