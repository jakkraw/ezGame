#pragma once
#include "draw.h"

namespace ezGame {
	Position operator*(const Position& lhs, const Size::Ratio& rhs) {
		return{ lhs.x*rhs.width , lhs.y*rhs.height };
	}

	struct DrawView : Draw {
		struct View {

			const Area area;
			const Size targetSize;

			Size::Ratio ratio() {
				return area.size /targetSize;
			}

			View(const Area& area, const Size& targetSize) : area(area), targetSize(targetSize) {}
			View(const Area& area) : area(area), targetSize({1,1}) {}
		};
		View view;
		Draw& draw;
		DrawView(Draw& draw, const View view) : view(view), draw(draw) {}


		Position transform(const RelativePosition& position) {
			return transform(position * view.targetSize);
		}

		Position transform (const Position& position) {
			return position * view.ratio() + view.area.topLeft;
		}


		Size transform(const RelativeSize& size) {
			return transform(size * view.targetSize);
		}

		Size transform(const Size& size) {
			return size * view.ratio();
		}

		Line transform(const Line& l) {
			return{ transform(l.start), transform(l.end) };
		}

		Line transform(const RelativeLine& l) {
			return{ transform(l.start), transform(l.end) };
		}

		Area transform(const Area& area) {
			return{ transform(area.topLeft), transform(area.size) };
		}

		Area transform(const RelativeArea& area) {
			return{ transform(area.topLeft), transform(area.size) };
		}

		void text(Text text, const Area& maximal, const Color& font) override {
			return draw.text(text, transform(maximal), font);
		}
		void text(Text text, const RelativeArea& maximal, const Color& font) override {
			return draw.text(text, transform(maximal), font);
		}
		void rectangle(const Area& a , const Color& c , Type t) override {
			return draw.rectangle(transform(a), c, t);
		}
		void rectangle(const RelativeArea& a , const Color& c , Type t ) override {
			return draw.rectangle(transform(a), c, t);
		}
		void point(const Position& p, const Color&c) override {
			return draw.point(transform(p), c);
		}
		void point(const RelativePosition& p, const Color& c) override {
			return draw.point(transform(p), c);
		}
		void line(const Line& l, const Color& c) override {
			return draw.line(transform(l), c);
		}
		void line(const RelativeLine& l, const Color& c) override {
			return draw.line(transform(l), c);
		}
		void image(Path p, const Area& a, Degrees rotation) override {
			return draw.image(p,transform(a), rotation);
		}
		void image(Path p, const RelativeArea& a, Degrees rotation) override {
			return draw.image(p, transform(a), rotation);
		}
	};
}