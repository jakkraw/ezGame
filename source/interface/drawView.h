#pragma once
#include "draw.h"

namespace ezGame {
	struct DrawView : Draw {
		struct View {
			struct Fragment {
				Fragment(const Position& offset, const Size& size)
					: offset(offset), size(size){}

				const Position offset;
				const Size size;
			};
			const Fragment fragment;
			const Size targetSize;
			View(const Fragment& fragment, const Size& targetSize) : fragment(fragment), targetSize(targetSize) {}
		};
		View view;
		Draw& draw;
		DrawView(Draw& draw, const View view) : view(view), draw(draw) {}
	};
}