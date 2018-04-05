#pragma once

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
}